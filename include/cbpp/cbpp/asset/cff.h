#ifndef CBPP_CFF_H
#define CBPP_CFF_H

#include <cstdint>
#include <map>

namespace cbpp {
	struct CFF_Header {
		char sig[4];
		uint16_t ver;
		uint8_t type;
		uint8_t sorted;
		uint32_t md;
		uint8_t sheet;
	};
	//"*_parse" structs are made for parsing data only, so they have their alignment disabled.
	//You should use their regular counterparts for anything except parsing.
	struct CFF_Header_parse { 
		char sig[4];		  
		uint16_t ver;
		uint8_t type;
		uint8_t sorted;
		uint32_t md;
		uint8_t sheet;
	} __attribute__((packed));

	struct CFF_CharMD {
		uint32_t code;
		float advance, bearingy, bbox_ymax, width;
	};

	struct CFF_CharMD_parse {
		uint32_t code;
		float advance, bearingy, bbox_ymax, width;
	} __attribute__((packed));

	struct CFF_Bitmap {
		char name[33];
		uint32_t res;
		uint8_t channels;
		uint8_t* bitmap;
	} __attribute__((packed));

	struct CFF_Bitmap_parse {
		char name[33];
		uint32_t res;
		uint8_t channels;
		uint8_t* bitmap;
	};

	class FontFile {
		public:
			FontFile() = default;
			FontFile(const char* path);

			bool Load(const char* path);
			bool Save(const char* path);

			CFF_CharMD GetCharInfo( uint32_t charcode );
			const char* GetSheetName( uint8_t sheetid );

		private:
			CFF_Header head;
			CFF_Bitmap* maps = NULL;
			CFF_CharMD* chars_data = NULL;

			uint8_t maps_num = 0;
			uint32_t md_num = 0;

			char* last_error = NULL;

			static bool char_comparator( CFF_CharMD& first, CFF_CharMD& second );
	};
}

#endif
