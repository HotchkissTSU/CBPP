#include "cbpp/cb_alloc.h"

namespace cbpp {
    void Free(void* pMemory) {
		if( pMemory == NULL ) {
			CbThrowError("Attempt to deallocate a nullpointer");
		}

		free(pMemory);
	}
}
