#ifndef CBPP_FILEREAD_H
#define CBPP_FILEREAD_H

#include <stdio.h>

namespace cbpp {
	char* ReadFileText(FILE* stream);
	char* GetFileName(const char* path);
}

#endif