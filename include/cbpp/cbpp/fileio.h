#ifndef CBPP_FILEIO_H
#define CBPP_FILEIO_H

#include <stdio.h>
#include <cstdint>

#include "cbpp/cbstring.h"

namespace cbpp {
	enum CBPP_FILE_OP : uint8_t {
		FILE_WRITE,
		FILE_READ
	};
	
	class File {
		public:
			File(){};
			File(String path, String mode);
			File(File& other) = delete;
			
			std::size_t Length();
			std::size_t TextLength();
			
			bool IO(uint8_t* buffer, std::size_t to_read, CBPP_FILE_OP opcode);
			bool IO(String& buffer, std::size_t to_read, CBPP_FILE_OP opcode);
			
			std::size_t GetPtr();
			void SetPtr(std::size_t new_ptr);
			void Rewind();
			
			bool Open(String path, String mode);
			bool IsOpen();
			void Close();
			
		private:
			bool is_open = false;
			FILE* io_ptr = NULL;
			std::size_t fl_length = -1;
	};
}

#endif