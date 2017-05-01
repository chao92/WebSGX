#ifndef StringEngine_H
#define StringEngine_H

#include <string>
#include "common.h"
#include "GenericEngine.h"

 
template<typename TStream>
class StringDecompressor: 
	public GenericDecompressor<std::string, TStream> 
{
public:
	StringDecompressor (int blockSize);
	virtual ~StringDecompressor (void);

public:
	virtual void importRecords (uint8_t *in, size_t in_size);
};

template<typename TStream>
StringDecompressor<TStream>::StringDecompressor (int blockSize): 
	GenericDecompressor<std::string, TStream>(blockSize)
{
}

template<typename TStream>
StringDecompressor<TStream>::~StringDecompressor (void) 
{
}

template<typename TStream>
void StringDecompressor<TStream>::importRecords (uint8_t *in, size_t in_size) 
{
	if (in_size == 0) 
		return;
	assert(in_size >= sizeof(size_t));
	
	Array<uint8_t> out;
	size_t s = decompressArray(this->streams.front(), in, out);
	size_t start = 0;

	this->records.resize(0);
	for (size_t i = 0; i < s; i++)
		if (out.data()[i] == 0) {
			this->records.add(std::string((char*)out.data() + start, i - start));
			start = i + 1;
		}	
}

#endif
