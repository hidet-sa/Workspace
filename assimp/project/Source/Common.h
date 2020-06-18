#pragma once

//standard
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cassert>
#include <fstream>
#include <functional>
#include <math.h>

//window
#include <windows.h>

#define ARRAY_SIZE(A) (sizeof(A)/sizeof((A)[0]))

//debug log
void DebugLog(const char *fmt, ...);
#if defined(_DEBUG)
#define Log(format, ...) DebugLog(format, ##__VA_ARGS__);
#else
#define Log(...)	do {} while(0);
#endif
