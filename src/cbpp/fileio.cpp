#include "cbpp/fileio.h"

#include "cbpp/error.h"
#include <cstring>

namespace cbpp {
	File::File(const char* path, const char* mode) {
		this->Open(path, mode);
	}

	bool File::Open(const char* path, const char* mode) {
		bool out = true;

		strcpy(f_mode, mode);
		io_ptr = fopen(path, f_mode);

		if(io_ptr == NULL) {
			out = false;

			char errtext[512];
			snprintf(errtext, 512, "Unable to open file \"%s\"\n", path);
			CbThrowError(errtext);
		}

		is_open = io_ptr != NULL;

		if( strchr(f_mode, '!') != NULL ) { //using wide text I/O
			fwide(io_ptr, 1);
		}else{
			fwide(io_ptr, -1);
		}

		fseek(io_ptr, 0, SEEK_END);
		fl_length = (uint64_t)ftell(io_ptr);
		fseek(io_ptr, 0, SEEK_SET);

		return out;
	}

	void File::Close() {
		if(is_open) {
			fclose( io_ptr );
		}
	}

	bool File::Write(void* buffer, uint64_t count, uint64_t size) {
		bool out = true;

		if(!is_open) {
			out = false;
			CbThrowError("File not open!");
		}

		out = out && fwrite(buffer, size, count, io_ptr);

		return out;
	}

	bool File::Read(void* buffer, uint64_t count, uint64_t size) {
		bool out = true;

		if(!is_open) {
			out = false;
			CbThrowError("File not open!");
		}

		out = out && fread(buffer, size, count, io_ptr);

		return out;
	}

	void File::AllocateBuffer(uint8_t*& buffer) {
		buffer = new uint8_t[fl_length];
	}

	uint64_t File::Length() {
		if(!is_open) {
			return -1;
		}

		return fl_length;
	}

	void File::SetPos(uint64_t npos) {
		if(is_open) {
			fseek(io_ptr, npos, SEEK_SET);
		}
	}

	uint64_t File::GetPos() {
		return ftell(io_ptr);
	}

	bool File::IsOpen() {
		return is_open;
	}

	File::~File() {
		if(is_open) {
			fclose(io_ptr);
		}
	}
}