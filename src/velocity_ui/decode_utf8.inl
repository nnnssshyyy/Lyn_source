		static char32_t decode_utf8( const char*& p, const char* end )
		{
			if ( p >= end )
			{
				return 0;
			}

			auto b = static_cast< std::uint8_t >( *p );
			char32_t cp;
			int extra;

			if ( b < 0x80 ) { cp = b; extra = 0; }
			else if ( b < 0xc0 ) { ++p; return 0xfffd; }
			else if ( b < 0xe0 ) { cp = b & 0x1f; extra = 1; }
			else if ( b < 0xf0 ) { cp = b & 0x0f; extra = 2; }
			else if ( b < 0xf8 ) { cp = b & 0x07; extra = 3; }
			else { ++p; return 0xfffd; }

			++p;

			for ( auto i = 0; i < extra; ++i )
			{
				if ( p >= end )
				{
					return 0xfffd;
				}

				auto cont = static_cast< std::uint8_t >( *p );

				if ( ( cont & 0xc0 ) != 0x80 )
				{
					return 0xfffd;
				}

				cp = ( cp << 6 ) | ( cont & 0x3f );
				++p;
			}

			return cp;
		}

