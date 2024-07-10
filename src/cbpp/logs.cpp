#include "cbpp/logs.h"

#include <iostream>

namespace cbpp{	
	Log::Log(std::string name){
		log = std::wofstream(CBPP_LOGPATH + name + std::string(".txt"));
		log_name = name;
		valid = log.is_open();
		
		//log.imbue(std::locale("ru_RU.utf8"));
	}
	
	void Log::Open(){
		log = std::wofstream(CBPP_LOGPATH + log_name + std::string(".txt"));
		valid = log.is_open();
		
		//log.imbue(std::locale("ru_RU.utf8"));
	}
	
	void Log::Close(){
		log.close();
	}
	
	void Log::_basewrite(std::wstring txt){
		if(!log.is_open()){ return; }
		
		time_t curtime;
		time(&curtime);
		
		std::string timemask( std::ctime(&curtime) );
		uint16_t* buff = new uint16_t[timemask.size()];
		
		if(buff == nullptr){ return; }
		
		for(std::size_t i = 0; i < timemask.size()-1; i++){
			buff[i] = (uint16_t)timemask[i];
		}
		
		std::wstring timemask_w((wchar_t*)buff);
		delete[] buff;
		
		std::wstring to_write = std::wstring(L"[") + timemask_w + std::wstring(L"] ") + txt;
		//std::wcout<<to_write<<std::endl;
		log<<to_write.c_str()<<std::endl;
	}
	
	void Log::WriteInfo(std::wstring str){
		std::wstring to_write(L" INFO: ");
		to_write += str;
		_basewrite(to_write);
	}
	
	void Log::WriteWarning(std::wstring str){
		std::wstring to_write(L" WARNING: ");
		to_write += str;
		_basewrite(to_write);
	}
	
	void Log::WriteError(std::wstring str){
		std::wstring to_write(L" ERROR: ");
		to_write += str;
		
		log<<"= ERROR ="<<std::endl<<'\t';
		
		_basewrite(to_write);
		
		log<<"= ERROR ="<<std::endl;
	}
	
	Log::~Log(){
		if(log.is_open()){
			log.close();
		}
	}
}