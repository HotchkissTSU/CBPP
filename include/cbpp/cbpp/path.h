#ifndef CBPP_PATH_H
#define CBPP_PATH_H

#include <string>
#include <vector>
#include <cstdint>

namespace cbpp {
	class Path {
		public:
			Path();
			Path(const wchar_t* str, wchar_t delim = L'\\');
			Path(wstring& str, wchar_t delim = L'\\');
			Path(Path& other);
			
			std::wstring& operator[](std::size_t index);
			std::wstring& At(std::size_t index);
			
			Path operator+(Path& other);
			Path operator+(const wchar_t* other);
			Path operator+(std::wstring& other);
			
			std::wstring ToString();
			
			uint32_t Levels();
			uint32_t Length();
			
		private:
			std::vector<std::wstring> segs;
			std::wstring zerov(L"_INVALID");
	};
}

#endif