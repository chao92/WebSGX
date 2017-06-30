#ifndef ReferenceFixes_H
#define ReferenceFixes_H

#include <map>
#include <string>
#include <vector>
//#include <mutex>
#include <stdio.h>
#include <string.h>
//#include "../zlib/zlib.h"
//#include <smmintrin.h>

#include "common.h"
#include "Stats.h"
#include "Reference.h"
#include "Engines/Engine.h"
#include "Engines/GenericEngine.h"
#include "Engines/StringEngine.h"
#include "Fields/EditOperation.h"
#include "Streams/GzipStream.h"
//#include "../Streams/BzipStream.h"
#include "../Streams/ACGTStream.h"
#include "EditOperation.h"
#include "../DeezEnclave_t.h" 
#include "SAMComment.h"

/*#ifdef __SSE4_1__
	#define DEEZ_SSE
#else
	#warning "Not using SSE 4.1 optimizations -- performance might be suboptimal"
#endif
	*/

class SequenceDecompressor: public Decompressor {
//	Reference reference;

private:
	std::string chromosome;
	std::string fixed;
	size_t fixedStart, fixedEnd;

public:
	SequenceDecompressor (const std::string &refFile, int bs);
	enum Fields {
		FIXES,
		FIXES_ST,
		REPLACE,
		ENUM_COUNT
	};
public:
	bool hasRecord (void);
	void importRecords (uint8_t *in, size_t in_size);
	void setIndexData (uint8_t *, size_t) {}

public:
	void scanChromosome (const std::string &s, const SAMComment &samComment);
	std::string getChromosome (void) const { return chromosome; }
	char operator[] (size_t pos) const;
	const char getReference(int pos) const { char tmp; ocall_reference_getCharAt(pos, &tmp); return tmp;} //reference.getCharAt(pos); }
};

#endif
