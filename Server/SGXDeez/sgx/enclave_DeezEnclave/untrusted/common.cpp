/*
 * common.cpp
 *
 *  Created on: Aug 9, 2016
 *      Author: weijiawu
 */

#include "common.h"
#include "Utils.h"


#include <sstream>
#include <stdlib.h>
#include <string.h>

using namespace std;

int StringToInt(std::string input)
{
    std::stringstream s(input);
    int i;
    s >> i;
    return i;
}

std::string IntToString(int input)
{
	std::string output;          // string which will contain the output
	std::ostringstream convert;   // stream used for the conversion

	convert << input;      // insert the textual representation of 'input' in the characters in the stream

	output = convert.str(); // set 'output' to the contents of the stream
	return output;
}

int my_vasprintf(char **strp, const char *fmt, va_list ap)//changed by wwj
{
//    va_list ap1;
	size_t size;
	char *buffer;

	//va_copy(ap1, ap);
//	ap1 = ap;
	size = vsnprintf(NULL, 0, fmt, ap) + 1;
	va_end(ap);
	buffer = (char *)calloc(1, size);

	if (!buffer)
		return -1;

	*strp = buffer;

	return vsnprintf(buffer, size, fmt, ap);
}

std::string S (const char* fmt, ...)
{
	char *ptr = 0;
    va_list args;
    va_start(args, fmt);
    my_vasprintf(&ptr, fmt, args);
    va_end(args);
    std::string s = ptr;
    free(ptr);
    return s;
}

errno_t memcpy_s(
    void *dest,
    size_t numberOfElements,
    const void *src,
    size_t count)
{
    if(numberOfElements<count)
        return -1;
    memcpy(dest, src, count);
    return 0;
}

errno_t memset_s(void *v,size_t smax, int c, size_t n)
{

	if(smax < n)
	{
		return -1;
	}
	else
	{
		memset(v, c, n);
		return 0;
	}
}

