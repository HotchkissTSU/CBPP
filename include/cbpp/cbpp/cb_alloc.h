#ifndef CBPP_ALLOCW_H
#define CBPP_ALLOCW_H

/*
	Memory management calls wrappers.
*/

#include <stdlib.h>
#include <string.h>
#include "cbpp/error.h"

namespace cbpp {
	template <typename T> T* Malloc(size_t iCount) noexcept {
		const size_t iMemorySize = sizeof(T) * iCount;
		T* pMemory = (T*) malloc( iMemorySize );
		if(pMemory == 0) {
			CbThrowErrorf("Allocation of size %lu has failed", iMemorySize);
		}

		return pMemory;
	}

	template <typename T> T* Calloc(size_t iCount) noexcept {
		T* pMemory = Malloc<T>(iCount);
		memset(pMemory, 0, sizeof(T) * iCount);
		return pMemory;
	}

	template <typename T> T* Malloc() noexcept {
		return Malloc<T> (1);
	}

	template <typename T> T* Realloc(T* pMemory, size_t iNewSize) noexcept {
		if( pMemory == NULL ) {
			return Malloc<T>(iNewSize);
		}

		const size_t iMemorySize = sizeof(T) * iNewSize;
		T* pTemp = (T*) realloc( pMemory, iMemorySize );
		if(pTemp == NULL) {
			CbThrowErrorf("%lx reallocation to size %lu has failed", pMemory, iMemorySize);
		}

		return pTemp;
	}

	void Free(void* pMemory);
}

#endif
