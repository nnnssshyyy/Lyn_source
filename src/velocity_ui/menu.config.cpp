#include "common.hpp"
#include "settings.hpp"
#include "rendering.hpp"
#include "ui_preferences.hpp"


namespace rendering {

	namespace detail {

		std::string search_buf{};
		std::string new_name_buf{};
		std::string status_message{};
		bool status_error{};
		std::vector<std::wstring> config_list{};
		std::wstring selected_config_name{};
        std::wstring active_profile_name{},confirm_load_name{};
        std::string active_profile_label{};
        std::uint64_t saved_fingerprint{};
        bool baseline_ready{},dirty{},detached{};
        std::chrono::steady_clock::time_point profile_poll_at{},confirm_load_until{};
		auto selected{ -1 };
		auto needs_refresh{ true };
		auto confirm_delete{ false };
		auto confirm_reset{ false };
		auto confirm_timer{ 0.0f };

		static inline bool copy_to_clipboard( const std::string& text )
		{
			const auto size = ( text.size( ) + 1 ) * sizeof( char );
			auto mem = GlobalAlloc( GMEM_MOVEABLE, size );
			if ( !mem )
			{
				return false;
			}

			auto dest = GlobalLock( mem );
			if ( !dest )
			{
				GlobalFree( mem );
				return false;
			}
			std::memcpy( dest, text.c_str( ), size );
			GlobalUnlock( mem );

			if ( !OpenClipboard( nullptr ) )
			{
				GlobalFree( mem );
				return false;
			}
			const auto copied = EmptyClipboard( ) && SetClipboardData( CF_TEXT, mem ) != nullptr;
			CloseClipboard( );
			if ( !copied )
			{
				GlobalFree( mem );
			}
			return copied;
		}

		static inline std::string paste_from_clipboard( )
		{
			if ( !OpenClipboard( nullptr ) )
			{
				return {};
			}

			std::string result{};

			auto mem = GetClipboardData( CF_TEXT );
			if ( mem )
			{
				auto data = static_cast< const char* >( GlobalLock( mem ) );
				if ( data )
				{
					result = data;
					GlobalUnlock( mem );
				}
			}

			CloseClipboard( );
			return result;
		}

		static inline std::string wide_to_utf8( std::wstring_view wide )
		{
			if ( wide.empty( ) ) return {};
			const auto len = WideCharToMultiByte( CP_UTF8, WC_ERR_INVALID_CHARS, wide.data( ), static_cast< int >( wide.size( ) ), nullptr, 0, nullptr, nullptr );
			if ( len <= 0 ) return {};
			std::string result( len, '\0' );
			if ( WideCharToMultiByte( CP_UTF8, WC_ERR_INVALID_CHARS, wide.data( ), static_cast< int >( wide.size( ) ), result.data( ), len, nullptr, nullptr ) != len ) return {};
			return result;
		}

		static inline std::wstring utf8_to_wide( std::string_view utf8 )
		{
			if ( utf8.empty( ) ) return {};
			const auto len = MultiByteToWideChar( CP_UTF8, MB_ERR_INVALID_CHARS, utf8.data( ), static_cast< int >( utf8.size( ) ), nullptr, 0 );
			if ( len <= 0 ) return {};
			std::wstring result( len, L'\0' );
			if ( MultiByteToWideChar( CP_UTF8, MB_ERR_INVALID_CHARS, utf8.data( ), static_cast< int >( utf8.size( ) ), result.data( ), len ) != len ) return {};
			return result;
		}

		static inline std::string trim_name( std::string_view text )
		{
			while ( !text.empty( ) && std::isspace( static_cast< unsigned char >( text.front( ) ) ) ) text.remove_prefix( 1 );
			while ( !text.empty( ) && std::isspace( static_cast< unsigned char >( text.back( ) ) ) ) text.remove_suffix( 1 );
			if ( std::any_of( text.begin( ), text.end( ), [ ]( char c ) { return static_cast< unsigned char >( c ) < 32 || c == 127; } ) ) return {};
			return std::string{ text };
		}

