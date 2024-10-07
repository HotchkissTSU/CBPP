#ifndef CBPP_FSYSTEM_H
#define CBPP_FSYSTEM_H

#include <cstdint>

#include "cbpp/ttype/tree.h"

namespace cbpp {
	enum VFS_FILETYPE : uint8_t {
		FTYPE_UNDEF,
		FTYPE_TEXT,
		FTYPE_IMAGE,
		FTYPE_SOUND,
		FTYPE_MAP,

		FTYPE_NUM_TYPES
	};

	struct VFS_File {
		VFS_FILETYPE ftype = FTYPE_UNDEF;
	};
}

#endif