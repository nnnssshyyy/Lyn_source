#pragma once
#include "xui.hpp"
#include "menu_snow.hpp"
namespace rendering {
    class menu
    {
    public:
        void initialize_graphics_before_rpack( );
        void finalize_textures_after_rpack( );

        void draw( );
        void draw_watermark( ) const;
        void shutdown( ) const;

        void toggle( ) { this->m_open = !this->m_open; }
        void set_open(bool open) { this->m_open = open; }
		[[nodiscard]] bool is_open( ) const { return this->m_open; }
		void apply_saved_cursor( );

        enum class tab : int
        {
            ragebot, legitbot, player, world, misc, config, count
        };

    private:
		bool draw_intro( );
        void draw_side_bar( float h );
        void draw_top_bar( float w );
        void draw_theme_swatches( float sb_x, float avatar_y );
        void draw_background_triangles( float wx, float wy, float ww, float wh, float reveal ) const;
        void draw_menu_comets( float wx, float wy, float ww, float wh, float reveal ) const;
        void apply_theme_preset( int preset );
        void sync_theme_style( ) const;
        void draw_search_results( float x, float y, float w, float h );
        void rebuild_search_index( );
        void close_search( );
        void activate_search_result( std::size_t index );

        void draw_ragebot( float group_w ) const;
        void draw_legitbot( float group_w ) const;
        void draw_player( float group_w ) const;
        void draw_world( float group_w ) const;
        void draw_misc( float group_w ) const;
        void draw_config( float group_w );
        void draw_interface_preferences();

        bool m_open{ true };
        bool m_config_modal_open{};
        bool m_config_cloud_refresh_pending{};
        bool m_config_advanced_open{};
        bool m_last_open{ true };
        float m_open_anim{ 0.0f };
        mutable menu_effects::CometField m_comets;
        mutable menu_effects::TriangleSnow m_snow;
        std::uint8_t m_saved_relative_mouse{};
		bool m_has_saved_cursor{};
		int m_saved_cursor_x{};
		int m_saved_cursor_y{};

        float m_x{ 100.0f };
        float m_y{ 100.0f };
        float m_w{ 960.0f };
        float m_h{ 640.0f };
        float m_body_x{};
        float m_body_y{};
        float m_body_w{};
        float m_body_h{};

        int m_tab{};
        int m_subtab{};
        int m_subtab_pill_tab{ -1 };
        float m_subtab_pill_x{ -1.0f };
		float m_intro_elapsed{};
		float m_intro_rpak_ready_at{ -1.0f };
		float m_intro_bar_phase{};
		bool m_intro_base_graphics_ready{};
		bool m_intro_pack_textures_ready{};
		bool m_intro_finished{};
		bool m_search_open{};
		int m_theme_preset{};
		std::string m_search_query{};
		std::vector<std::size_t> m_search_visible_indices{};

		struct search_entry
		{
            const xui::setting* target{};
			std::string name{};
			std::string category{};
			std::string path{};
			std::string name_lower{};
			std::string category_lower{};
			std::string path_lower{};
			int tab{};
			int subtab{};
			int bind_key{};
		};
		std::vector<search_entry> m_search_entries{};

        struct textures
        {
            struct entry
            {
                Microsoft::WRL::ComPtr<IDirect3DTexture9> resource{};
                int width{};
                int height{};
            };

            entry logo{};
            entry user{};
            entry hunter{};
            entry tabs[ static_cast< int >( tab::count ) ]{};
            entry search{};
            entry settings{};
            entry cfg_folder_on{};
            entry cfg_folder_off{};
            entry cfg_cloud_on{};
            entry cfg_cloud_off{};
            entry cfg_plus{};
			entry fih{};
			entry intro_splash{};
			entry konata_peek{};
            xdraw::gif_image gif{};
        } m_textures{};

        static constexpr auto k_max_subtabs{ 6 };
		static constexpr std::array<const char*, static_cast< int >( tab::count )> k_tab_labels{
			"Combat", "Filters", "Players", "Visuals", "Misc", "Profiles"
		};

        struct subtab_info
        {
            const char* names[ k_max_subtabs ]{};
            int count{};
        };

        static constexpr subtab_info k_subtab_defs[ static_cast< int >( tab::count ) ]
        {
            { { "Aimbot", "Infected aim", "Weapons" },                      3 },
            { { "Targets", "Automation" },                                2 },
            { { "General", "Appearance", "Colors" },                    3 },
            { { "World ESP", "Radar / spectators", "Chat spy", "Effects", "HUD" }, 5 },
            { { "Movement", "Camera", "Other", "Protection" },        4 },
            { { "Profiles" },                                           1 }
        };
    };

	class fonts
	{
	public:
		enum class size : std::uint8_t
		{
			petite,
			normal,
			big,
			count
		};

		struct family_t
		{
			std::array<xdraw::font*, static_cast< std::size_t >( size::count )> sizes{ };

			xdraw::font* operator[]( size size ) const { return this->sizes[ static_cast< std::size_t >( size ) ]; }
			xdraw::font*& operator[]( size size ) { return this->sizes[ static_cast< std::size_t >( size ) ]; }
		};

		void initialize( );

		family_t inter_medium{};
		family_t inter_bold{};
		family_t smallest_pixel7{};
        xdraw::font* esp{};
        xdraw::font* menu_body{};
        xdraw::font* menu_heading{};

	private:
		void load_family( family_t& family, std::uint32_t resource_id, const std::array<float, static_cast< std::size_t >( size::count )>& sizes );
	};

inline menu g_menu{};
inline fonts g_fonts{};
}
