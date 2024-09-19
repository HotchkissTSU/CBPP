#ifndef CBPP_ALLOC_H
#define CBPP_ALLOC_H

#include <cstring>

#include "cbpp/misc.h"

namespace cbpp{	
	template<typename T> T* Allocate(std::size_t arrlen){
		if(arrlen <= 0){ return nullptr; }
		
		T* out = new T[arrlen];
		memset(out, 0, arrlen*sizeof(T));
		
		return out;
	}
	
	template<typename T> void ArrayPushBack(T*& arr, std::size_t arrlen, const T& value){
		if(arr == nullptr){
			arr = Allocate<T>(1);
			arr[0] = value;
			return;
		}
		
		T* new_arr = Allocate<T>(arrlen+1);
		memcpy(new_arr, arr, arrlen*sizeof(T));
		new_arr[arrlen] = value;
		
		delete[] arr;
		arr = new_arr;
	}
	
	template<typename T> bool ArrayHasValue(T* arr, std::size_t arrlen, const T& value) {
		for(std::size_t i = 0; i < arrlen; i++){
			if(arr[i] == value){
				return true;
			}
		}
		
		return false;
	}
	
	template<typename T> void ArrayPopBack(T*& arr, std::size_t arrlen) {
		T* new_arr = Allocate<T>(arrlen-1);
		memcpy(new_arr, arr, (arrlen-1)*sizeof(T));
		
		delete[] arr;
		arr = new_arr;
	}
}

#endif