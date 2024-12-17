#ifndef CBPP_ALLOC_H
#define CBPP_ALLOC_H

//A lot of memory management info is printed in the log in the memory debug mode
#define CB_MEMDBG 0

#include <stdlib.h>
#include <type_traits> 
#include <stddef.h>
#include <stdexcept>

#include "cbpp/misc.h"
#include "cbpp/error.h"

/*
	Type-safe ( probably :) ) wrappers for C memory allocation calls.
	Hail to the realloc()!
*/

namespace cbpp {
	/*
		Attempt to call object`s destructor.
		Nullifies primitive types.
	*/
	template<typename T> void Destruct(T* object) noexcept {
		if(std::is_destructible<T>::value) {
			object->~T();
		}else {
			memset(object, 0, sizeof(object));
		}
	}

	/*
		Attempt to properly construct an object. Can throw exceptions.
	*/
	template<typename T, typename... Args> void Construct(T* object, Args... Fargs) {
		if(std::is_constructible<T, Args...>::value) {
			new(object) T( std::forward<Args>(Fargs)... );
		}else{
			if(!std::is_trivially_constructible<T>::value) {
				memset(object, 0, sizeof(object));
			}else {
				CbThrowErrorf("Unable to construct object of type '%s'", typeid(T).name());
			}
		}
	}

	/*
		Type-safe free() wrapper. Intended to deallocate buffers of
		complex objects with custom destructors.

		This call NULL-ifies the passed pointer automatically.

		Regular free() can be used on the primitive types` arrays instead
	*/
	template<typename T> void Free(T*& ptr, size_t ln) noexcept {
		if(ptr == NULL) { return; }

		for(size_t i = 0; i < ln; i++) {
			Destruct<T>(&ptr[i]);
		}

		free(ptr);
		ptr = NULL;
	}

	/*
		Type-safe malloc() wrapper.
		Returns NULL upon allocation fail.
	*/
	template<typename T, typename ... Args> T* Allocate(size_t size, Args... Fargs) {
		T* out = (T*) malloc(size * sizeof(T));

		if(out == NULL) {
			char buffer[128];
			snprintf(buffer, 128, "Failed to allocate bytes chunk of size %lu", size * sizeof(T));
			PushError(ERROR_MEM, buffer);
			return NULL;
		}

		for(size_t i = 0; i < size; i++) {
			try {
				new(&out[i]) T(std::forward<Args>(Fargs)...);
			} catch (...) {
				Free<T>(out, size);

				char buffer[128];
				snprintf(buffer, 128, "Construction failure: %x[%lu]", out, i);
				PushError(ERROR_MEM, buffer);
				return NULL;
			}
		}
		
		return out;
	}
	
	/*
		Type-safe realloc() wrapper. Calls destructors upon shrinks and
		default constructors upon up-allocations.

		If NULL is passed as the reallocation target, a new buffer will be created.

		If -1 is passed to the 'old_size' parameter, there will be no resize-type-safety checks.

		If reallocation fails, it will not modify the original pointer and
		will simply return NULL
	*/
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
				try {
					new(&new_ptr[i]) T();
				} catch (...) {
					Free<T>(new_ptr, old_size + i);
					throw;
				}
			}
		}

		return new_ptr;
	}
}

#endif