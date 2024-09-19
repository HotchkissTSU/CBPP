#ifndef CBPP_FSYSTEM_H
#define CBPP_FSYSTEM_H

#include <cstdint>
#include <fstream>

#include "cbpp/ttype/segarr.h"
#include "cbpp/ttype/tree.h"

namespace cbpp {
	enum FSYS_FTYPE : uint16_t {
		FILE_BINARY,
		FILE_TEXT,
		FILE_IMAGE,
		FILE_MODEL,
		FILE_SND,
		FILE_MAP
	};
	
	enum FSYS_NTYPE : uint8_t {
		FSYS_FILE,
		FSYS_DIRECTORY
	};
	
	struct VFile {
		FSYS_FTYPE ftype = FILE_BINARY;
		
		BSegArray data;

		~VFile();
	};
	
	struct FSYS_Node {
		FSYS_NTYPE ntype = FSYS_DIRECTORY;
		uint16_t fname_ln = 0; uint16_t* fname_ptr = nullptr;
		void* data_ptr = nullptr;
		
		~FSYS_Node();
	};
	
	class VFileSystem {
		public:
			VFileSystem();
			VFileSystem(std::ifstream& ifs);
			
		private:
			Tree<FSYS_Node> fsys;
	};
}

#endif