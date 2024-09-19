#include "cbpp/misc.h"
#include "cbpp/itoa.h"
#include <iostream>
#include <iomanip>

namespace cbpp{
	uint32_t GetNumLength(int64_t val){
		uint32_t out = 0;
		
		while(val){
			val = val/10;
			out++;
		}
		
		return out;
	}
	
	std::string IntToString(int64_t val){
		uint32_t numlen = GetNumLength(val);
		char* buff = new char[numlen+1];
		memset(buff, 0, numlen+1);
		
		//itoa(val, buff, 10);
		
		std::string out(buff);
		delete[] buff;
		
		return out;
	}
	
	int64_t StringToInt(std::string val){
		return atoi(val.c_str());
	}
	
	float Clampf(float a, float mx, float mn){
		if(a > mx){
			return mx;
		}else if(a < mn){
			return mn;
		}else{
			return a;
		}
	}
	
	bool VecInBox(Vec2 v, Vec2 p1, Vec2 p2){
		return (v.x >= p1.x && v.x <= p2.x && v.y >= p1.y && v.y <= p2.y);
	}
	
	Vec2 ClampVectorV(Vec2 v, Vec2 min, Vec2 max){
		return Vec2(Clampf(v.x, min.x, max.x), Clampf(v.y, min.y, max.y));
	}
	
	std::vector<std::wstring> SplitString(std::wstring str, wchar_t delim){
		std::wstring tmp;
		std::vector<std::wstring> out;
		
		
		for(std::size_t i = 0; i < str.size()+1; i++){
			if(i == str.size()){
				out.push_back(tmp);
				break;
			}

			if(str[i] != delim){
				tmp = tmp + str[i];
			}else{
				out.push_back(tmp);
				tmp.clear();
			}
		}
		
		return out;
	}
	
	std::vector<std::string> SplitString(std::string str, char delim){
		std::string tmp;
		std::vector<std::string> out;
		
		for(std::size_t i = 0; i < str.size()+1; i++){
			if(i == str.size()){
				out.push_back(tmp);
				break;
			}

			if(str[i] != delim){
				tmp = tmp + str[i];
			}else{
				out.push_back(tmp);
				tmp.clear();
			}
		}
		
		return out;
	}
}
