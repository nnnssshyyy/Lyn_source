	font* load_font( std::span<const std::byte> data, float size_px, int atlas_w, int atlas_h )
	{
		FT_Library ft{};
		if ( FT_Init_FreeType( &ft ) != 0 )
		{
			return nullptr;
		}

		FT_Face face{};
		if ( FT_New_Memory_Face( ft, reinterpret_cast< const FT_Byte* >( data.data( ) ), static_cast< FT_Long >( data.size( ) ), 0, &face ) != 0 )
		{
			FT_Done_FreeType( ft );
			return nullptr;
		}

		FT_Size_RequestRec req{};
		req.type = FT_SIZE_REQUEST_TYPE_REAL_DIM;
		req.height = static_cast< FT_Long >( size_px * 64.0f );
		FT_Request_Size( face, &req );

		auto f = std::make_unique<font>( );
		f->size = size_px;
		f->atlas_w = atlas_w;
		f->atlas_h = atlas_h;
		f->ascent = static_cast< float >( FT_MulFix( face->ascender, face->size->metrics.y_scale ) ) / 64.0f;
		f->descent = static_cast< float >( FT_MulFix( face->descender, face->size->metrics.y_scale ) ) / 64.0f;
		f->line_height = static_cast< float >( face->size->metrics.height ) / 64.0f;
		f->ft_library = ft;
		f->ft_face = face;
		f->atlas_bitmap.resize( static_cast< std::size_t >( atlas_w * atlas_h ) * 4, 0 );

		for ( auto i = 0; i < 96; ++i )
		{
			f->rasterize( static_cast< char32_t >( 32 + i ) );
		}

        f->atlas_tex=create_srv_from_rgba(f->atlas_bitmap.data(),atlas_w,atlas_h);
        if (!f->atlas_tex) return nullptr;
        f->atlas_srv=f->atlas_tex;
		f->atlas_dirty = false;

		detail::g.fonts.push_back( std::move( f ) );
		auto result = detail::g.fonts.back( ).get( );

		if ( !detail::g.primary_font )
		{
			detail::g.primary_font = result;
			detail::g.font_stack.push_back( result );
		}

		return result;
	}

