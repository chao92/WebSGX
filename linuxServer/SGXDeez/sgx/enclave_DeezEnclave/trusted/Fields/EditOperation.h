#ifndef EditOperation_H
#define EditOperation_H

#include <string>

#include "common.h"
#include "../Streams/GzipStream.h"
#include "../Streams/BzipStream.h"
#include "../Streams/ACGTStream.h"
#include "../Streams/ArithmeticOrder0Stream.h"
#include "../Engines/GenericEngine.h"
#include "../Engines/StringEngine.h"
#include "Reference.h"

class SequenceDecompressor;

struct EditOperation 
{
	size_t start;
	size_t end;
	std::string seq, MD, op;
	int NM;

	Array<pair<char, int>> ops;

	EditOperation(): NM(-1) {}
	EditOperation(size_t s, const std::string &se, const std::string &op);

	void calculateTags(Reference &reference);
};
//template<>
//size_t sizeInMemory(EditOperation t);

class EditOperationDecompressor: 
	public GenericDecompressor<EditOperation, GzipDecompressionStream>  
{
	const SequenceDecompressor &sequence;
	ACTGStream nucleotides[3];

public:
	EditOperationDecompressor(int blockSize, const SequenceDecompressor &seq);
	enum Fields {
		LOCATION,
		STITCH,
		OPCODES,
		SEQPOS,
		SEQEND,
		XLEN,
		HSLEN,
		LEN,
		OPLEN,
		ACGT,
		ACGT_IS,
		ACGT_U,
		// ACGT_N,
		ENUM_COUNT
	};
public:
	void importRecords (uint8_t *in, size_t in_size);
	void setIndexData (uint8_t *in, size_t in_size);
	EditOperation &getRecord(size_t i);

private:
	EditOperation getEditOperation (size_t loc, vector<uint8_t*> &fields);
};

#endif // EditOperation_H
