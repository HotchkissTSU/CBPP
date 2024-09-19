#ifndef CBPP_BYTETABLE_H
#define CBPP_BYTETABLE_H

#include <cstdint>
#include <ostream>
#include <iostream>
#include <cstring>

#include "cbpp/vec2.h"
#include "cbpp/cb_alloc.h"
#include "cbpp/error.h"

#define SEGARR_MAX_SEGLEN (1024) //максимальная длина одного сегмента (в байтах) при автоопределении

#define BSegArray SegArray<uint8_t>

using std::size_t;

namespace cbpp {	
	extern "C++" template<typename T> size_t GetSegLength(){
		size_t val = SEGARR_MAX_SEGLEN / sizeof(T);
		if(val <= 0){
			val = 1;
		}
		
		return val;
	}

	extern "C++" template<typename T> struct SegArray { //массив, разбитый по памяти на сегменты
		SegArray(size_t length, size_t _segsize){
			Allocate(length, _segsize);
		}
		
		SegArray(SegArray& other){
			Set(other);
		}
		
		SegArray(size_t length){
			size_t _segsize;
			
			if(sizeof(T) <= SEGARR_MAX_SEGLEN){
				_segsize = GetSegLength<T>(); //автоматически выдумываем длину сегмента
			}else{
				_segsize = 1;
			}
			
			Allocate(length, _segsize);
		}
		
		SegArray(T* arr, size_t length){
			Set(arr, length);
		}
		
		SegArray(){allocated = false;}
		
		void Set(const T* arr, size_t length) {
			if(length <= 0) {
				CbThrowError("SegArray length cannot be less or equal than zero");
			}
			
			Allocate(length, GetSegLength<T>());
			
			for(size_t i = 0; i < length; i++){
				At(i) = arr[i];
			}
		}
		
		void Set(SegArray<T>& other) {
			Allocate(other.Length(), other.SegmentSize());
			
			for(size_t i = 0; i < other.Length(); i++){
				At(i) = other.At(i);
			}
		}
		
		void Reset(){
			if(allocated){
				for(size_t i = 0; i < segs; i++){
					memset(segarr[i], 0, segsize*sizeof(T));
				}
			}
		}
		
		T* AsArray(){
			T* out = new T[arrlen];
			for(size_t i = 0; i < arrlen; i++){
				out[i] = At(i);
			}
			
			return out;
		}
		
		T& operator()(size_t seg_id, size_t local_id){ //обращение напрямую по сегменту и лок. индексу
			return segarr[seg_id][local_id];
		}
		
		T& operator[](size_t index){
			return At(index);
		}
		
		T& At(size_t index){
			size_t segid, lindex;
			
			segid =  index / segsize; //индекс сегмента
			lindex = index % segsize; //локальный индекс внутри сегмента
			
			return segarr[segid][lindex];
		}
		
		void RemoveValue(const T& val){
			for(size_t i = 0; i < Length(); i++){
				if(const_cast<T&>(val) == At(i)){
					memset(At(i), 0, sizeof(T));
				}
			}
		}
		
		size_t Length(){
			if(allocated){
				return arrlen;
			}
			return 0;
		}
		
		size_t FullLength(){
			return segs*segsize;
		}
		
		size_t SegmentSize(){
			return segsize;
		}
		
		size_t Segments(){
			return segs;
		}
		
		template<typename T2> void PrintVerbose(int32_t row = -1, uint8_t width = 3){
			if(allocated){
				std::cout<<"SegArray with "<<segs<<" segments ("<<segsize<<" units / "<<segsize*sizeof(T)<<" bytes each)\n";
				std::cout<<segsize*segs*sizeof(T)<<" bytes are used at all\n";
				
				for(size_t i = 0; i < segs; i++){
					std::cout<<"SEG "<<i<<std::endl;
					
					int32_t x = 0;
					
					for(size_t j = 0; j < segsize; j++){
						std::cout.width(width);
						std::cout<<(T2)segarr[i][j];
						
						if(j != segsize-1){
							std::cout<<" ";
						}
						
						x++;
						if(row != -1){
							if(x > row-1){
								x = 0;
								std::cout<<std::endl;
							}
						}
					}
					
					std::cout<<"\n\n";
				}
			}else{
				std::cout<<"Not allocated segmented array\n";
			}
		}
		
		template<typename T2> void Print(){
			if(allocated){
				for(size_t i = 0; i < segs; i++){
					for(size_t j = 0; j < segsize; j++){
						
						size_t gindex = i*segsize + j;
						
						if(gindex < arrlen){
							std::cout<<(T2)(segarr[i][j]);
							if(gindex != arrlen-1){
								std::cout<<", ";
							}
						}
					}
				}
				std::cout<<";"<<std::endl;
			}
		}
		
		SegArray& operator=(SegArray& other){
			if(other.allocated){
				Free();
				
				Allocate(other.Length(), other.SegmentSize());
				
				for(size_t i = 0; i < other.Segments()*other.SegmentSize(); i++){
					At(i) = other.At(i);
				}
			}
			return *this;
		}
		
		void AddSegment(){
			if(allocated){
			
				T** tmp = cbpp::Allocate<T*>(segs+1);
				
				for(size_t i = 0; i < segs; i++){
					tmp[i] = cbpp::Allocate<T>(segsize);
					
					for(size_t j = 0; j < segsize; j++){
						tmp[i][j] = segarr[i][j];
					}
				}
				
				tmp[segs] = cbpp::Allocate<T>(segsize);			
				
				for(size_t i = 0; i < segs; i++){
					delete[] segarr[i];
				}
				delete[] segarr;
				
				segarr = tmp;
				segs++;
			}else{
				Allocate(1, GetSegLength<T>() );
			}
		}
		
		void RemoveSegment(size_t seg_id){
			if(!allocated){ return; }
			
			T** tmp = cbpp::Allocate<T*>(segs-1);
			
			size_t k = 0;
			for(size_t i = 0; i < segs; i++){
				if(i != seg_id){
					tmp[k] = cbpp::Allocate<T>(segsize);
					
					for(size_t j = 0; j < segsize; j++){
						tmp[k][j] = segarr[i][j];
					}
					
					k++;
				}
			}
			
			for(size_t i = 0; i < segs; i++){
				delete[] segarr[i];
			}
			
			delete[] segarr;
			segarr = tmp;
			segs--;
		}
		
		void PushBack(const T& value){
			if(allocated){
				if(arrlen >= segsize*segs){
					AddSegment();
				}
				
				At(arrlen) = const_cast<T&>(value);
				arrlen++;
			}else{
				Allocate(1, GetSegLength<T>() );
				At(0) = const_cast<T&>(value);
			}
		}
		
		T& PopBack(){
			if(!allocated){ return zerov; }
			
			if(arrlen > 0){
				size_t local_index = arrlen % segsize;
				arrlen--;
				
				T ret;
				memcpy(&ret, &At(arrlen+1), sizeof(T));
				
				if(local_index == 0){ //текущий сегмент закончился, выпиливаем его
					RemoveSegment(segs - 1);
				}
				
				memset(&At(arrlen+1), 0, sizeof(T));
				return ret;
			}else{
				return zerov;
			}
		}
		
		void Pop(size_t index){
			if(!allocated){ return; }
			
			T zero;
			memset(&zero, 0, sizeof(T));
			
			At(index) = zero;
		}
		
		size_t Emplace(const T& value){
			if(!allocated){ return -1; }
			
			size_t out = -1;
			
			T zero;
			memset(&zero, 0, sizeof(T));
			
			for(size_t i = 0; i < arrlen; i++){ //если находится дырка, кладём элемент туда
				if(At(i) == zero){
					At(i) = const_cast<T&>(value);
					return i;
				}
			}
			
			PushBack(value); //дырки нет, добавляем в конец
			
			return Length() - 1;
		}
		
		void Allocate(size_t length, size_t _segsize){
			if(allocated){
				Free();
			}
			
			arrlen = length;
			
			segsize = _segsize;
			
			segs = length/segsize + 1;
			
			bytes = segs*segsize*sizeof(T);
			
			segarr = cbpp::Allocate<T*>(segs);
			
			for(size_t i = 0; i < segs; i++){
				T* segment = new T[segsize];
				memset(segment, 0, segsize*sizeof(T));
				
				segarr[i] = segment;
			}
			
			allocated = true;
		}
		
		void Free(){
			if(allocated){
				for(size_t i = 0; i < segs; i++){
					T* segment = segarr[i];
					if(segment != nullptr){
						delete[] segment;
					}
				}
				delete[] segarr;
				
				segarr = nullptr;
				bytes = 0;
				segs = 0;
				arrlen = 0;
				
				allocated = false;
			}
		}
		
		~SegArray(){
			Free();
		}
		
		private:
			T** segarr = nullptr;
			size_t segsize = 0, bytes = 0, segs = 0, arrlen = 0;
			bool allocated = false;
			
			T zerov; //резервное значение, возвращаемое в случае неприятных ситуаций
	};
	
	extern "C++" template<typename T> void WriteValueToSegArray(BSegArray *sarr, T value, size_t shift){
		size_t sz = sizeof(T);
		
		uint8_t* buff = new uint8_t[sz];
		memset(buff, 0, sz);
		memcpy(buff, &value, sz);
		
		for(size_t i = 0; i < sz; i++){
			sarr->At(i+shift) = buff[i];
		}
		
		delete[] buff;
	}
	
	extern "C++" template<typename T> T ReadValueFromSegArray(BSegArray *sarr, size_t shift){
		size_t sz = sizeof(T);
		T out;
		
		uint8_t* buff = new uint8_t[sz];
		for(size_t i = 0; i < sz; i++){
			buff[i] = sarr->At(shift+i);
		}
		
		memcpy(&out, buff, sz);
		
		delete[] buff;
		return out;
	}
}

#endif