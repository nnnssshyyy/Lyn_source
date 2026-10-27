	font::~font( )
	{
		if ( this->ft_face )
		{
			FT_Done_Face( static_cast< FT_Face >( this->ft_face ) );
		}

		if ( this->ft_library )
		{
			FT_Done_FreeType( static_cast< FT_Library >( this->ft_library ) );
		}
	}

	bool font::has_glyph( char32_t cp ) const
	{
		if ( !this->ft_face )
		{
			return false;
		}

		return FT_Get_Char_Index( static_cast< FT_Face >( this->ft_face ), cp ) != 0;
	}

	const glyph& font::get( char32_t cp )
	{
		const auto it = this->glyph_cache.find( cp );
		if ( it != this->glyph_cache.end( ) )
		{
			return it->second;
		}

		if ( this->rasterize( cp ) )
		{
			return this->glyph_cache[ cp ];
		}

		return this->missing_glyph;
	}

	std::pair<font*, const glyph*> font::resolve( char32_t cp )
	{
		const auto it = this->glyph_cache.find( cp );
		if ( it != this->glyph_cache.end( ) )
		{
			return { this, &it->second };
		}

		if ( this->has_glyph( cp ) )
		{
			if ( this->rasterize( cp ) )
			{
				return { this, &this->glyph_cache[ cp ] };
			}
		}

		if ( this->fallback )
		{
			return this->fallback->resolve( cp );
		}

		return { this, &this->missing_glyph };
	}

	std::pair<float, float> font::measure( std::string_view str )
	{
		auto w{ 0.0f };
		auto h{ 0.0f };
		auto line_w{ 0.0f };

		const char* p = str.data( );
		const char* end = p + str.size( );

		while ( p < end )
		{
			const auto cp = detail::decode_utf8( p, end );
			if ( cp == U'\n' )
			{
				w = std::max( w, line_w );
				h += this->line_height;
				line_w = 0.0f;
				continue;
			}

			if ( cp < 32 )
			{
				continue;
			}

			auto [owner, gl] = this->resolve( cp );
			line_w += gl->advance;
		}

		w = std::max( w, line_w );

		if ( line_w > 0.0f || h == 0.0f )
		{
			h += this->line_height;
		}

		return { std::ceil( w ), h };
	}

	void font::flush_atlas() {
    if (!atlas_dirty || !atlas_tex) return;
    D3DLOCKED_RECT lock{};
    if (FAILED(atlas_tex->LockRect(0,&lock,nullptr,0))) return;
    for (int y=0;y<atlas_h;++y)
        std::memcpy(static_cast<unsigned char*>(lock.pBits)+y*lock.Pitch,
                    atlas_bitmap.data()+static_cast<size_t>(y)*atlas_w*4,static_cast<size_t>(atlas_w)*4);
    atlas_tex->UnlockRect(0);
    atlas_dirty=false;
}

	bool font::rasterize( char32_t cp )
	{
		auto face = static_cast< FT_Face >( this->ft_face );
		if ( !face )
		{
			return false;
		}

		if ( FT_Load_Char( face, cp, FT_LOAD_RENDER | FT_LOAD_TARGET_NORMAL | FT_LOAD_FORCE_AUTOHINT ) != 0 )
		{
			return false;
		}

		auto& slot = face->glyph;
		auto& bmp = slot->bitmap;
		const auto gw = static_cast< int >( bmp.width );
		const auto gh = static_cast< int >( bmp.rows );

		if ( this->pen_x + gw + 1 > this->atlas_w )
		{
			this->pen_x = 1;
			this->pen_y += row_h + 1;
			this->row_h = 0;
		}

		if ( this->pen_y + gh + 1 > this->atlas_h )
		{
			glyph gl{};
			gl.advance = static_cast< float >( slot->advance.x ) / 64.0f;
			this->glyph_cache[ cp ] = gl;
			return true;
		}

		for ( int y = 0; y < gh; ++y )
		{
			for ( int x = 0; x < gw; ++x )
			{
				const auto dst = static_cast< std::size_t >( ( this->pen_y + y ) * this->atlas_w + ( this->pen_x + x ) ) * 4;
				this->atlas_bitmap[ dst + 0 ] = 255;
				this->atlas_bitmap[ dst + 1 ] = 255;
				this->atlas_bitmap[ dst + 2 ] = 255;
				this->atlas_bitmap[ dst + 3 ] = bmp.buffer[ y * bmp.pitch + x ];
			}
		}

		glyph gl{};
		gl.advance = static_cast< float >( slot->advance.x ) / 64.0f;
		gl.bearing_x = static_cast< float >( slot->bitmap_left );
		gl.bearing_y = static_cast< float >( slot->bitmap_top );
		gl.width = static_cast< float >( gw );
		gl.height = static_cast< float >( gh );
		gl.atlas_x = static_cast< float >( this->pen_x );
		gl.atlas_y = static_cast< float >( this->pen_y );

		this->glyph_cache[ cp ] = gl;

		this->pen_x += gw + 1;
		this->row_h = std::max( row_h, gh );
		this->atlas_dirty = true;

		return true;
	}

