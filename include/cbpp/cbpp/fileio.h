#ifndef CBPP_FILEIO_H
#define CBPP_FILEIO_H

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <vector>

#include "cbpp/cbstring.h"

namespace cbpp {	
	enum SEARCH_PATH : uint8_t {
		PATH_ALL,
		PATH_LOCALE,
		PATH_TEXTURE,
		PATH_MAP,
		PATH_SOUND,

		SPATHS_AMOUNT
	};

	extern std::vector<CString> g_aSearchPaths[];

	class File {
		public:
			File(){};
			File(const char* path, const char* mode);
			File(const File& other) = delete;

			bool Open(const char* path, const char* mode);
			void Close();

			// Output <count> elements to <buffer>, each <size> bytes long
			size_t Write(void* buffer, uint64_t count, uint64_t size = 1);
			size_t Read(void* buffer, uint64_t count, uint64_t size = 1);

			String ReadUTF8();

			char* ReadString();

			void AllocateBuffer(uint8_t*& target);

			uint64_t Length() const;
			uint64_t Length();
			
			bool IsOpen() const;
			bool IsOpen();

			void SetPos(uint64_t npos);
			uint64_t GetPos() const;

			~File();

		private:
			char f_mode[16];

			bool is_open = false;
			FILE* io_ptr = NULL;
			uint64_t fl_length = -1;
	};

	const char* SearchPathGroupName(SEARCH_PATH iGroupIndex) noexcept;

	bool MountSearchPath(SEARCH_PATH iGroupIndex, const char* sPath);

	File* OpenFile(const char* sPath, const char* sModes);
	File* OpenFile(SEARCH_PATH iGroupIndex, const char* sPath, const char* sModes);
}

#endif
