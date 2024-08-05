#ifndef CBPP_ERROR_H
#define CBPP_ERROR_H

#define CBPP_DEBUG 1 //if in debug mode, exceptions can be raised

#define CbThrowError(err_text) cbpp::_ThrowError(cbpp::Exception( err_text , __FILE__, __FUNCTION__, __LINE__))

#include <stdexcept>

namespace cbpp {
	class Exception : public std::exception {
		public:
			Exception(const char* what, const char* file = NULL, const char* func = NULL, std::size_t line = -1);
			
			virtual const char* what() const noexcept;
			
			~Exception();
			
			char *file, *func, *text, *errtext;
			std::size_t line;
	};
	
	void _ThrowError(Exception exc);
}

#endif