		static inline bool name_exists( std::wstring_view name )
		{
			for ( const auto& existing : config_list )
			{
				if ( CompareStringOrdinal( existing.c_str( ), -1, name.data( ), static_cast< int >( name.size( ) ), TRUE ) == CSTR_EQUAL ) return true;
			}
			return false;
		}

		static inline void show_status( std::string message, bool error = false )
		{
			status_message = std::move( message );
			status_error = error;
		}

		static inline bool config_matches_search( const std::wstring& wname )
		{
			if ( detail::search_buf.empty( ) )
			{
				return true;
			}

			std::string lower_name = wide_to_utf8( wname );
			std::string lower_search{ detail::search_buf };

			for ( auto& c : lower_name )
			{
				c = static_cast< char >( std::tolower( static_cast< unsigned char >( c ) ) );
			}

			for ( auto& c : lower_search )
			{
				c = static_cast< char >( std::tolower( static_cast< unsigned char >( c ) ) );
			}

			return ui::search_text(wide_to_utf8(wname)).find(ui::search_text(detail::search_buf))!=std::string::npos;
		}

		static inline std::string selected_name( )
		{
			if ( detail::selected < 0 || detail::selected >= static_cast< int >( detail::config_list.size( ) ) )
			{
				return {};
			}

			return wide_to_utf8( detail::config_list[ detail::selected ] );
		}

		static inline void reset_defaults( )
		{
			auto& reg = config::detail::get_registry( );

			for ( auto& f : reg.fields )
			{
				char key_str[ 12 ];
				std::snprintf( key_str, sizeof( key_str ), "%08x", f.key );

				auto def = reg.defaults.find( key_str );
				if ( def != reg.defaults.end( ) )
				{
					config::serial::json_to_field( *def, f );
				}
			}

			settings::finalize_binds( );
		}

        std::uint64_t profile_fingerprint() {
            std::uint64_t hash=14695981039346656037ull;
            const auto add=[&](const void* data,size_t count) {
                const auto* bytes=static_cast<const unsigned char*>(data);
                for(size_t i=0;i<count;++i) {hash^=bytes[i];hash*=1099511628211ull;}
            };
            for(const auto& field:config::detail::get_registry().fields) {
                add(&field.key,sizeof(field.key));add(&field.type,sizeof(field.type));
                using type=config::field_type;
                switch(field.type) {
                case type::setting: {
                    const auto& value=*static_cast<const xui::setting*>(field.ptr);
                    add(&value.value,sizeof(value.value));add(&value.bind.key,sizeof(value.bind.key));
                    if(value.bind.key)add(&value.bind.mode,sizeof(value.bind.mode));
                    break;
                }
                case type::bool_val:add(field.ptr,sizeof(bool));break;
                case type::int_val:add(field.ptr,sizeof(int));break;
                case type::uint8_val:add(field.ptr,sizeof(std::uint8_t));break;
                case type::float_val:add(field.ptr,sizeof(float));break;
                case type::color:add(field.ptr,sizeof(xdraw::color));break;
                case type::float3:add(field.ptr,sizeof(config::float3));break;
                case type::bool_array:add(&field.count,sizeof(field.count));add(field.ptr,field.count*sizeof(bool));break;
                case type::string_val: {
                    const auto& value=*static_cast<const std::string*>(field.ptr);const auto size=value.size();
                    add(&size,sizeof(size));add(value.data(),size);break;
                }
                case type::custom: {
                    const auto value=config::serial::field_to_json(field).dump();add(value.data(),value.size());break;
                }
                }
            }
            return hash;
        }

	} // namespace detail

