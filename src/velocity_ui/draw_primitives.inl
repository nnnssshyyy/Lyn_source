	void draw_list::clear( )
	{
		this->vertices.clear( );
		this->indices.clear( );
		this->commands.clear( );
		this->clip_stack.clear( );
	}

	void draw_list::push_clip( float x, float y, float w, float h )
	{
		RECT r{};
		r.left = static_cast< long >( std::floor( x ) );
		r.top = static_cast< long >( std::floor( y ) );
		r.right = static_cast< long >( std::ceil( x + w ) );
		r.bottom = static_cast< long >( std::ceil( y + h ) );

		if ( !this->clip_stack.empty( ) )
		{
			const auto& p = this->clip_stack.back( );
			r.left = std::max( r.left, p.left );
			r.top = std::max( r.top, p.top );
			r.right = std::min( r.right, p.right );
			r.bottom = std::min( r.bottom, p.bottom );
		}

		this->clip_stack.push_back( r );
	}

	void draw_list::push_clip_absolute( float x, float y, float w, float h )
	{
		RECT r{};
		r.left = static_cast< long >( std::floor( x ) );
		r.top = static_cast< long >( std::floor( y ) );
		r.right = static_cast< long >( std::ceil( x + w ) );
		r.bottom = static_cast< long >( std::ceil( y + h ) );

		this->clip_stack.push_back( r );
	}

	void draw_list::pop_clip( )
	{
		if ( !this->clip_stack.empty( ) )
		{
			this->clip_stack.pop_back( );
		}
	}

	void draw_list::rect_filled( float x, float y, float w, float h, color col )
	{
		if ( w <= 0.0f || h <= 0.0f || col.a == 0 )
		{
			return;
		}

		this->ensure_cmd( nullptr );

		auto a = this->emit_vtx( x, y, 0, 0, col );
		auto b = this->emit_vtx( x + w, y, 1, 0, col );
		auto c = this->emit_vtx( x + w, y + h, 1, 1, col );
		auto d = this->emit_vtx( x, y + h, 0, 1, col );

		this->emit_quad( a, b, c, d );
	}

	void draw_list::rect_filled( float x, float y, float w, float h, color col, corner_radius rounding, bool aa )
	{
		if ( w <= 0.0f || h <= 0.0f || col.a == 0 )
		{
			return;
		}

		if ( rounding.tl <= 0.5f && rounding.tr <= 0.5f && rounding.br <= 0.5f && rounding.bl <= 0.5f )
		{
			this->rect_filled( x, y, w, h, col );
			return;
		}

		std::vector<float> path{};
		this->build_rounded_rect_path( x, y, w, h, rounding, path );
		this->convex_filled( path, col, aa );
	}

	void draw_list::rect_filled_gradient( float x, float y, float w, float h, color tl, color tr, color br, color bl )
	{
		if ( w <= 0.0f || h <= 0.0f )
		{
			return;
		}

		this->ensure_cmd( nullptr );

		const auto a = this->emit_vtx( x, y, 0, 0, tl );
		const auto b = this->emit_vtx( x + w, y, 1, 0, tr );
		const auto c = this->emit_vtx( x + w, y + h, 1, 1, br );
		const auto d = this->emit_vtx( x, y + h, 0, 1, bl );

		this->emit_quad( a, b, c, d );
	}

	void draw_list::rect_filled_gradient( float x, float y, float w, float h, color tl, color tr, color br, color bl, corner_radius rounding, bool aa )
	{
		if ( w <= 0.0f || h <= 0.0f )
		{
			return;
		}

		if ( rounding.tl <= 0.5f && rounding.tr <= 0.5f && rounding.br <= 0.5f && rounding.bl <= 0.5f )
		{
			this->rect_filled_gradient( x, y, w, h, tl, tr, br, bl );
			return;
		}

		std::vector<float> path{};
		this->build_rounded_rect_path( x, y, w, h, rounding, path );

		const auto count = static_cast< int >( path.size( ) ) / 2;
		if ( count < 3 )
		{
			return;
		}

		const auto inv_w = 1.0f / w;
		const auto inv_h = 1.0f / h;

		auto lerp_color = [ & ]( float px, float py ) -> color
			{
				const auto sx = std::clamp( ( px - x ) * inv_w, 0.0f, 1.0f );
				const auto sy = std::clamp( ( py - y ) * inv_h, 0.0f, 1.0f );

				const auto r = static_cast< std::uint8_t >( tl.r * ( 1 - sx ) * ( 1 - sy ) + tr.r * sx * ( 1 - sy ) + bl.r * ( 1 - sx ) * sy + br.r * sx * sy );
				const auto g = static_cast< std::uint8_t >( tl.g * ( 1 - sx ) * ( 1 - sy ) + tr.g * sx * ( 1 - sy ) + bl.g * ( 1 - sx ) * sy + br.g * sx * sy );
				const auto b_ = static_cast< std::uint8_t >( tl.b * ( 1 - sx ) * ( 1 - sy ) + tr.b * sx * ( 1 - sy ) + bl.b * ( 1 - sx ) * sy + br.b * sx * sy );
				const auto a_ = static_cast< std::uint8_t >( tl.a * ( 1 - sx ) * ( 1 - sy ) + tr.a * sx * ( 1 - sy ) + bl.a * ( 1 - sx ) * sy + br.a * sx * sy );

				return color{ r, g, b_, a_ };
			};

		this->ensure_cmd( nullptr );

		if ( !aa )
		{
			std::uint32_t first{};

			for ( auto i = 0; i < count; ++i )
			{
				const auto px = path[ static_cast< std::size_t >( i ) * 2 ];
				const auto py = path[ static_cast< std::size_t >( i ) * 2 + 1 ];
				const auto v = this->emit_vtx( px, py, 0.5f, 0.5f, lerp_color( px, py ) );

				if ( i == 0 )
				{
					first = v;
				}
			}

			for ( auto i = 0; i < count - 2; ++i )
			{
				this->emit_idx( first, first + static_cast< std::uint32_t >( i + 1 ), first + static_cast< std::uint32_t >( i + 2 ) );
			}

			return;
		}

		constexpr auto aa_fringe{ 1.0f };
		constexpr auto aa_half = aa_fringe * 0.5f;

		auto centroid_x{ 0.0f };
		auto centroid_y{ 0.0f };

		for ( auto i = 0; i < count; ++i )
		{
			centroid_x += path[ static_cast< std::size_t >( i ) * 2 ];
			centroid_y += path[ static_cast< std::size_t >( i ) * 2 + 1 ];
		}

		centroid_x /= static_cast< float >( count );
		centroid_y /= static_cast< float >( count );

		std::vector<float> normals( count * 2 );

		for ( auto i = 0; i < count; ++i )
		{
			const auto j = ( i + 1 ) % count;
			const auto dx = path[ static_cast< std::size_t >( j ) * 2 ] - path[ static_cast< std::size_t >( i ) * 2 ];
			const auto dy = path[ static_cast< std::size_t >( j ) * 2 + 1 ] - path[ static_cast< std::size_t >( i ) * 2 + 1 ];
			const auto len = std::sqrt( dx * dx + dy * dy );

			if ( len > 0.0001f )
			{
				normals[ static_cast< std::size_t >( i ) * 2 ] = dy / len;
				normals[ static_cast< std::size_t >( i ) * 2 + 1 ] = -dx / len;
			}
		}

		{
			const auto mid_x = ( path[ 0 ] + path[ 2 ] ) * 0.5f;
			const auto mid_y = ( path[ 1 ] + path[ 3 ] ) * 0.5f;
			const auto dot = normals[ 0 ] * ( mid_x - centroid_x ) + normals[ 1 ] * ( mid_y - centroid_y );

			if ( dot < 0.0f )
			{
				for ( auto i = 0; i < count * 2; ++i )
				{
					normals[ i ] = -normals[ i ];
				}
			}
		}

		const auto base = static_cast< std::uint32_t >( this->vertices.size( ) );

		for ( auto i = 0; i < count; ++i )
		{
			const auto prev = ( i + count - 1 ) % count;
			auto nx = ( normals[ static_cast< std::size_t >( prev ) * 2 ] + normals[ static_cast< std::size_t >( i ) * 2 ] ) * 0.5f;
			auto ny = ( normals[ static_cast< std::size_t >( prev ) * 2 + 1 ] + normals[ static_cast< std::size_t >( i ) * 2 + 1 ] ) * 0.5f;
			auto miter{ 1.0f };

			const auto dm = std::sqrt( nx * nx + ny * ny );
			if ( dm > 0.0001f )
			{
				miter = std::min( 1.0f / dm, 4.0f );
				nx /= dm;
				ny /= dm;
			}

			const auto px = path[ static_cast< std::size_t >( i ) * 2 ];
			const auto py = path[ static_cast< std::size_t >( i ) * 2 + 1 ];
			const auto inset = aa_half * miter;
			const auto outset = aa_half * miter;

			const auto col = lerp_color( px, py );

			this->emit_vtx( px - nx * inset, py - ny * inset, 0.5f, 0.5f, col );
			this->emit_vtx( px + nx * outset, py + ny * outset, 0.5f, 0.5f, col.alpha( 0 ) );
		}

		const auto inner0 = base;

		for ( auto i = 0; i < count - 2; ++i )
		{
			this->emit_idx( inner0, base + static_cast< std::uint32_t >( ( i + 1 ) * 2 ), base + static_cast< std::uint32_t >( ( i + 2 ) * 2 ) );
		}

		for ( auto i = 0; i < count; ++i )
		{
			const auto j = ( i + 1 ) % count;
			const auto ci = base + static_cast< std::uint32_t >( i * 2 );
			const auto ni = base + static_cast< std::uint32_t >( j * 2 );

			this->emit_quad( ci, ni, ni + 1, ci + 1 );
		}
	}

	void draw_list::rect_filled_blurred( float x, float y, float w, float h, color tint )
	{
		if ( w <= 0.0f || h <= 0.0f || tint.a == 0 )
		{
			return;
		}

		auto* blur_srv = detail::g.blur_chain[ 0 ].srv.Get( );
		if ( !blur_srv )
		{
			return;
		}

		const auto vp_w = static_cast< float >( detail::g.blur_cached_w );
		const auto vp_h = static_cast< float >( detail::g.blur_cached_h );

		if ( vp_w <= 0.0f || vp_h <= 0.0f )
		{
			return;
		}

		const auto u0 = x / vp_w;
		const auto v0 = y / vp_h;
		const auto u1 = ( x + w ) / vp_w;
		const auto v1 = ( y + h ) / vp_h;

		this->ensure_cmd( blur_srv );

		auto a = this->emit_vtx( x, y, u0, v0, tint );
		auto b = this->emit_vtx( x + w, y, u1, v0, tint );
		auto c = this->emit_vtx( x + w, y + h, u1, v1, tint );
		auto d = this->emit_vtx( x, y + h, u0, v1, tint );
		this->emit_quad( a, b, c, d );
	}

	void draw_list::rect_filled_blurred( float x, float y, float w, float h, corner_radius rounding, color tint, bool aa )
	{
		if ( w <= 0.0f || h <= 0.0f || tint.a == 0 )
		{
			return;
		}

		if ( rounding.tl <= 0.5f && rounding.tr <= 0.5f && rounding.br <= 0.5f && rounding.bl <= 0.5f )
		{
			this->rect_filled_blurred( x, y, w, h, tint );
			return;
		}

		auto blur_srv = detail::g.blur_chain[ 0 ].srv.Get( );
		if ( !blur_srv )
		{
			return;
		}

		const auto vp_w = static_cast< float >( detail::g.blur_cached_w );
		const auto vp_h = static_cast< float >( detail::g.blur_cached_h );

		if ( vp_w <= 0.0f || vp_h <= 0.0f )
		{
			return;
		}

		std::vector<float> path{};
		this->build_rounded_rect_path( x, y, w, h, rounding, path );

		const auto count = static_cast< int >( path.size( ) ) / 2;
		if ( count < 3 )
		{
			return;
		}

		this->ensure_cmd( blur_srv );

		if ( !aa )
		{
			std::uint32_t first{};

			for ( auto i = 0; i < count; ++i )
			{
				const auto px = path[ static_cast< std::size_t >( i ) * 2 ];
				const auto py = path[ static_cast< std::size_t >( i ) * 2 + 1 ];
				const auto vtx = this->emit_vtx( px, py, px / vp_w, py / vp_h, tint );

				if ( i == 0 )
				{
					first = vtx;
				}
			}

			for ( auto i = 0; i < count - 2; ++i )
			{
				this->emit_idx( first, first + static_cast< std::uint32_t >( i + 1 ), first + static_cast< std::uint32_t >( i + 2 ) );
			}

			return;
		}

		constexpr auto aa_fringe{ 1.0f };
		constexpr auto aa_half = aa_fringe * 0.5f;

		const auto transparent = tint.alpha( 0 );

		auto centroid_x{ 0.0f };
		auto centroid_y{ 0.0f };

		for ( auto i = 0; i < count; ++i )
		{
			centroid_x += path[ static_cast< std::size_t >( i ) * 2 ];
			centroid_y += path[ static_cast< std::size_t >( i ) * 2 + 1 ];
		}

		centroid_x /= static_cast< float >( count );
		centroid_y /= static_cast< float >( count );

		const auto base = static_cast< std::uint32_t >( this->vertices.size( ) );
		this->emit_vtx( centroid_x, centroid_y, centroid_x / vp_w, centroid_y / vp_h, tint );

		std::vector<float> normals( count * 2 );

		for ( auto i = 0; i < count; ++i )
		{
			const auto j = ( i + 1 ) % count;
			const auto dx = path[ static_cast< std::size_t >( j ) * 2 ] - path[ static_cast< std::size_t >( i ) * 2 ];
			const auto dy = path[ static_cast< std::size_t >( j ) * 2 + 1 ] - path[ static_cast< std::size_t >( i ) * 2 + 1 ];
			const auto len = std::sqrt( dx * dx + dy * dy );

			if ( len > 0.0001f )
			{
				normals[ static_cast< std::size_t >( i ) * 2 ] = -dy / len;
				normals[ static_cast< std::size_t >( i ) * 2 + 1 ] = dx / len;
			}
		}

		{
			const auto mid_x = ( path[ 0 ] + path[ 2 ] ) * 0.5f;
			const auto mid_y = ( path[ 1 ] + path[ 3 ] ) * 0.5f;
			const auto dot = normals[ 0 ] * ( mid_x - centroid_x ) + normals[ 1 ] * ( mid_y - centroid_y );

			if ( dot < 0.0f )
			{
				for ( auto i = 0; i < count * 2; ++i )
				{
					normals[ i ] = -normals[ i ];
				}
			}
		}

		for ( auto i = 0; i < count; ++i )
		{
			const auto prev = ( i + count - 1 ) % count;

			auto nx = ( normals[ static_cast< std::size_t >( prev ) * 2 ] + normals[ static_cast< std::size_t >( i ) * 2 ] ) * 0.5f;
			auto ny = ( normals[ static_cast< std::size_t >( prev ) * 2 + 1 ] + normals[ static_cast< std::size_t >( i ) * 2 + 1 ] ) * 0.5f;
			auto miter{ 1.0f };

			const auto dm = std::sqrt( nx * nx + ny * ny );
			if ( dm > 0.0001f )
			{
				miter = std::min( 1.0f / dm, 4.0f );
				nx /= dm;
				ny /= dm;
			}

			const auto px = path[ static_cast< std::size_t >( i ) * 2 ];
			const auto py = path[ static_cast< std::size_t >( i ) * 2 + 1 ];
			const auto offset = aa_half * miter;

			const auto inner_x = px - nx * offset;
			const auto inner_y = py - ny * offset;
			const auto outer_x = px + nx * offset;
			const auto outer_y = py + ny * offset;

			this->emit_vtx( inner_x, inner_y, inner_x / vp_w, inner_y / vp_h, tint );
			this->emit_vtx( outer_x, outer_y, outer_x / vp_w, outer_y / vp_h, transparent );
		}

		const auto center = base;
		for ( auto i = 0; i < count; ++i )
		{
			const auto next = ( i + 1 ) % count;
			const auto curr_inner = base + 1 + static_cast< std::uint32_t >( i * 2 );
			const auto next_inner = base + 1 + static_cast< std::uint32_t >( next * 2 );

			this->emit_idx( center, curr_inner, next_inner );
		}

		for ( auto i = 0; i < count; ++i )
		{
			const auto next = ( i + 1 ) % count;
			const auto ci = base + 1 + static_cast< std::uint32_t >( i * 2 );
			const auto co = ci + 1;
			const auto ni = base + 1 + static_cast< std::uint32_t >( next * 2 );
			const auto no_ = ni + 1;

			this->emit_quad( ci, ni, no_, co );
		}
	}

	void draw_list::circle_filled( float cx, float cy, float radius, color col, int segments, bool aa )
	{
		if ( radius <= 0.0f || col.a == 0 )
		{
			return;
		}

		if ( segments <= 0 )
		{
			segments = this->auto_segments( radius );
		}

		const auto step = 2.0f * std::numbers::pi_v<float> / static_cast< float >( segments );
		std::vector<float> pts( segments * 2 );

		for ( auto i = 0; i < segments; ++i )
		{
			const auto a = step * static_cast< float >( i );
			pts[ static_cast< std::size_t >( i ) * 2 ] = cx + std::cos( a ) * radius;
			pts[ static_cast< std::size_t >( i ) * 2 + 1 ] = cy + std::sin( a ) * radius;
		}

		this->convex_filled( pts, col, aa );
	}

	void draw_list::triangle_filled( float x0, float y0, float x1, float y1, float x2, float y2, color col, bool aa )
	{
		if ( col.a == 0 )
		{
			return;
		}

		if ( aa )
		{
			const float pts[ ]{ x0, y0, x1, y1, x2, y2 };
			this->convex_filled( std::span<const float>{ pts, 6 }, col, true );
			return;
		}

		this->ensure_cmd( nullptr );

		const auto a = this->emit_vtx( x0, y0, 0, 0, col );
		const auto b = this->emit_vtx( x1, y1, 0, 0, col );
		const auto c = this->emit_vtx( x2, y2, 0, 0, col );

		this->emit_idx( a, b, c );
	}

	void draw_list::convex_filled( std::span<const float> points, color col, bool aa )
	{
		const auto count = static_cast< int >( points.size( ) ) / 2;
		if ( count < 3 || col.a == 0 )
		{
			return;
		}

		this->ensure_cmd( nullptr );

		if ( !aa )
		{
			std::uint32_t first{};

			for ( auto i = 0; i < count; ++i )
			{
				const auto v = this->emit_vtx( points[ i * 2 ], points[ i * 2 + 1 ], 0.5f, 0.5f, col );

				if ( i == 0 )
				{
					first = v;
				}
			}

			for ( auto i = 0; i < count - 2; ++i )
			{
				this->emit_idx( first, first + static_cast< std::uint32_t >( i + 1 ), first + static_cast< std::uint32_t >( i + 2 ) );
			}

			return;
		}

		constexpr auto aa_fringe{ 1.0f };
		constexpr auto aa_half = aa_fringe * 0.5f;

		const auto transparent = col.alpha( 0 );

		auto centroid_x{ 0.0f };
		auto centroid_y{ 0.0f };

		for ( auto i = 0; i < count; ++i )
		{
			centroid_x += points[ static_cast< std::size_t >( i ) * 2 ];
			centroid_y += points[ static_cast< std::size_t >( i ) * 2 + 1 ];
		}

		centroid_x /= static_cast< float >( count );
		centroid_y /= static_cast< float >( count );

		std::vector<float> normals( count * 2 );

		for ( auto i = 0; i < count; ++i )
		{
			const auto j = ( i + 1 ) % count;
			const auto dx = points[ static_cast< std::size_t >( j ) * 2 ] - points[ static_cast< std::size_t >( i ) * 2 ];
			const auto dy = points[ static_cast< std::size_t >( j ) * 2 + 1 ] - points[ static_cast< std::size_t >( i ) * 2 + 1 ];
			const auto len = std::sqrt( dx * dx + dy * dy );

			if ( len > 0.0001f )
			{
				normals[ static_cast< std::size_t >( i ) * 2 ] = dy / len;
				normals[ static_cast< std::size_t >( i ) * 2 + 1 ] = -dx / len;
			}
		}

		{
			const auto mid_x = ( points[ 0 ] + points[ 2 ] ) * 0.5f;
			const auto mid_y = ( points[ 1 ] + points[ 3 ] ) * 0.5f;
			const auto to_mid_x = mid_x - centroid_x;
			const auto to_mid_y = mid_y - centroid_y;
			const auto dot = normals[ 0 ] * to_mid_x + normals[ 1 ] * to_mid_y;

			if ( dot < 0.0f )
			{
				for ( auto i = 0; i < count * 2; ++i )
				{
					normals[ i ] = -normals[ i ];
				}
			}
		}

		const auto base = static_cast< std::uint32_t >( this->vertices.size( ) );

		for ( auto i = 0; i < count; ++i )
		{
			const auto prev = ( i + count - 1 ) % count;

			auto nx = ( normals[ static_cast< std::size_t >( prev ) * 2 ] + normals[ static_cast< std::size_t >( i ) * 2 ] ) * 0.5f;
			auto ny = ( normals[ static_cast< std::size_t >( prev ) * 2 + 1 ] + normals[ static_cast< std::size_t >( i ) * 2 + 1 ] ) * 0.5f;
			auto miter{ 1.0f };

			const auto dm = std::sqrt( nx * nx + ny * ny );
			if ( dm > 0.0001f )
			{
				miter = 1.0f / dm;
				nx /= dm;
				ny /= dm;
			}

			miter = std::min( miter, 4.0f );

			const auto px = points[ static_cast< std::size_t >( i ) * 2 ];
			const auto py = points[ static_cast< std::size_t >( i ) * 2 + 1 ];

			const auto inset = aa_half * miter;
			const auto outset = aa_half * miter;

			this->emit_vtx( px - nx * inset, py - ny * inset, 0.5f, 0.5f, col );
			this->emit_vtx( px + nx * outset, py + ny * outset, 0.5f, 0.5f, transparent );
		}

		const auto inner0 = base;
		for ( auto i = 0; i < count - 2; ++i )
		{
			this->emit_idx( inner0, base + static_cast< std::uint32_t >( ( i + 1 ) * 2 ), base + static_cast< std::uint32_t >( ( i + 2 ) * 2 ) );
		}

		for ( auto i = 0; i < count; ++i )
		{
			const auto j = ( i + 1 ) % count;
			const auto ci = base + static_cast< std::uint32_t >( i * 2 );
			const auto co = ci + 1;
			const auto ni = base + static_cast< std::uint32_t >( j * 2 );
			const auto no_ = ni + 1;

			this->emit_quad( ci, ni, no_, co );
		}
	}

	void draw_list::rect( float x, float y, float w, float h, color col, float thickness, bool aa )
	{
		if ( w <= 0.0f || h <= 0.0f || col.a == 0 || thickness <= 0.0f )
		{
			return;
		}

		const float pts[ ]{ x, y, x + w, y, x + w, y + h, x, y + h };
		this->polyline( std::span<const float>{ pts, 8 }, col, true, thickness, aa );
	}

	void draw_list::rect( float x, float y, float w, float h, color col, corner_radius rounding, float thickness, bool aa )
	{
		if ( w <= 0.0f || h <= 0.0f || col.a == 0 || thickness <= 0.0f )
		{
			return;
		}

		if ( rounding.tl <= 0.5f && rounding.tr <= 0.5f && rounding.br <= 0.5f && rounding.bl <= 0.5f )
		{
			this->rect( x, y, w, h, col, thickness, aa );
			return;
		}

		std::vector<float> path{};
		this->build_rounded_rect_path( x, y, w, h, rounding, path );
		this->polyline( path, col, true, thickness, aa );
	}

	void draw_list::circle( float cx, float cy, float radius, color col, float thickness, int segments, bool aa )
	{
		if ( radius <= 0.0f || col.a == 0 || thickness <= 0.0f )
		{
			return;
		}

		if ( segments <= 0 )
		{
			segments = this->auto_segments( radius );
		}

		const auto step = 2.0f * std::numbers::pi_v<float> / static_cast< float >( segments );
		std::vector<float> pts( segments * 2 );

		for ( auto i = 0; i < segments; ++i )
		{
			const auto a = step * static_cast< float >( i );
			pts[ static_cast< std::size_t >( i ) * 2 ] = cx + std::cos( a ) * radius;
			pts[ static_cast< std::size_t >( i ) * 2 + 1 ] = cy + std::sin( a ) * radius;
		}

		this->polyline( pts, col, true, thickness, aa );
	}

	void draw_list::line( float x0, float y0, float x1, float y1, color col, float thickness, bool aa )
	{
		if ( col.a == 0 || thickness <= 0.0f )
		{
			return;
		}

		const auto dx = x1 - x0;
		const auto dy = y1 - y0;
		const auto len = std::sqrt( dx * dx + dy * dy );

		if ( len < 0.0001f )
		{
			return;
		}

		if ( aa )
		{
			const float pts[ ]{ x0, y0, x1, y1 };
			this->polyline( std::span<const float>{ pts, 4 }, col, false, thickness, true );
			return;
		}

		const auto nx = -dy / len;
		const auto ny = dx / len;
		const auto half = thickness * 0.5f;

		this->ensure_cmd( nullptr );

		const auto a = this->emit_vtx( x0 + nx * half, y0 + ny * half, 0, 0, col );
		const auto b = this->emit_vtx( x1 + nx * half, y1 + ny * half, 1, 0, col );
		const auto c = this->emit_vtx( x1 - nx * half, y1 - ny * half, 1, 1, col );
		const auto d = this->emit_vtx( x0 - nx * half, y0 - ny * half, 0, 1, col );

		this->emit_quad( a, b, c, d );
	}

	void draw_list::polyline( std::span<const float> points, color col, bool closed, float thickness, bool aa )
	{
		const auto count = static_cast< int >( points.size( ) ) / 2;
		if ( count < 2 || col.a == 0 || thickness <= 0.0f )
		{
			return;
		}

		const auto seg_count = closed ? count : ( count - 1 );

		this->ensure_cmd( nullptr );

		std::vector<float> normals( seg_count * 2 );

		for ( auto i = 0; i < seg_count; ++i )
		{
			const auto j = closed ? ( ( i + 1 ) % count ) : ( i + 1 );
			const auto dx = points[ static_cast< std::size_t >( j ) * 2 ] - points[ static_cast< std::size_t >( i ) * 2 ];
			const auto dy = points[ static_cast< std::size_t >( j ) * 2 + 1 ] - points[ static_cast< std::size_t >( i ) * 2 + 1 ];
			const auto len = std::sqrt( dx * dx + dy * dy );

			if ( len > 0.0001f )
			{
				normals[ static_cast< std::size_t >( i ) * 2 ] = -dy / len;
				normals[ static_cast< std::size_t >( i ) * 2 + 1 ] = dx / len;
			}
		}

		auto get_normal = [ & ]( int i, float& out_nx, float& out_ny, float& out_miter )
			{
				if ( closed )
				{
					const auto prev = ( ( i - 1 ) + seg_count ) % seg_count;
					out_nx = ( normals[ static_cast< std::size_t >( prev ) * 2 ] + normals[ ( static_cast< std::size_t >( i ) % seg_count ) * 2 ] ) * 0.5f;
					out_ny = ( normals[ static_cast< std::size_t >( prev ) * 2 + 1 ] + normals[ ( static_cast< std::size_t >( i ) % seg_count ) * 2 + 1 ] ) * 0.5f;
				}
				else if ( i == 0 )
				{
					out_nx = normals[ 0 ];
					out_ny = normals[ 1 ];
				}
				else if ( i == count - 1 )
				{
					out_nx = normals[ ( static_cast< std::size_t >( seg_count ) - 1 ) * 2 ];
					out_ny = normals[ ( static_cast< std::size_t >( seg_count ) - 1 ) * 2 + 1 ];
				}
				else
				{
					out_nx = ( normals[ ( static_cast< std::size_t >( i ) - 1 ) * 2 ] + normals[ static_cast< std::size_t >( i ) * 2 ] ) * 0.5f;
					out_ny = ( normals[ ( static_cast< std::size_t >( i ) - 1 ) * 2 + 1 ] + normals[ static_cast< std::size_t >( i ) * 2 + 1 ] ) * 0.5f;
				}

				const auto dm = std::sqrt( out_nx * out_nx + out_ny * out_ny );
				if ( dm > 0.0001f )
				{
					out_nx /= dm;
					out_ny /= dm;
					out_miter = std::min( 1.0f / dm, 4.0f );
				}
				else
				{
					out_miter = 1.0f;
				}
			};

		if ( !aa )
		{
			const auto half = thickness * 0.5f;
			std::uint32_t first_top{};

			for ( auto i = 0; i < count; ++i )
			{
				const auto px = points[ static_cast< std::size_t >( i ) * 2 ];
				const auto py = points[ static_cast< std::size_t >( i ) * 2 + 1 ];

				float nx, ny, miter;
				get_normal( i, nx, ny, miter );

				const auto ext = half * miter;

				auto top = this->emit_vtx( px + nx * ext, py + ny * ext, 0, 0, col );
				this->emit_vtx( px - nx * ext, py - ny * ext, 1, 1, col );

				if ( i == 0 )
				{
					first_top = top;
				}
			}

			for ( auto i = 0; i < seg_count; ++i )
			{
				const auto j = closed ? ( ( i + 1 ) % count ) : ( i + 1 );
				const auto ct = first_top + static_cast< std::uint32_t >( i * 2 );
				const auto cb = ct + 1;
				const auto nt = first_top + static_cast< std::uint32_t >( j * 2 );
				const auto nb = nt + 1;

				this->emit_quad( ct, nt, nb, cb );
			}

			return;
		}

		constexpr auto fringe{ 1.0f };
		const auto half = ( thickness - fringe ) * 0.5f;
		const auto half_stroke = std::max( half, 0.0f );
		const auto half_outer = half_stroke + fringe;
		const auto transparent = col.alpha( 0 );

		const auto base = static_cast< std::uint32_t >( this->vertices.size( ) );

		for ( auto i = 0; i < count; ++i )
		{
			const auto px = points[ static_cast< std::size_t >( i ) * 2 ];
			const auto py = points[ static_cast< std::size_t >( i ) * 2 + 1 ];

			float nx, ny, miter;
			get_normal( i, nx, ny, miter );

			const auto stroke_ext = half_stroke * miter;
			const auto outer_ext = half_outer * miter;

			this->emit_vtx( px + nx * outer_ext, py + ny * outer_ext, 0, 0, transparent );
			this->emit_vtx( px + nx * stroke_ext, py + ny * stroke_ext, 0, 0, col );
			this->emit_vtx( px - nx * stroke_ext, py - ny * stroke_ext, 1, 1, col );
			this->emit_vtx( px - nx * outer_ext, py - ny * outer_ext, 1, 1, transparent );
		}

		for ( auto i = 0; i < seg_count; ++i )
		{
			const auto j = closed ? ( ( i + 1 ) % count ) : ( i + 1 );
			const auto ci = base + static_cast< std::uint32_t >( i * 4 );
			const auto ni = base + static_cast< std::uint32_t >( j * 4 );

			this->emit_quad( ci + 0, ci + 1, ni + 1, ni + 0 );
			this->emit_quad( ci + 1, ci + 2, ni + 2, ni + 1 );
			this->emit_quad( ci + 2, ci + 3, ni + 3, ni + 2 );
		}
	}

	void draw_list::polyline_gradient( std::span<const float> points, std::span<const color> colors, bool closed, float thickness, bool aa )
	{
		const auto count = static_cast< int >( points.size( ) ) / 2;
		if ( count < 2 || thickness <= 0.0f || colors.empty( ) )
		{
			return;
		}

		const auto seg_count = closed ? count : ( count - 1 );

		this->ensure_cmd( nullptr );

		std::vector<float> normals( seg_count * 2 );

		for ( auto i = 0; i < seg_count; ++i )
		{
			const auto j = closed ? ( ( i + 1 ) % count ) : ( i + 1 );
			const auto dx = points[ static_cast< std::size_t >( j ) * 2 ] - points[ static_cast< std::size_t >( i ) * 2 ];
			const auto dy = points[ static_cast< std::size_t >( j ) * 2 + 1 ] - points[ static_cast< std::size_t >( i ) * 2 + 1 ];
			const auto len = std::sqrt( dx * dx + dy * dy );

			if ( len > 0.0001f )
			{
				normals[ static_cast< std::size_t >( i ) * 2 ] = -dy / len;
				normals[ static_cast< std::size_t >( i ) * 2 + 1 ] = dx / len;
			}
		}

		auto get_normal = [ & ]( int i, float& out_nx, float& out_ny, float& out_miter )
			{
				if ( closed )
				{
					const auto prev = ( ( i - 1 ) + seg_count ) % seg_count;
					out_nx = ( normals[ static_cast< std::size_t >( prev ) * 2 ] + normals[ ( static_cast< std::size_t >( i ) % seg_count ) * 2 ] ) * 0.5f;
					out_ny = ( normals[ static_cast< std::size_t >( prev ) * 2 + 1 ] + normals[ ( static_cast< std::size_t >( i ) % seg_count ) * 2 + 1 ] ) * 0.5f;
				}
				else if ( i == 0 )
				{
					out_nx = normals[ 0 ];
					out_ny = normals[ 1 ];
				}
				else if ( i == count - 1 )
				{
					out_nx = normals[ ( static_cast< std::size_t >( seg_count ) - 1 ) * 2 ];
					out_ny = normals[ ( static_cast< std::size_t >( seg_count ) - 1 ) * 2 + 1 ];
				}
				else
				{
					out_nx = ( normals[ ( static_cast< std::size_t >( i ) - 1 ) * 2 ] + normals[ static_cast< std::size_t >( i ) * 2 ] ) * 0.5f;
					out_ny = ( normals[ ( static_cast< std::size_t >( i ) - 1 ) * 2 + 1 ] + normals[ static_cast< std::size_t >( i ) * 2 + 1 ] ) * 0.5f;
				}

				const auto dm = std::sqrt( out_nx * out_nx + out_ny * out_ny );
				if ( dm > 0.0001f )
				{
					out_nx /= dm;
					out_ny /= dm;
					out_miter = std::min( 1.0f / dm, 4.0f );
				}
				else
				{
					out_miter = 1.0f;
				}
			};

		auto get_color = [ & ]( int i ) -> color
			{
				return colors[ std::min( static_cast< std::size_t >( i ), colors.size( ) - 1 ) ];
			};

		if ( !aa )
		{
			const auto half = thickness * 0.5f;
			std::uint32_t first_top{};

			for ( auto i = 0; i < count; ++i )
			{
				const auto px = points[ static_cast< std::size_t >( i ) * 2 ];
				const auto py = points[ static_cast< std::size_t >( i ) * 2 + 1 ];
				const auto c = get_color( i );

				float nx, ny, miter;
				get_normal( i, nx, ny, miter );

				const auto ext = half * miter;

				auto top = this->emit_vtx( px + nx * ext, py + ny * ext, 0, 0, c );
				this->emit_vtx( px - nx * ext, py - ny * ext, 1, 1, c );

				if ( i == 0 )
				{
					first_top = top;
				}
			}

			for ( auto i = 0; i < seg_count; ++i )
			{
				const auto j = closed ? ( ( i + 1 ) % count ) : ( i + 1 );
				const auto ct = first_top + static_cast< std::uint32_t >( i * 2 );
				const auto cb = ct + 1;
				const auto nt = first_top + static_cast< std::uint32_t >( j * 2 );
				const auto nb = nt + 1;

				this->emit_quad( ct, nt, nb, cb );
			}

			return;
		}

		constexpr auto fringe{ 1.0f };
		const auto half = ( thickness - fringe ) * 0.5f;
		const auto half_stroke = std::max( half, 0.0f );
		const auto half_outer = half_stroke + fringe;

		const auto base = static_cast< std::uint32_t >( this->vertices.size( ) );

		for ( auto i = 0; i < count; ++i )
		{
			const auto px = points[ static_cast< std::size_t >( i ) * 2 ];
			const auto py = points[ static_cast< std::size_t >( i ) * 2 + 1 ];
			const auto c = get_color( i );
			const auto transparent = c.alpha( 0 );

			float nx, ny, miter;
			get_normal( i, nx, ny, miter );

			const auto stroke_ext = half_stroke * miter;
			const auto outer_ext = half_outer * miter;

			this->emit_vtx( px + nx * outer_ext, py + ny * outer_ext, 0, 0, transparent );
			this->emit_vtx( px + nx * stroke_ext, py + ny * stroke_ext, 0, 0, c );
			this->emit_vtx( px - nx * stroke_ext, py - ny * stroke_ext, 1, 1, c );
			this->emit_vtx( px - nx * outer_ext, py - ny * outer_ext, 1, 1, transparent );
		}

		for ( auto i = 0; i < seg_count; ++i )
		{
			const auto j = closed ? ( ( i + 1 ) % count ) : ( i + 1 );
			const auto ci = base + static_cast< std::uint32_t >( i * 4 );
			const auto ni = base + static_cast< std::uint32_t >( j * 4 );

			this->emit_quad( ci + 0, ci + 1, ni + 1, ni + 0 );
			this->emit_quad( ci + 1, ci + 2, ni + 2, ni + 1 );
			this->emit_quad( ci + 2, ci + 3, ni + 3, ni + 2 );
		}
	}

	void draw_list::text( float x, float y, std::string_view str, color col, font* f )
	{
		if ( str.empty( ) || col.a == 0 )
		{
			return;
		}

		if ( !f )
		{
			f = current_font( );
		}

        f=interface_font(f);
		if ( !f || !f->atlas_srv )
		{
			return;
		}

		auto cx = std::floor(x*interface_scale)/interface_scale;
		auto cy = std::floor((y+f->ascent/f->density)*interface_scale)/interface_scale;

		auto p = str.data( );
		auto end = p + str.size( );

		while ( p < end )
		{
			const auto cp = detail::decode_utf8( p, end );
			if ( cp == U'\n' )
			{
				cx = std::floor(x*interface_scale)/interface_scale;
				cy += f->line_height/f->density;
				continue;
			}

			if ( cp < 32 )
			{
				continue;
			}

			auto [owner, gl] = f->resolve( cp );

			if ( gl->width > 0.0f && gl->height > 0.0f )
			{
				owner->flush_atlas( );

				const auto inv_w = 1.0f / static_cast< float >( owner->atlas_w );
				const auto inv_h = 1.0f / static_cast< float >( owner->atlas_h );

				this->ensure_cmd( owner->atlas_srv.Get( ) );

				const auto gx = cx + gl->bearing_x/owner->density;
				const auto gy = cy - gl->bearing_y/owner->density;
				const auto gw = gl->width/owner->density;
				const auto gh = gl->height/owner->density;

				const auto u0 = gl->atlas_x * inv_w;
				const auto v0 = gl->atlas_y * inv_h;
				const auto u1 = ( gl->atlas_x + gl->width ) * inv_w;
				const auto v1 = ( gl->atlas_y + gl->height ) * inv_h;

				auto a = this->emit_vtx( gx, gy, u0, v0, col );
				auto b = this->emit_vtx( gx + gw, gy, u1, v0, col );
				auto c2 = this->emit_vtx( gx + gw, gy + gh, u1, v1, col );
				auto d = this->emit_vtx( gx, gy + gh, u0, v1, col );
				this->emit_quad( a, b, c2, d );
			}

			cx += gl->advance/owner->density;
		}
	}

	void draw_list::text( float x, float y, std::string_view str, color col, text_style style, font* f )
	{
		this->text( x, y, str, col, style, color{}, f );
	}

	void draw_list::text( float x, float y, std::string_view str, color col, text_style style, color shadow_col, font* f )
	{
		if ( str.empty( ) || col.a == 0 )
		{
			return;
		}

		const auto default_shadow = color{ 0, 0, 0, static_cast< std::uint8_t >( static_cast< float >( col.a ) * 0.64f ) };
		const auto& effect_col = shadow_col.a > 0 ? shadow_col : default_shadow;

		switch ( style )
		{
		case text_style::shadowed:
			this->text( x + 1.0f, y + 1.0f, str, effect_col, f );
			break;

		case text_style::outlined:
		{
			constexpr float offsets[ ][ 2 ]
			{
				{ -1.0f,  0.0f }, { 1.0f, 0.0f },
				{  0.0f, -1.0f }, { 0.0f, 1.0f }
			};

			for ( const auto& [ox, oy] : offsets )
			{
				this->text( x + ox, y + oy, str, effect_col, f );
			}

			break;
		}

		default:
			break;
		}

		this->text( x, y, str, col, f );
	}

	void draw_list::image( float x, float y, float w, float h, IDirect3DTexture9* tex, color tint )
	{
		this->image_uv( x, y, w, h, tex, 0, 0, 1, 1, tint );
	}

	void draw_list::image( float x, float y, float w, float h, IDirect3DTexture9* tex, corner_radius rounding, color tint, bool aa )
	{
		if ( !tex || w <= 0.0f || h <= 0.0f || tint.a == 0 )
		{
			return;
		}

		if ( rounding.tl <= 0.5f && rounding.tr <= 0.5f && rounding.br <= 0.5f && rounding.bl <= 0.5f )
		{
			this->image( x, y, w, h, tex, tint );
			return;
		}

		std::vector<float> path{};
		this->build_rounded_rect_path( x, y, w, h, rounding, path );

		const auto count = static_cast< int >( path.size( ) ) / 2;
		if ( count < 3 )
		{
			return;
		}

		const auto inv_w = 1.0f / w;
		const auto inv_h = 1.0f / h;

		this->ensure_cmd( tex );

		if ( !aa )
		{
			std::uint32_t first{};

			for ( auto i = 0; i < count; ++i )
			{
				const auto px = path[ static_cast< std::size_t >( i ) * 2 ];
				const auto py = path[ static_cast< std::size_t >( i ) * 2 + 1 ];
				const auto u = std::clamp( ( px - x ) * inv_w, 0.0f, 1.0f );
				const auto v = std::clamp( ( py - y ) * inv_h, 0.0f, 1.0f );
				const auto vtx = this->emit_vtx( px, py, u, v, tint );

				if ( i == 0 )
				{
					first = vtx;
				}
			}

			for ( auto i = 0; i < count - 2; ++i )
			{
				this->emit_idx( first, first + static_cast< std::uint32_t >( i + 1 ), first + static_cast< std::uint32_t >( i + 2 ) );
			}

			return;
		}

		constexpr auto aa_fringe{ 1.0f };
		constexpr auto aa_half = aa_fringe * 0.5f;

		const auto transparent = tint.alpha( 0 );

		auto centroid_x{ 0.0f };
		auto centroid_y{ 0.0f };

		for ( auto i = 0; i < count; ++i )
		{
			centroid_x += path[ static_cast< std::size_t >( i ) * 2 ];
			centroid_y += path[ static_cast< std::size_t >( i ) * 2 + 1 ];
		}

		centroid_x /= static_cast< float >( count );
		centroid_y /= static_cast< float >( count );

		std::vector<float> normals( count * 2 );

		for ( auto i = 0; i < count; ++i )
		{
			const auto j = ( i + 1 ) % count;
			const auto dx = path[ static_cast< std::size_t >( j ) * 2 ] - path[ static_cast< std::size_t >( i ) * 2 ];
			const auto dy = path[ static_cast< std::size_t >( j ) * 2 + 1 ] - path[ static_cast< std::size_t >( i ) * 2 + 1 ];
			const auto len = std::sqrt( dx * dx + dy * dy );

			if ( len > 0.0001f )
			{
				normals[ static_cast< std::size_t >( i ) * 2 ] = dy / len;
				normals[ static_cast< std::size_t >( i ) * 2 + 1 ] = -dx / len;
			}
		}

		{
			const auto mid_x = ( path[ 0 ] + path[ 2 ] ) * 0.5f;
			const auto mid_y = ( path[ 1 ] + path[ 3 ] ) * 0.5f;
			const auto dot = normals[ 0 ] * ( mid_x - centroid_x ) + normals[ 1 ] * ( mid_y - centroid_y );

			if ( dot < 0.0f )
			{
				for ( auto i = 0; i < count * 2; ++i )
				{
					normals[ i ] = -normals[ i ];
				}
			}
		}

		const auto base = static_cast< std::uint32_t >( this->vertices.size( ) );

		for ( auto i = 0; i < count; ++i )
		{
			const auto prev = ( i + count - 1 ) % count;
			auto nx = ( normals[ static_cast< std::size_t >( prev ) * 2 ] + normals[ static_cast< std::size_t >( i ) * 2 ] ) * 0.5f;
			auto ny = ( normals[ static_cast< std::size_t >( prev ) * 2 + 1 ] + normals[ static_cast< std::size_t >( i ) * 2 + 1 ] ) * 0.5f;

			const auto dm = std::sqrt( nx * nx + ny * ny );
			auto miter{ 1.0f };

			if ( dm > 0.0001f )
			{
				miter = std::min( 1.0f / dm, 4.0f );
				nx /= dm;
				ny /= dm;
			}

			const auto px = path[ static_cast< std::size_t >( i ) * 2 ];
			const auto py = path[ static_cast< std::size_t >( i ) * 2 + 1 ];
			const auto inset = aa_half * miter;
			const auto outset = aa_half * miter;

			const auto u = std::clamp( ( px - x ) * inv_w, 0.0f, 1.0f );
			const auto v = std::clamp( ( py - y ) * inv_h, 0.0f, 1.0f );

			this->emit_vtx( px - nx * inset, py - ny * inset, u, v, tint );
			this->emit_vtx( px + nx * outset, py + ny * outset, u, v, transparent );
		}

		const auto inner0 = base;
		for ( auto i = 0; i < count - 2; ++i )
		{
			this->emit_idx( inner0, base + static_cast< std::uint32_t >( ( i + 1 ) * 2 ), base + static_cast< std::uint32_t >( ( i + 2 ) * 2 ) );
		}

		for ( auto i = 0; i < count; ++i )
		{
			const auto j = ( i + 1 ) % count;
			const auto ci = base + static_cast< std::uint32_t >( i * 2 );
			const auto ni = base + static_cast< std::uint32_t >( j * 2 );

			this->emit_quad( ci, ni, ni + 1, ci + 1 );
		}
	}

	void draw_list::image_uv( float x, float y, float w, float h, IDirect3DTexture9* tex, float u0, float v0, float u1, float v1, color tint )
	{
		if ( !tex || w <= 0.0f || h <= 0.0f || tint.a == 0 )
		{
			return;
		}

		this->ensure_cmd( tex );

		const auto a = this->emit_vtx( x, y, u0, v0, tint );
		const auto b = this->emit_vtx( x + w, y, u1, v0, tint );
		const auto c = this->emit_vtx( x + w, y + h, u1, v1, tint );
		const auto d = this->emit_vtx( x, y + h, u0, v1, tint );

		this->emit_quad( a, b, c, d );
	}

	void draw_list::ensure_cmd( IDirect3DTexture9* texture )
	{
		auto tex = texture ? texture : detail::g.white_srv.Get( );
		const auto has_clip = !this->clip_stack.empty( );
		RECT clip{};

		if ( has_clip )
		{
			clip = this->clip_stack.back( );
            clip={static_cast<LONG>(std::floor(clip.left*interface_scale)),static_cast<LONG>(std::floor(clip.top*interface_scale)),
                static_cast<LONG>(std::ceil(clip.right*interface_scale)),static_cast<LONG>(std::ceil(clip.bottom*interface_scale))};
		}

		if ( !this->commands.empty( ) )
		{
			auto& last = this->commands.back( );
			if ( last.texture == tex && last.has_scissor == has_clip )
			{
				if ( !has_clip )
				{
					return;
				}

				if ( last.scissor.left == clip.left && last.scissor.top == clip.top && last.scissor.right == clip.right && last.scissor.bottom == clip.bottom )
				{
					return;
				}
			}
		}

		draw_cmd cmd{};
		cmd.idx_offset = static_cast< std::uint32_t >( this->indices.size( ) );
		cmd.vtx_offset = static_cast< std::uint32_t >( this->vertices.size( ) );
		cmd.texture = tex;
		cmd.has_scissor = has_clip;
		cmd.scissor = clip;
		this->commands.push_back( cmd );
	}

	std::uint32_t draw_list::emit_vtx( float x, float y, float u, float v, color c )
	{
		auto idx = static_cast< std::uint32_t >( this->vertices.size( ) );
		this->vertices.push_back( { { x*interface_scale, y*interface_scale }, { u, v }, c } );
		return idx;
	}

	void draw_list::emit_idx( std::uint32_t a, std::uint32_t b, std::uint32_t c )
	{
		this->indices.push_back( a );
		this->indices.push_back( b );
		this->indices.push_back( c );
		this->commands.back( ).idx_count += 3;
	}

	void draw_list::emit_quad( std::uint32_t a, std::uint32_t b, std::uint32_t c, std::uint32_t d )
	{
		this->emit_idx( a, b, c );
		this->emit_idx( a, c, d );
	}

	int draw_list::auto_segments( float radius ) const
	{
		if ( radius <= 0.0f )
		{
			return 4;
		}

		const auto segs = static_cast< int >( std::ceil( std::numbers::pi_v<float> *2.0f * radius / 1.5f ) );
		return std::clamp( segs, 24, 128 );
	}

	void draw_list::build_rounded_rect_path( float x, float y, float w, float h, corner_radius r, std::vector<float>& path ) const
	{
		path.clear( );

		const auto max_r = std::min( w, h ) * 0.5f;
		r.tl = std::clamp( r.tl, 0.0f, max_r );
		r.tr = std::clamp( r.tr, 0.0f, max_r );
		r.br = std::clamp( r.br, 0.0f, max_r );
		r.bl = std::clamp( r.bl, 0.0f, max_r );

		constexpr auto half_pi = std::numbers::pi_v<float> *0.5f;

		auto arc = [ & ]( float cx, float cy, float radius, float start_angle, int segments )
			{
				if ( radius <= 0.5f )
				{
					if ( path.empty( ) || path[ path.size( ) - 2 ] != cx || path[ path.size( ) - 1 ] != cy )
					{
						path.push_back( cx );
						path.push_back( cy );
					}

					return;
				}

				const auto step = half_pi / static_cast< float >( segments );

				for ( auto i = 0; i < segments; ++i )
				{
					const auto a = start_angle + step * static_cast< float >( i );
					path.push_back( cx + std::cos( a ) * radius );
					path.push_back( cy + std::sin( a ) * radius );
				}
			};

		const auto segs_tl = std::max( 1, this->auto_segments( r.tl ) / 4 );
		arc( x + r.tl, y + r.tl, r.tl, std::numbers::pi_v<float>, segs_tl );

		const auto segs_tr = std::max( 1, this->auto_segments( r.tr ) / 4 );
		arc( x + w - r.tr, y + r.tr, r.tr, std::numbers::pi_v<float> +half_pi, segs_tr );

		const auto segs_br = std::max( 1, this->auto_segments( r.br ) / 4 );
		arc( x + w - r.br, y + h - r.br, r.br, 0.0f, segs_br );

		const auto segs_bl = std::max( 1, this->auto_segments( r.bl ) / 4 );
		arc( x + r.bl, y + h - r.bl, r.bl, half_pi, segs_bl );
	}

