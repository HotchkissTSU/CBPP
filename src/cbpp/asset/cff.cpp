#include "cbpp/asset/cff.h"

#include <stdio.h>

namespace cbpp {
	void cff_fread(void* dest, std::size_t num, std::size_t size, FILE* file_obj) {
		int result = fread(dest, num, size, file_obj);
		
		if(!result) {
			//log it
		}
	}
	
	CFF::CFF(const char* filename) {
		Open(filename);
	}
	
	void CFF::Open(const char* filename) {
		FILE* inp = fopen(filename, "rb");
		if(inp == nullptr) {
			//log it
		}
		
		//header
		cff_fread(&head, sizeof(head), 1, inp);
		if(head.signature != CFF_HEADER) {
			//log it
			return;
		}
		
		//text
		cff_fread(&text.text_length, sizeof(uint64_t), 1, inp);
		
		if(text.text_length == 0) {
			//log it
			return;
		}
		
		text.text = (uint8_t*)(new char[text.text_length]);
		cff_fread(text.text, text.text_length, 1, inp);
		
		//sheet
		//cff_fread(sheet);
	}
}