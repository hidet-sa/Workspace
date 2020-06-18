#include "Common.h"

void DebugLog(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	{
		char tmp[1024];
		vsnprintf(tmp, ARRAY_SIZE(tmp), fmt, ap);
		std::string s = tmp + std::string("\n");
		//int len = strlen(tmp);
		//if (ARRAY_SIZE(tmp) <= len) { len -= 1; }
		//tmp[len] = '\n';
		//std::string 
		OutputDebugStringA(s.c_str());
	}
	va_end(ap);
}
