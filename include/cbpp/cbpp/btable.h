/* 
Насколько я понял, С++ не умеет в раздельное объявление и инициализацию шаблонных функций и классов,
поэтому пришлось написать весь код здесь.

SegArray - сегментированный по памяти массив, внешне работает как самый обычный.
Использовать для хранения больших массивов данных, память под которые может не выделиться целиком.

ByteTable - структура для хранения массивов любого типа с доступом по ключам.
Используется для передачи данных по сети и для записи/чтения произвольных данных в файлы.

Здесь определены метки для типов данных. Они носят сугубо вспомогательный справочный характер и нужны для
корректного угадывания использованных типов, если мы их не знаем (эта система вообще не применяется в рамках
одной машины, т.к. программист не макака и помнит, что и под каким типом он записал ранее)

Эти метки использует, например, клиент или сервер при расшифровке пакета.
*/

#ifndef CBPP_BYTETABLE_H
#define CBPP_BYTETABLE_H

#include <map>
#include <cstdint>
#include <string>
#include <ostream>
#include <iostream>
#include <stdexcept>
#include <cstring>

#include "cbpp/vec2.h"

#define BT_INT uint32_t //тип, используемый для хранения статических значений в ByteTable
						//лучше вообще никогда его не менять, т.к. никакой информации о нем нигде внутри
						//таблицы нет

#define SEGARR_MAX_SEGLEN 1024 //максимальная длина одного сегмента (в байтах) при автоопределении

using std::string;
using std::size_t;

namespace cbpp{
	enum BTABLE_TYPES : uint8_t { //вспомогательные метки типа
		VOID,					  //можно добавить свои, но важно, чтобы и на клиенте, и на сервере они были одинаковые
		
		INT8,INT16,INT32,INT64,
		
		UINT8,UINT16,UINT32,UINT64,
		
		FLOAT,DOUBLE,
		
		STR,WSTR,
		
		VEC2
	};
	
	
	//записываем любую переменную в виде массива байтов
	extern "C++" template<typename T> void WriteValueToByteArray(T value, uint8_t* barr, BT_INT shift){
		uint8_t* buff_tmp = new uint8_t[sizeof(T)];
		memset(buff_tmp, 0, sizeof(T));
		
		memcpy(buff_tmp, &value, sizeof(T));
		
		for(BT_INT i = 0; i < sizeof(T); i++){
			barr[i+shift] = buff_tmp[i];
		}
		
		delete[] buff_tmp;
	}
	
	//восстанавливаем переменную любого типа по массиву байтов
	extern "C++" template<typename T> T ReadValueFromByteArray(uint8_t* barr, BT_INT shift){
		uint8_t* buff_tmp = new uint8_t[sizeof(T)];
		memset(buff_tmp, 0, sizeof(T));
		
		for(BT_INT i = 0; i < sizeof(T); i++){
			buff_tmp[i] = barr[i+shift];
		}
		
		T out;
		memcpy(&out, buff_tmp, sizeof(T));
		
		return out;
	}
	
	extern "C++" template<typename T> void PrintArray(T* arr, BT_INT size){
		for(BT_INT i = 0; i < size; i++){
			std::cout<<arr[i]<<" ";
		}
		std::cout<<std::endl;
	}
	
