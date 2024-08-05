#ifndef CBPP_CFF_H
#define CBPP_CFF_H

#include <cstdint>
#include <map>

#define CFF_HEADER (uint32_t)(0b01000011010001100100011000000000)

namespace cbpp {
	struct CFF_Header {
		uint32_t signature;
		uint16_t version;
	};
	
	struct CFF_Text {
		uint64_t text_length;
		uint8_t* text;
	};
	
	struct CFF_Sheet {
		uint64_t sheet_length;
		uint8_t* sheet;
	};
	
	struct CFF_CharInfo {
		uint32_t x, y;
		uint32_t w, h;
		int16_t x_offset, y_offset;
	};
	
	class CFF {
		public:
			CFF(){};
			CFF(const char* filename);
			
			void Open(const char* filename);
			
			bool IsCFF();
			
		private:
			bool is_open = false;
			int32_t texid = -1;
			std::map<uint16_t, CFF_CharInfo> sheetmap;
			
			CFF_Header head;
			CFF_Text text;
			CFF_Sheet sheet;
	};
}

#endif