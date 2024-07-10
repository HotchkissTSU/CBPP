#include "cbpp/path.h"
#include "cbpp/misc.h"

using namespace cbpp;
using namespace std;
using Path;

Path(){}

Path(const wchar_t* str, wchar_t delim){
	Path(wstring(str), delim);
}

Path(wstring& str, wchar_t delim){
	auto words = SplitString(str, delim);
	segs.resize(words.size());
	uint32_t cnt = 0;
	for(uint32_t i = 0; i < words.size(); i++){
		wstring word = words[i];
		
		if(word.length() > 0){
			segs[cnt] = word;
			cnt++;
		}
	}
}

Path(Path& other){
	segs.resize(other.Levels());
	for(uint32_t i = 0; i < other.Levels(); i++){
		At(i) = other.At(i);
	}
}

wstring ToString(){
	wstring out;
	
	for(uint32_t i = 0; i < segs.size(); i++){
		if(i+1 != segs.size()){
			out = out + L'\\';
		}
		out = out + At(i);
	}
	
	return out;
}