	extern "C++" template<typename T> struct SegArray{ //массив, разбитый по памяти на сегменты
		SegArray(size_t length, size_t _segsize){	   //внешне работает как обычный массив
			Allocate(length, _segsize);
		}
		
		SegArray(size_t length){
			size_t _segsize;
			
			if(sizeof(T) <= SEGARR_MAX_SEGLEN){
				_segsize = SEGARR_MAX_SEGLEN/sizeof(T); //автоматически выдумываем длину сегмента
			}else{
				_segsize = 1;
			}
			
			Allocate(length, _segsize);
		}
		
		SegArray(T* arr, size_t length){
			Allocate(length, 16);
			
			for(size_t i = 0; i < length; i++){
				At(i) = arr[i];
			}
		}
		
		T& operator()(size_t seg_id, size_t local_id){ //обращение напрямую по сегменту и лок. индексу
			return segarr[seg_id][local_id];
		}
		
		T& operator[](size_t index){
			return At(index);
		}
		
		T& At(size_t index){
			T out;
			if( allocated && (index < segs*segsize) ){
				size_t segid, lindex;
				
				segid =  index / segsize; //индекс сегмента
				lindex = index % segsize; //локальный индекс внутри сегмента
				
				return segarr[segid][lindex];
			}else{
				throw std::out_of_range("ah no man");
			}
		}
		
		size_t Length(){
			return arrlen*segsize;
		}
		
		size_t FullLength(){
			return segs*segsize;
		}
		
		template<typename T2> void PrintVerbose(int32_t row = -1){
			if(allocated){
				std::cout<<"SegArray with "<<segs<<" segments "<<segsize<<" units ("<<segsize*sizeof(T)<<" bytes) each\n";
				std::cout<<segsize*segs<<" bytes are allocated at all\n";
				
				for(size_t i = 0; i < segs; i++){
					std::cout<<"SEG "<<i<<std::endl;
					
					int32_t x = 0;
					
					for(size_t j = 0; j < segsize; j++){
						std::cout.width(3);
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
		
		SegArray& operator=(const SegArray& other){
			if(other.allocated){
				Clear();
				
				Allocate(other.arrlen, other.segsize);
				
				for(size_t i = 0; i < other.segs*other.segsize; i++){
					At(i) = other.At(i);
				}
			}
		}
		
		void Allocate(size_t length, size_t _segsize){
			if(!allocated){
				arrlen = length;
				
				segsize = _segsize;
				
				segs = length/segsize + 1;
				
				bytes = segs*segsize;
				
				segarr = new T*[segs];
				
				for(size_t i = 0; i < segs; i++){
					T* segment = new T[segsize];
					memset(segment, 0, segsize);
					
					segarr[i] = segment;
				}
				
				allocated = true;
			}
		}
		
		void Clear(){
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
			Clear();
		}
		
		T** segarr = nullptr;
		size_t segsize, bytes, segs, arrlen;
		bool allocated = false;
	};
	
	extern "C++" template<typename T> void WriteValueToSegArray(SegArray<uint8_t> *sarr, T value, size_t shift){
		size_t sz = sizeof(T);
		
		uint8_t* buff = new uint8_t[sz];
		memset(buff, 0, sz);
		memcpy(buff, &value, sz);
		
		for(size_t i = 0; i < sz; i++){
			sarr->At(i+shift) = buff[i];
		}
		
		delete[] buff;
	}
	
	extern "C++" template<typename T> T ReadValueFromSegArray(SegArray<uint8_t> *sarr, size_t shift){
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
	
	extern "C++" struct ByteTable{
		ByteTable(){
			header = string("undef");
		};
		
		bool HasKey(string key){
			bool out = false;
			
			for(auto it = data.begin(); it != data.end(); ++it){
				if(key == it->first){
					out = true;
					break;
				}
			}
			
			return out;
		}
		
		BT_INT GetTypeLength(string fname){
			BT_INT out;
			if(HasKey(fname)){
				uint8_t type_id = data[fname][0];
				
				switch(type_id){
					case VOID:
						out = 0;
					case INT8:
						out = 1;
					case INT16:
						out = 2;
					case INT32:
						out = 4;
					case INT64:
						out = 8;
					case UINT8:
						out = 1;
					case UINT16:
						out = 2;
					case UINT32:
						out = 4;
					case UINT64:
						out = 8;
						
					case FLOAT:
						out = 4;
					case DOUBLE:
						out = 8;
					
					case STR:
						out = 0;
					case WSTR:
						out = 0;
					case VEC2:
						out = sizeof(cbpp::Vec2);
				}
			}
			return out;
		}
		
		template<typename T> void WriteValue(string fname, T* fvalue, BT_INT count, BTABLE_TYPES type_id){
		//[typeid byte][count, 4 bytes][typesize, 4 bytes][data, typesize*count bytes]
			if(HasKey(fname)){
				delete[] data[fname];
			}
			
			size_t sz = sizeof(BT_INT);
			
			BT_INT shift = 1;
			BT_INT full_data_length = 1+sz+sz+count*sizeof(T);
			
			uint8_t* buff = new uint8_t[full_data_length];
			for(BT_INT i = 0; i < full_data_length; i++){
				buff[i] = 0;
			}
			
			buff[0] = type_id; //записываем метку типа
			
			WriteValueToByteArray<BT_INT>(count, buff, shift); //записываем кол-во элементов
			shift += sz;
			
			WriteValueToByteArray<BT_INT>((BT_INT)sizeof(T), buff, shift); //размер типа данных
			shift += sz;
			
			for(BT_INT i = 0; i < count; i++){
				WriteValueToByteArray<T>(fvalue[i], buff, shift);
				shift += sizeof(T);
			}

			data[fname] = buff;
		}
		
		template<typename T1> void WriteValueSingle(string fname, T1 fvalue, BTABLE_TYPES type_id){
			T1* tmp_buff = new T1[1];
			tmp_buff[0] = fvalue;
			
			WriteValue(fname, tmp_buff, 1, type_id);
			
			delete[] tmp_buff;
		}
		
		BT_INT GetValueSize(string fname){
			if(!HasKey(fname)){
				return 0;
			}
			
			BT_INT arr_length = ReadValueFromByteArray<BT_INT>(data[fname], 1);
			BT_INT type_size = ReadValueFromByteArray<BT_INT>(data[fname], 5);
			
			return arr_length*type_size;
		}
		
		template<typename T2> int ReadValue(string fname, T2* arr_target){
			if(!HasKey(fname)){
				return 0;
			}
			
			size_t sz = sizeof(BT_INT);
			
			BT_INT shift = 1;
			BT_INT count = ReadValueFromByteArray<BT_INT>(data[fname], 1);
			shift += sz;

			BT_INT type_size = ReadValueFromByteArray<BT_INT>(data[fname], shift);
			shift += sz;
			
			for(BT_INT i = 0; i < count; i++){
				arr_target[i] = ReadValueFromByteArray<T2>(data[fname], shift);
				shift += sizeof(T2);
			}
			
			return 1;
		}
		
		template<typename T3> T3 ReadValueSingle(string fname){
			T3* tmp_buff = new T3[1];
			ReadValue(fname, tmp_buff);
			
			T3 out = tmp_buff[0];
			delete[] tmp_buff;
			
			return out;
		}
		
		SegArray<uint8_t> Assemble(){ //собираем все поля в одну гигаструктуру
			if(header.length() == 0){
				header = string("undef");
			}
			
			size_t sz = sizeof(BT_INT), shift = 0, db_count = 0;
			
			BT_INT fullsize = 1 + header.length() + sz;
			
			for(auto it = data.begin(); it != data.end(); it++){
				string key = it->first;
				fullsize = fullsize + (1 + key.length()) + (GetCount(key)*GetTypeSize(key) + sz*2 + 1);
				
				db_count++;
			}
			
			SegArray<uint8_t> output(fullsize, 16);//для маленьких типов оптимален маленький размер сегмента
			
			//[header length, 1 byte][header name, N bytes][datablocks count, 4 bytes][datablocks]
			//заголовок общего массива
			
			output[0] = (uint8_t)(header.length());
			shift += 1;
			
			for(size_t i = 0; i < header.length(); i++){
				output[i+shift] = (uint8_t)header[i];
			}
			
			shift += header.length();
			
			WriteValueToSegArray<BT_INT>(&output, (BT_INT)db_count, shift);
			shift += sz;
			
			//[key len, 1 byte][key bytes][datablock] - структура блока данных в общем массиве
			
			for(auto it = data.begin(); it != data.end(); it++){
				uint8_t* dbuff = it->second;
				string key = it->first;
				
				BT_INT dlen = 1 + key.length() + GetCount(key)*GetTypeSize(key) + sz*2 + 1;
				BT_INT dbufflen = GetCount(key)*GetTypeSize(key) + sz*2 + 1;
				
				output[shift] = (uint8_t)key.length(); //длина имени поля
				shift++;
				
				for(BT_INT j = 0; j < key.length(); j++){
					output[j+shift] = (uint8_t)key[j]; //само имя
				}
				shift += key.length();
				
				for(BT_INT j = 0; j < dbufflen; j++){
					output[j+shift] = dbuff[j]; //байты поля
				}
				shift += dbufflen;
			}
			
			return output;
		}
		
		void Disassemble(SegArray<uint8_t> sarr){ //восстанавливаем таблицу по имеющейся гигаструктуре
			Clear();
			
			BT_INT shift = 0, db_count = 0, sz;
			
			sz = sizeof(BT_INT);
			
			int32_t hdr_len = (int32_t)sarr[0];
			shift++;
			
			string header;
			for(int32_t i = 0; i < hdr_len; i++){
				header = header + (char)sarr[i+shift]; //читаем заголовок
			}
			shift += hdr_len;
			
			db_count = ReadValueFromSegArray<BT_INT>(&sarr, shift);
			shift += sz;
			
			//начинаем читать блоки данных
			
			for(BT_INT i = 0; i < db_count; i++){
				int32_t keylen = 0;
				string key;
				
				keylen = (int32_t)sarr[shift]; //длина заголовка
				shift++;
				
				for(int32_t j = 0; j < keylen; j++){ //заголовок
					key += sarr[j+shift];
				}
				shift += keylen;
				
					uint8_t type_id = sarr[shift]; //метка типа данных
					BT_INT dbufflen = 0, count = 0, typesize = 0;
			
					count = ReadValueFromSegArray<BT_INT>(&sarr, shift+1); 
					typesize = ReadValueFromSegArray<BT_INT>(&sarr, shift+1+sz); //инфа о массиве в ячейке
					
					dbufflen = 1+sz+sz+typesize*count;

					uint8_t* dbuff = new uint8_t[dbufflen];
					memset(dbuff, 0, dbufflen);
					
					for(BT_INT j = 0; j < dbufflen; j++){
						dbuff[j] = sarr[j+shift];
					}
					
				shift += dbufflen;
				
				data[key] = dbuff;
			}
		}
		
		BT_INT GetCount(string fname){
			if(!HasKey(fname)){
				return 0;
			}
			
			BT_INT count = ReadValueFromByteArray<BT_INT>(data[fname], 1);
			return count;
		}
		
		BT_INT GetTypeSize(string fname){
			if(!HasKey(fname)){
				return 0;
			}
			
			BT_INT sz = ReadValueFromByteArray<BT_INT>(data[fname], 5);
			return sz;
		}
		
		BT_INT GetFullSize(string fname){
			if(!HasKey(fname)){
				return 0;
			}
			
			size_t sz = sizeof(BT_INT);
			
			return 1+sz+sz+GetTypeSize(fname)*GetCount(fname);
		}
		
		void SetHeader(string hdr){ header = hdr; }
		string GetHeader(){ return header; }
		
		void Clear(){
			for(auto it = data.begin(); it != data.end(); it++){
				delete[] it->second;
			}
			
			data.clear();
		}
		
		private:
			std::map<string, uint8_t*> data;
			string header;
	};
}

#endif