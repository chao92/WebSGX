#ifndef GzipStream_H
#define GzipStream_H

//#include "../zlib/zlib.h"
#include "zlib_local/zlib_local.h"
#include "common.h"
#include "Stream.h"

// Z_RLE strategy inspired by Scramble
// http://sourceforge.net/projects/staden/files/io_lib/

class GzipDecompressionStream: 
	public DecompressionStream {
public:
	virtual size_t decompress (uint8_t *source, size_t source_sz, 
		Array<uint8_t> &dest, size_t dest_offset)
	{
		size_t sz = dest.size() - dest_offset;

		//int c = 0;
		int c = zlib_local::uncompress(dest.data() + dest_offset, (uLongf *)&sz, source, source_sz); // tmp remove

		if (c == Z_OK) 
			return sz;
		else 
			throw DZException("zlib decompression error %d", c);
	}

	virtual void setCurrentState (uint8_t *source, size_t source_sz) 
	{
	}
};

#endif
