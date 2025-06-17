#include "cbpp/fileio.h"

#include "cbpp/error.h"
#include <string.h>

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

		fseek(io_ptr, 0, SEEK_END);
		fl_length = (uint64_t)ftell(io_ptr);
		fseek(io_ptr, 0, SEEK_SET);

		return out;
	}

	void File::Close() {
		if(is_open) {
			fclose( io_ptr );
			is_open = false;
		}
	}

	size_t File::Write(void* buffer, uint64_t count, uint64_t size) {
		size_t out = -1;

		if(!is_open) {
			out = -1;
			CbThrowError("File not open!");
			return out;
		}

		out = fwrite(buffer, size, count, io_ptr);

		return out;
	}

	size_t File::Read(void* buffer, uint64_t count, uint64_t size) {
		size_t out = -1;

		if(!is_open) {
			out = -1;
			CbThrowError("File not open!");
			return out;
		}
		
		out = fread(buffer, size, count, io_ptr);

		if(out < count) {
			PushError(ERROR_IO, "Unexpected EOF");
		}

		return out;
	}

	char* File::ReadString() {
		char* buffer = new char[fl_length+1]();
		this->Read(buffer, fl_length+1, 1);

		return buffer;
	}

	void File::AllocateBuffer(uint8_t*& buffer) {
		buffer = new uint8_t[fl_length];
	}

	uint64_t File::Length() const {
		if(!is_open) {
			return -1;
		}

		return fl_length;
	}

	uint64_t File::Length(){
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

	uint64_t File::GetPos() const {
		return ftell(io_ptr);
	}

	bool File::IsOpen() const {
		return is_open;
	}

	bool File::IsOpen() {
		return is_open;
	}

	FILE* File::Handle() const noexcept {
		return io_ptr;
	}

	File::~File() {
		if(is_open) {
			fclose(io_ptr);
		}
	}
}

namespace cbpp {
	List<CString> g_aSearchPaths[SPATHS_AMOUNT];

	const char* SearchPathGroupName(SEARCH_PATH iGroupIndex) noexcept {
		switch( iGroupIndex ) {
			case PATH_ALL:     return "PATH_ALL";
			case PATH_LOCALE:  return "PATH_LOCALE";
			case PATH_MAP:     return "PATH_MAP";
			case PATH_SOUND:   return "PATH_SOUND";
			case PATH_TEXTURE: return "PATH_TEXTURE";
			case PATH_SHADER:  return "PATH_SHADER";
            case PATH_FONT:    return "PATH_FONT";
			default: 		   return "PATH_UNKNOWN";
		}
	}
	
	bool MountSearchPath(SEARCH_PATH iGroupIndex, const char* sPath) {
		List<CString>& aGroup = g_aSearchPaths[iGroupIndex];

		bool bCollide = false;
		for(size_t i = 0; i < aGroup.Length(); i++) {
			if( strcmp(aGroup.At(i), sPath) == 0 ) {
				bCollide = true;
				break;
			}
		}

		if(bCollide) {
			char sBuffer[512];
			snprintf(sBuffer, 512, "The search path '%s' does already exist in the group %s", sPath, SearchPathGroupName(iGroupIndex));
			PushError(ERROR_IO, sBuffer);
			return false;
		}

		aGroup.PushBack(sPath);

		return true;
	}

	File* OpenFile(SEARCH_PATH iGroupIndex, const char* sPath, const char* sModes) {
		bool bWriting = (strchr(sModes, 'w') != NULL);
		if(bWriting) {
			return new File(sPath, sModes);
		}

		List<CString>& aGroup = g_aSearchPaths[iGroupIndex];

		char sBuffer[512];
		for(size_t i = 0; i < aGroup.Length(); i++) {
			snprintf(sBuffer, 512, "%s%s", (const char*)aGroup.At(i), sPath);

			FILE* hTestHandle = fopen(sBuffer, "r");
			if(hTestHandle != NULL) {
				/*#ifdef CBPP_DEBUG
				printf("Resolved path for '%s' in group %s: %s\n", sPath, SearchPathGroupName(iGroupIndex), sBuffer);
				#endif*/

				fclose(hTestHandle);
				return new File(sBuffer, sModes);
			}
        }
        
        CbThrowErrorf("Failed to open file '%s' from the group %s", sPath, SearchPathGroupName(iGroupIndex));

		return NULL;
	}

	File* OpenFile(const char* sPath, const char* sModes) {
		File* hOut = NULL;

		for(size_t i = 0; i < SPATHS_AMOUNT; i++) {
			hOut = OpenFile((SEARCH_PATH)i, sPath, sModes);
			if(hOut != NULL) {
				return hOut;
			}
		}

		char sBuffer[128];
		snprintf(sBuffer, 128, "Can`t find file '%s' in any registered search group", sPath);
		PushError(ERROR_IO, sBuffer);

		return NULL;
	}

	size_t GetFileExtension(const char* sPath, char* sBuffer = NULL) noexcept {
		size_t iLen = strlen(sPath);
		size_t iIndex = iLen;

		return 0;
	}
}
