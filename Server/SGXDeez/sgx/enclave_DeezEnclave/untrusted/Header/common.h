#ifndef COMMON_H
#define COMMON_H

#include <string>
//#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <cstring>

#include "../trusted/Array.h"


extern bool optStdout;
extern int  optThreads;
extern int  optLogLevel;

extern bool optReadLossy;
extern bool optBzip;

//typedef unsigned long long MESSAGE_LENGTH_TYPE;
typedef unsigned int MESSAGE_LENGTH_TYPE;
#define SEALING_BUFFER_SIZE 1024
#define SEALED_BUFFER_SIZE 1760
#define SECRET_MAC_SIZE 64
#define ENCRYPTED_ADD_SIZE 16
#define TRANSFERBOLCKS		5000

#ifndef _ERRNO_T_DEFINED
#define _ERRNO_T_DEFINED
typedef int errno_t;
#endif

#ifndef __int64
#define __int64 int64_t
#endif

#define ENCRYPTED_ADD_SIZE 16

#define CHROM_UPDATE(f, _chr, _md5, _filename, _len, _loc) {\
	f.chr = _chr;\
	f.md5 = _md5;\
	f.filename = _filename;\
	f.len = _len;\
	f.loc = _loc;}

int StringToInt(std::string input);

std::string IntToString(int input);

void initCache();

inline std::string int2str (int64_t k);

void inttostr (int64_t k, std::string &r);

std::string xtoa (int64_t k);

char getDNAValue (char ch);

void addEncoded (size_t n, Array<uint8_t> &o, uint8_t offset = 0);

ssize_l_t getEncoded (uint8_t *&len, uint8_t offset = 0);

int packInteger(uint64_t num, Array<uint8_t> &o);

uint64_t unpackInteger(int T, Array<uint8_t> &i, size_t &ii) ;

int my_vasprintf(char **strp, const char *fmt, va_list ap);

int Enclave_vasprintf(char **strp, const char *fmt, va_list ap);

std::string S (const char* fmt, ...);

errno_t memcpy_s(void *dest, size_t numberOfElements, const void *src, size_t count);

errno_t memset_s(void *v,size_t smax, int c, size_t n);

class DZException: public std::exception
{
protected:
	char msg[256];

public:
	DZException (void) {}
	DZException (const char *fmt, ...);

	const char *what (void) const throw()
	{
		return msg;
	}
};

class DZSortedException : public DZException
{
public:
	DZSortedException (const char *fmt, ...);
};

#endif
