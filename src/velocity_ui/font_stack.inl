	void push_font( font* f )
	{
		detail::g.font_stack.push_back( f ? f : detail::g.primary_font );
	}

	void pop_font( )
	{
		if ( detail::g.font_stack.size( ) > 1 )
		{
			detail::g.font_stack.pop_back( );
		}
	}

	font* current_font( )
	{
		return detail::g.font_stack.empty( ) ? detail::g.primary_font : detail::g.font_stack.back( );
	}

	font* primary_font( )
	{
		return detail::g.primary_font;
	}

