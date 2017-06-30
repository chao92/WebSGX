#ifndef rANSOrder2CompressionStream_H
#define rANSOrder2CompressionStream_H

#include <vector>
#include "common.h"
#include "rANSCoder.h"	

template<int AS>
class rANSOrder2CompressionStream: public DecompressionStream {
	vector<vector<rANSCoder::Stat>> stats;
	vector<vector<uint8_t>> lookup;
public:
	rANSOrder2CompressionStream()
	{
	}

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

#define RANS1
#ifdef RANS1
	size_t decompress (uint8_t *source, size_t source_sz, 
			Array<uint8_t> &dest, size_t dest_offset) 
	{	
		size_t num = *((size_t*)source);
		source += sizeof(size_t);
		source_sz -= sizeof(size_t);

		Array<uint8_t> freqData(0, MB);
		auto freqComp = new GzipDecompressionStream();
		uint8_t *prevSource = source;
		decompressArray(freqComp, source, freqData);
		source_sz -= source - prevSource;

		if (!stats.size()) 
			stats.resize(AS, vector<rANSCoder::Stat>(AS + 1));
		for (size_t i = 0; i < freqData.size(); ) {
			uint16_t c = *(uint16_t*)(freqData.data() + i); i += sizeof(uint16_t);
			uint8_t fMin = freqData[i++];
			uint8_t fMax = freqData[i++];
			for (int j = fMin; j <= fMax; j++) 
				stats[c][j].freq = *(uint16_t*)(freqData.data() + i), i += sizeof(uint16_t);
		}
		rANSCoder::Stat::setUpFreqs(stats, AS, AS);
		
		if (!lookup.size()) 
			lookup.resize(AS, vector<uint8_t>(rANSCoder::SCALE));
		for (int i = 0; i < stats.size(); i++) {
			for (int j = 0; j < stats[i].size(); j++) {
				for (int k = stats[i][j].start; k < stats[i][j].start + stats[i][j].freq; k++) {
					lookup[i][k] = j;
				}
			}
		}

		rANSCoder coder;
		coder.initDecode(source + source_sz - 1);
		dest.resize(dest_offset + num);
		uint16_t context = 0, q1 = 0;
		for (size_t i = 0; i < num; i++) {
			uint8_t s = lookup[context][coder.get()];
			// uint8_t s = find(stats[context], coder.get());
			*(dest.data() + dest_offset + i) = s;
			coder.decode(stats[context][s]);
			coder.renormalize();
			context = s;
		}
		return num;
	}

#else
	size_t decompress (uint8_t *source, size_t source_sz, 
			Array<uint8_t> &dest, size_t dest_offset) 
	{	
		size_t num = *((size_t*)source);
		source += sizeof(size_t);
		source_sz -= sizeof(size_t);

		Array<uint8_t> freqData(0, MB);
		auto freqComp = new GzipDecompressionStream();
		uint8_t *prevSource = source;
		decompressArray(freqComp, source, freqData);
		source_sz -= source - prevSource;

		if (!stats.size()) 
			stats.resize(AS * AS, vector<rANSCoder::Stat>(AS + 1));

		for (size_t i = 0; i < freqData.size(); ) {
			uint16_t c = *(uint16_t*)(freqData.data() + i); i += sizeof(uint16_t);
			uint8_t fMin = freqData[i++];
			uint8_t fMax = freqData[i++];
			for (int j = fMin; j <= fMax; j++) 
				stats[c][j].freq = *(uint16_t*)(freqData.data() + i), i += sizeof(uint16_t);
		}
		rANSCoder::Stat::setUpFreqs(stats, AS * AS, AS);
		
		// if (!lookup.size()) 
		// 	lookup.resize(AS * AS, vector<uint8_t>(rANSCoder::SCALE));
		// for (int i = 0; i < stats.size(); i++) {
		// 	for (int j = 0; j < stats[i].size(); j++) {
		// 		for (int k = stats[i][j].start; k < stats[i][j].start + stats[i][j].freq; k++) 
		// 			lookup[i][k] = j;
		// 	}
		// }

		rANSCoder coder;
		coder.initDecode(source + source_sz - 1);
		dest.resize(dest_offset + num);
		uint16_t context = 0, q1 = 0;
		for (size_t i = 0; i < num; i++) {
			//uint8_t s = lookup[context][coder.get()];
			uint8_t s = find(stats[context], coder.get());
			*(dest.data() + dest_offset + i) = s;
			coder.decode(stats[context][s]);
			coder.renormalize();
			context = q1 * AS + s, q1 = s;
		}
		return num;
	}
#endif

	void getCurrentState (Array<uint8_t> &ou) 
	{
	}

	void setCurrentState (uint8_t *in, size_t sz) 
	{
	}
};

#define rANSOrder2DecompressionStream rANSOrder2CompressionStream

#endif // rANSOrder2CompressionStream_H
