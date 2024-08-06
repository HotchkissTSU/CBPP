#include "cbpp/fileio.h"

#include "cbpp/error.h"

namespace cbpp {
	File::File(String filename, String mode) {
		Open(filename, mode);
	}
	
	bool File::Open(String filename, String mode) {
		io_ptr = _wfopen(filename.c_str(), mode.c_str());
		
		if(io_ptr == NULL) {
			char buff[256];
			sprintf(buff, "Unable to open file: \"%s\"", filename.c_str());
			CbThrowError(buff);
		}
		
		is_open = io_ptr != NULL;
		
		fseek(io_ptr, 0, SEEK_END);
		fl_length = ftell(io_ptr);
		rewind(io_ptr);
		
		return is_open;
	}
	
	bool File::IsOpen() {
		return is_open;
	}
	
	std::size_t File::Length() {
		if(!is_open) {
			CbThrowError("File not open");
			return -1;
		}
		
		return fl_length;
	}
	
	bool File::IO(uint8_t* buffer, std::size_t to_read, CBPP_FILE_OP opcode) {
		if(!is_open) {
			CbThrowError("File not open");
			return false;
		}
		
		if(opcode == FILE_READ) {
			std::size_t read_result = fread(buffer, to_read, 1, io_ptr);
			if(read_result != 1) {
				CbThrowError("EOF");
				return false;
			}
		}else{ //FILE_WRITE
			std::size_t write_result = fwrite(buffer, to_read, 1, io_ptr);
			if(write_result != 1) {
				CbThrowError("Unable to write to the file");
				return false;
			}
		}
		
		return true;
	}
	
	bool File::IO(String& buffer, std::size_t to_read, CBPP_FILE_OP opcode) {
		if(!is_open) {
			CbThrowError("File not open");
			return false;
		}
		
		if(opcode == FILE_READ) {
			buffer.clear();
			for(std::size_t i = 0; i < to_read; i++) {
				wchar_t read_wchar = fgetwc(io_ptr);
				if(read_wchar == WEOF) {
					CbThrowWarning("Attempt to read more data than exists in the file");
					return false;
				}
				
				buffer += read_wchar;
			}
		}else{ //FILE_WRITE
			std::size_t write_result = fputws(buffer.c_str(), io_ptr);
			
			if(write_result == EOF) {
				CbThrowError("Failed to write the string");
				return false;
			}
		}
		
		return true;
	}
	
	void File::Rewind() {
		if(!is_open) {
			CbThrowError("File not open");
			return;
		}
		
		rewind(io_ptr);
	}
}