#ifndef ReadName_H
#define ReadName_H

#include <string>

#include "common.h"
#include "../Streams/GzipStream.h"
#include "../Streams/BzipStream.h"
#include "../Engines/GenericEngine.h"
#include "../Engines/StringEngine.h"
#include "PairedEnd.h"

/*
 * Three types of names DeeZ supports:
 * SRR:
 	* SRR00000000	.	1
 * Solexa
	* EAS139		: 136	: FC706VJ	: 2 	: 2104		: 15343	: 197393 
 * Illumina
	* HWUSI-EAS100R	: 6		: 73		: 941	: 197393	#0/1
*/


const int MAX_TOKEN = 20;

class ReadNameDecompressor: 
	public StringDecompressor<GzipDecompressionStream>  
{
	Array<uint32_t> paired;

public:
	ReadNameDecompressor(int blockSize);
	enum Fields {
		CONTENT,
		INDEX,
		PAIRED,
		ENUM_COUNT
	};

public:
	void importRecords (uint8_t *in, size_t in_size);
	void setIndexData (uint8_t *in, size_t in_size);

public:
	uint32_t getPaired(size_t i);
};

#endif