	void menu::draw_config( float group_w )
	{
		( void )group_w;

		if ( detail::needs_refresh )
		{
			detail::config_list = config::registry::list( );
			detail::needs_refresh = false;
			const auto found = std::find( detail::config_list.begin( ), detail::config_list.end( ), detail::selected_config_name );
			detail::selected = found == detail::config_list.end( ) ? -1 : static_cast< int >( std::distance( detail::config_list.begin( ), found ) );
			if ( detail::selected < 0 ) detail::selected_config_name.clear( );
		}

		const auto dt = xdraw::delta_time( );

		if ( detail::confirm_delete || detail::confirm_reset )
		{
			detail::confirm_timer += dt;

			if ( detail::confirm_timer > 5.0f )
			{
				detail::confirm_delete = false;
				detail::confirm_reset = false;
			}
		}

		auto& dl = xui::draw::current( );
		const auto& s = xui::ctx( ).style;
		const auto& input = xui::ctx( ).input;

		xui::layout::set_cursor( this->m_body_x - this->m_x, this->m_body_y - this->m_y );

		if ( !xui::begin_child( "##cfg_panel", this->m_body_w, this->m_body_h, true ) )
		{
			return;
		}

		ui::poll_profile_state();
        xdraw::push_font(g_fonts.menu_heading);
        xui::text("Saved profiles",tokens::col_text);xdraw::pop_font();
        const auto active=ui::active_profile();
        xui::text_raw(std::string(ui::translate("Active profile: "))+
            std::string(active.empty()?ui::translate("none"):active)+"  /  "+
            std::string(ui::translate(ui::profile_dirty()?"Unsaved changes":(active.empty()?"Current settings":"Saved"))),
            ui::profile_dirty()?tokens::col_accent:tokens::col_text_dim);
		xui::text_input( "##cfg_search", detail::search_buf, 64, "Search profiles" );
		xui::text( "New profile name", tokens::col_text_dim );
		xui::text_input( "##cfg_new_name", detail::new_name_buf, 64 );

		constexpr auto btn_h{ 28.0f };
		const auto [ avail_w, avail_h ] = xui::layout::avail( );
		const auto list_h = std::max( 80.0f, avail_h - btn_h * 2.0f - 75.0f - s.item_spacing_y * 6.0f );

		if ( xui::begin_child( "##cfg_list", avail_w, list_h, true ) )
		{
			const auto row_w = xui::layout::avail( ).first;
			constexpr auto row_h{ 28.0f };
			auto visible_rows{ 0 };

			for ( auto i = 0; i < static_cast< int >( detail::config_list.size( ) ); ++i )
			{
				const auto& wname = detail::config_list[ i ];

				if ( !detail::config_matches_search( wname ) )
				{
					continue;
				}

				const auto narrow = detail::wide_to_utf8( wname );

				const auto row = xui::layout::item( row_w, row_h );
				const auto is_selected = ( detail::selected == i );
				const auto is_hovered = input.in_rect( row );

				if ( is_hovered && input.mouse_clicked && !xui::ctx( ).overlay_blocking( ) )
				{
					detail::confirm_delete = false;
					detail::confirm_reset = false;
                    detail::selected=i;
                    detail::selected_config_name=wname;
                    detail::confirm_load_name.clear();
				}

				const auto hover_anim = xui::anim::lerp( xui::fnv1a( "cfgrow" ) + i, is_hovered ? 1.0f : 0.0f, 14.0f );
				const auto sel_anim = xui::anim::lerp( xui::fnv1a( "cfgsel" ) + i, is_selected ? 1.0f : 0.0f, 10.0f );

				if ( sel_anim > 0.01f )
				{
					dl.rect_filled( row.x, row.y, row.w, row.h, tokens::col_accent.alpha( static_cast< std::uint8_t >( 70.0f * sel_anim ) ), xdraw::corner_radius{ 6.0f } );
				}
				else if ( hover_anim > 0.01f )
				{
					dl.rect_filled( row.x, row.y, row.w, row.h, tokens::col_elevated.alpha( static_cast< std::uint8_t >( 255.0f * hover_anim * 0.5f ) ), xdraw::corner_radius{ 6.0f } );
				}

				const auto [ tw, th ] = xdraw::measure_text( narrow );
				const auto text_col = is_selected
					? xui::lerp( tokens::col_text, tokens::col_accent, sel_anim )
					: xui::lerp( tokens::col_text_dim, tokens::col_text, hover_anim );

				dl.text( row.x + 10.0f, row.y + ( row.h - th ) * 0.5f, narrow, text_col );

				visible_rows++;
			}

			if ( visible_rows == 0 )
			{
				const auto row = xui::layout::item( row_w, row_h );
				dl.text( row.x + 10.0f, row.y + 6.0f, ui::translate(detail::config_list.empty()?"no configs found":"no matches"), tokens::col_text_dim );
			}

			xui::end_child( );
		}

		const auto has_selection = detail::selected >= 0 && detail::selected < static_cast< int >( detail::config_list.size( ) );
		auto selected_label = has_selection ? detail::selected_name( ) : std::string(ui::translate("none"));
		if ( selected_label.size( ) > 48 ) selected_label = selected_label.substr( 0, 45 ) + "...";
		xui::text_raw(std::string(ui::translate("Selected: "))+selected_label,tokens::col_text_dim);
		const auto btn_w = ( avail_w - s.item_spacing_x * 3.0f ) / 4.0f;

		if ( xui::button( "save selected", btn_w, btn_h ) )
		{
			if ( !has_selection ) detail::show_status( "Select a profile before saving changes.", true );
			else
			{
				const auto saved = config::registry::save( detail::config_list[ detail::selected ] );
                if(saved)ui::profile_saved(detail::config_list[detail::selected]);
				detail::show_status( saved ? "Selected profile saved." : "Could not save the selected profile.", !saved );
			}
		}

		xui::layout::same_line( );
		if ( xui::button( "save as new", btn_w, btn_h ) )
		{
			const auto trimmed = detail::trim_name( detail::new_name_buf );
			const auto wide_name = detail::utf8_to_wide( trimmed );
			if ( wide_name.empty( ) ) detail::show_status( "Enter a valid new profile name.", true );
			else if ( detail::name_exists( wide_name ) ) detail::show_status( "That name exists. Select it to update it.", true );
			else if ( config::registry::save( wide_name ) )
			{
				detail::selected_config_name = wide_name;
            ui::profile_saved(wide_name);
				detail::new_name_buf.clear( );
				detail::search_buf.clear( );
				detail::needs_refresh = true;
				detail::show_status( "New profile saved." );
			}
			else detail::show_status( "Could not save the new profile.", true );
		}

		xui::layout::same_line( );
		if ( xui::button( "load selected", btn_w, btn_h ) )
		{
			if ( !has_selection ) detail::show_status( "Select a profile to load.", true );
            else {
                const auto& chosen=detail::config_list[detail::selected];
                ui::poll_profile_state(true);
                const auto now=std::chrono::steady_clock::now();
                if(ui::profile_dirty()&&(detail::confirm_load_name!=chosen||now>detail::confirm_load_until)) {
                    detail::confirm_load_name=chosen;detail::confirm_load_until=now+std::chrono::seconds(5);
                    detail::show_status("Unsaved changes will be replaced. Load again to confirm.");
                } else {
                    detail::confirm_load_name.clear();
                    if(config::registry::load(chosen)) {
                        settings::finalize_binds();ui::profile_saved(chosen);detail::show_status("Profile loaded.");
                    } else detail::show_status("Could not load this profile. Its data may be damaged.",true);
                }
            }
		}

		xui::layout::same_line( );
		if ( detail::confirm_delete )
		{
			if ( xui::button( "confirm delete", btn_w, btn_h ) )
			{
				detail::confirm_delete = false;
				if ( has_selection && config::registry::remove( detail::config_list[ detail::selected ] ) )
				{
					ui::profile_removed(detail::config_list[detail::selected]);
                    detail::selected = -1;
                    detail::selected_config_name.clear( );
					detail::needs_refresh = true;
					detail::show_status( "Profile deleted." );
				}
				else detail::show_status( "Could not delete this profile.", true );
			}
		}
		else if ( xui::button( "delete selected", btn_w, btn_h ) )
		{
			if ( !has_selection ) detail::show_status( "Select a profile to delete.", true );
			else
			{
				detail::confirm_delete = true;
				detail::confirm_reset = false;
				detail::confirm_timer = 0.0f;
				detail::show_status( "Click confirm delete within five seconds." );
			}
		}

		xui::layout::new_line( );
		if ( detail::confirm_reset )
		{
			if ( xui::button( "confirm reset", btn_w, btn_h ) )
			{
				detail::reset_defaults( );
				detail::confirm_reset = false;
				detail::show_status( "Defaults applied. Save to keep them in a profile." );
			}
		}
		else if ( xui::button( "reset defaults", btn_w, btn_h ) )
		{
			detail::confirm_reset = true;
			detail::confirm_delete = false;
			detail::confirm_timer = 0.0f;
			detail::show_status( "Click confirm reset within five seconds." );
		}

		xui::layout::same_line( );
		if ( xui::button( "import code", btn_w, btn_h ) )
		{
			const auto clip = detail::paste_from_clipboard( );
			if ( clip.empty( ) ) detail::show_status( "Clipboard is empty or unavailable.", true );
			else
			{
				const auto result = config::import_auto( clip );
				if ( !result.success ) detail::show_status( "Invalid or damaged share code.", true );
				else
				{
					settings::finalize_binds( );
					std::string candidate = detail::trim_name( result.name );
					if ( candidate.size( ) > 48 || detail::utf8_to_wide( candidate ).empty( ) )
					{
						SYSTEMTIME st{};
						GetLocalTime( &st );
						char buf[ 64 ]{};
						std::snprintf( buf, sizeof( buf ), "import_%04d%02d%02d_%02d%02d%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );
						candidate = buf;
					}
					const auto base_name = candidate;
					std::wstring wide_name;
					for ( auto suffix = 1; suffix <= 100; ++suffix )
					{
						wide_name = detail::utf8_to_wide( suffix == 1 ? base_name : base_name + "_" + std::to_string( suffix ) );
						if ( !detail::name_exists( wide_name ) ) break;
					}
					if ( detail::name_exists( wide_name ) || !config::registry::save( wide_name ) )
						detail::show_status( "Code applied, but its profile could not be saved.", true );
					else
					{
						detail::selected_config_name = wide_name;
                        ui::profile_saved(wide_name);
						detail::search_buf.clear( );
						detail::needs_refresh = true;
						detail::show_status( "Code imported and saved as a new profile." );
					}
				}
			}
		}

		xui::layout::same_line( );
		if ( xui::button( "copy code", btn_w, btn_h ) )
		{
			const auto code = config::export_share_words( has_selection ? detail::selected_name( ) : std::string{ "current" } );
			const auto copied = !code.empty( ) && detail::copy_to_clipboard( code );
			detail::show_status( copied ? "Share code copied to clipboard." : "Could not copy the share code.", !copied );
		}

		xui::layout::same_line( );
		if ( xui::button( "refresh list", btn_w, btn_h ) )
		{
			detail::needs_refresh = true;
			detail::show_status( "Profile list refreshed." );
		}

		xui::layout::new_line( );
		if ( !detail::status_message.empty( ) )
			xui::text( detail::status_message,
				detail::status_error ? xdraw::color{ 255, 128, 128, 255 } : tokens::col_text_dim );
		xui::end_child( );
	}

} // namespace rendering

namespace ui {
void poll_profile_state(bool force) {
    using namespace rendering::detail;
    const auto now=std::chrono::steady_clock::now();
    if(!force&&now<profile_poll_at)return;
    profile_poll_at=now+std::chrono::milliseconds(250);
    const auto fingerprint=profile_fingerprint();
    if(!baseline_ready) {saved_fingerprint=fingerprint;baseline_ready=true;}
    dirty=detached||fingerprint!=saved_fingerprint;
}
std::string_view active_profile() {return rendering::detail::active_profile_label;}
bool profile_dirty() {return rendering::detail::dirty;}
void profile_saved(std::wstring_view name) {
    using namespace rendering::detail;
    active_profile_name=name;active_profile_label=wide_to_utf8(active_profile_name);
    saved_fingerprint=profile_fingerprint();baseline_ready=true;dirty=false;detached=false;confirm_load_name.clear();
}
void profile_removed(std::wstring_view name) {
    using namespace rendering::detail;
    if(name==active_profile_name) {active_profile_name.clear();active_profile_label.clear();detached=true;dirty=true;}
}
}
