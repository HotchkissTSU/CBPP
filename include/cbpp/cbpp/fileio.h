#ifndef CBPP_FILEIO_H
#define CBPP_FILEIO_H

#include <stdio.h>
#include <cstdint>

#include "cbpp/cbstring.h"

namespace cbpp {	
	class File {
		public:
			File(){};
			File(const char* path, const char* mode);
			File(File& other) = delete;

			bool Open(const char* path, const char* mode);

			// Input/Output <count> elements to <buffer>, each <size> bytes long
			bool Write(void* buffer, uint64_t count, uint64_t size = 1);
			bool Read(void* buffer, uint64_t count, uint64_t size = 1);

			void AllocateBuffer(uint8_t*& target);

			uint64_t Length();
			bool IsOpen();

			void SetPos(uint64_t npos);
			uint64_t GetPos();
			
			~File();

		private:
			char f_mode[16];

			bool is_open = false;
			FILE* io_ptr = NULL;
			uint64_t fl_length = -1;
	};
}

#endif