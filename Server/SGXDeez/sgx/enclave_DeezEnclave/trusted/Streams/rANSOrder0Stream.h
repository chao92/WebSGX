#ifndef rANSOrder0CompressionStream_H
#define rANSOrder0CompressionStream_H

// Inspired by J. Bonfield's Scramble
// http://sourceforge.net/projects/staden/files/io_lib/

#include <vector>
#include "common.h"
#include "Engines/Engine.h"
#include "rANSCoder.h"	

template<int AS>
class rANSOrder0CompressionStream: public DecompressionStream {
public:
	inline uint8_t find(const vector<rANSCoder::Stat> &stat, size_t f) 
	{
		int l = 0, h = AS - 1, m;
		while (l <= h) {
			m = l + (h-l) / 2;
			if (f >= stat[m].start && f < stat[m+1].start)
				return m;
			else if (f < stat[m+1].start) 
				h = m - 1;
			else
				l = m + 1;
		}
		assert(0);
	}

	size_t decompress (uint8_t *source, size_t source_sz, Array<uint8_t> &dest, size_t dest_offset) 
	{	
		size_t num = *((size_t*)source);
		source += sizeof(size_t);
		source_sz -= sizeof(size_t);

		Array<uint8_t> freqData(0, MB);
		auto freqComp = new GzipDecompressionStream();
		uint8_t *prevSource = source;
		decompressArray(freqComp, source, freqData);
		source_sz -= source - prevSource;
		
		auto stats = vector<vector<rANSCoder::Stat>>(1, vector<rANSCoder::Stat>(AS + 1));
		uint8_t fMin = freqData[0];
		uint8_t fMax = freqData[1];
		for (int i = fMin; i <= fMax; i++) 
			stats[0][i].freq = *(uint16_t*)(freqData.data() + 2 + (i - fMin) * sizeof(uint16_t));
		rANSCoder::Stat::setUpFreqs(stats, 1, AS);

		rANSCoder coder;
		coder.initDecode(source + source_sz - 1);
		dest.resize(dest_offset + num);
		for (size_t i = 0; i < num; i++) {
			uint8_t s = find(stats[0], coder.get());
			*(dest.data() + dest_offset + i) = s;
			coder.decode(stats[0][s]);
			coder.renormalize();
		}
		return num;
	}

	void getCurrentState (Array<uint8_t> &ou) 
	{
	}

	void setCurrentState (uint8_t *in, size_t sz) 
	{
	}
};

#define rANSOrder0DecompressionStream rANSOrder0CompressionStream

#endif // rANSOrder0CompressionStream_H
