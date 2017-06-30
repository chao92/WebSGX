#ifndef MappingQuality_H
#define MappingQuality_H

#include "common.h"
#include "../Streams/GzipStream.h"
#include "../Streams/BzipStream.h"
#include "../Engines/GenericEngine.h"


class MappingQualityDecompressor:
	public GenericDecompressor<uint8_t, GzipDecompressionStream> 
{
public:
	MappingQualityDecompressor (int blockSize): 
		GenericDecompressor<uint8_t, GzipDecompressionStream>(blockSize)
	{
		if (optBzip) {
			streams[0] = new BzipDecompressionStream();
		}
	}

public:
	void importRecords (uint8_t *in, size_t in_size) 
	{
		if (in_size == 0) 
			return;
		assert(in_size >= sizeof(size_t));
		
		Array<uint8_t> out;
		size_t s = decompressArray(streams.front(), in, out);
		assert(s % sizeof(uint8_t) == 0);
		records.resize(0);
		records.add((uint8_t*)out.data(), s / sizeof(uint8_t));
	}
};


#endif
