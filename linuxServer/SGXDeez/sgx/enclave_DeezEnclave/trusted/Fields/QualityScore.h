#ifndef QualityScore_H
#define QualityScore_H

#include "common.h"
#include "../Streams/GzipStream.h"
#include "../Streams/SAMCompStream.h"
#include "../Engines/StringEngine.h"
#include "../Streams/ArithmeticOrder2Stream.h"
#include "../Streams/rANSOrder0Stream.h"
#include "../Streams/rANSOrder2Stream.h"

extern char optQuality;
extern char optLossy;
extern bool optNoQual;

const int QualRange = 96;


typedef 
	//ArithmeticOrder2DecompressionStream<QualRange>
	rANSOrder2DecompressionStream<QualRange>
	QualityDecompressionStream;


class QualityScoreDecompressor: 
	public StringDecompressor<QualityDecompressionStream> 
{
	char offset;
	char sought;

public:
	QualityScoreDecompressor (int blockSize);

public:
	std::string getRecord (size_t i, size_t seq_len, int flag);
	void importRecords (uint8_t *in, size_t in_size);
	void setIndexData (uint8_t *in, size_t in_size);
};

#endif
