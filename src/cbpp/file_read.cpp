#include "cbpp/file_read.h"

#include <cstdint>
#include <cstring>
#include <string>

#include "cbpp/misc.h"

namespace cbpp {	
	char* ReadFileText(FILE* stream) {
		if(stream == nullptr){
			return nullptr;
		}
		
		int c;
		uint32_t length = 0;
		while(1){
			c = fgetc(stream);
			
			if(c != EOF){
				length++;
			}else{
				break;
			}
		}
		
		char* buffer = new char[length+1];
		memset(buffer, 0, length+1);
		
		rewind(stream);
		fread(buffer, sizeof(char), length+1, stream);
		rewind(stream);
		
		return buffer;
	}
}