#ifndef CBPP_MISC
#define CBPP_MISC

#include "cbpp/vec2.h"

#include <time.h>
#include <cmath>
#include <vector>
#include <cstdint>

#include <string>
#include <cstring>

namespace cbpp{
	extern "C" float Clampf(float a, float mx, float mn);
	extern "C" bool VecInBox(Vec2 v, Vec2 p1, Vec2 p2);
	extern "C" Vec2 ClampVectorV(Vec2 v, Vec2 min, Vec2 max);
	
	extern "C++" template<typename T> T Min(T a, T b){
		if(a < b){ return a; }
		if(a > b){ return b; }
		
		return a;
	}
	
	extern "C++" template<typename T> T Max(T a, T b){
		if(a > b){ return a; }
		if(a < b){ return b; }
		
		return a;
	}
	
	extern "C" uint32_t GetNumLength(int64_t val);

	extern "C" std::string IntToString(int64_t val);
	extern "C" int64_t StringToInt(std::string val);

	std::vector<std::wstring> SplitString(std::wstring str, wchar_t delim);
	std::vector<std::string> SplitString(std::string str, char delim);
}

#endif