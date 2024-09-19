#ifndef CBPP_LOG_H
#define CBPP_LOG_H

#include <stdio.h>

namespace cbpp {
	static FILE* log_startup = NULL;
	
	FILE*& StartupLog();
}

#endif