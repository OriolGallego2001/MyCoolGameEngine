#pragma once
#include "Globals.h"

std::vector<char*> logMessages; // Definition of the vector
std::mutex logMutex; // Definition of the mutex


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

	// Store the logged message in the vector (thread-safe)
	std::lock_guard<std::mutex> lock(logMutex);
	logMessages.push_back(_strdup(tmp_string2)); // Duplicate the string and store it

	OutputDebugString(tmp_string2);

}