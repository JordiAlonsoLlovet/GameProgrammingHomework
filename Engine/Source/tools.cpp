#pragma once
#include "Globals.h"
#include <windows.h>
#include <fstream>

char* string_format(const char* format, ...)
{
	static char tmp_string[4096];
	static va_list  ap;

	// Construct the string from variable arguments
	va_start(ap, format);
	vsprintf_s(tmp_string, 4096, format, ap);
	va_end(ap);
	return tmp_string;
}

void log(const char file[], int line, const char* format, ...)
{
	static char tmp_string[4096];
	static char tmp_string2[4096];
	static va_list  ap;

	// Construct the string from variable arguments
	va_start(ap, format);
	vsprintf_s(tmp_string, 4096, format, ap);
	va_end(ap);
	sprintf_s(tmp_string2, 4096, "\n%s(%d) : %s", file, line, tmp_string);
	OutputDebugString(tmp_string2);
	
	std::ofstream out("logs.txt", std::ios_base::app);
	out << tmp_string2;
	out.close();
}

void clearLog() {
	std::ofstream out("logs.txt");
	out << "";
	out.close();
}

