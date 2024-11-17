#ifndef CBPP_ALLOC_H
#define CBPP_ALLOC_H

#include <stdlib.h>
#include <type_traits> 
#include <stddef.h>
#include <stdexcept>

#include "cbpp/misc.h"
#include "cbpp/error.h"

/*
	Type-safe (probably) wrappers for C memory allocation calls
	Hail to the realloc()!
*/

namespace cbpp {
	template<typename T> struct Pointer {
		size_t Length = 0;
		T* Ptr = nullptr;
	};

	template<typename T> T* Allocate(size_t size) {
		T* out = (T*) malloc(size * sizeof(T));

		if(out == NULL) {
			char buffer[128];
			snprintf(buffer, 128, "Failed to allocate bytes chunk of size %lu", size * sizeof(T));
			PushError(ERROR_MEM, buffer);
			return NULL;
		}

		for(size_t i = 0; i < size; i++) {
			if(std::is_trivially_constructible<T>::value) {
				new(&out[i]) T();
			}else{
				try {
					new(&out[i]) T();
				} catch(...) {
					free(out);
					throw;
				}
			}
		}

		return out;
	}

	template<typename T> void Free(T*& ptr, size_t ln) {
		if(ptr == NULL) { return; }

		if(std::is_destructible<T>::value) {
			for(size_t i = 0; i < ln; i++) {
				ptr[i].~T();
			}
		}

		free(ptr);
		ptr = NULL;
	}

	template<typename T> T* Reallocate(T* ptr, size_t old_size, size_t new_size) {
		if(ptr == NULL) {
			return Allocate<T>(new_size);
		}

		if(std::is_destructible<T>::value) {
			if(old_size > new_size && old_size != -1) {
				for(size_t i = new_size; i < old_size; i++) {
					ptr[i].~T();
				}
			}
		}
		
		T* new_ptr = (T*)realloc(ptr, new_size*sizeof(T));

		if(new_ptr == NULL) {
			char buffer[64];
			snprintf(buffer, 64, "Reallocation from %lu to %lu failed", old_size, new_size);
			PushError(ERROR_MEM, buffer);
			return NULL;
		}

		if(new_size > old_size && old_size != -1) {
			for(size_t i = old_size; i < new_size; i++) {
				new(&new_ptr[i]) T();
			}
		}

		return new_ptr;
	}
}

#endif