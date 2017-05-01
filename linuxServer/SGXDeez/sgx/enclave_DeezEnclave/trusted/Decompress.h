#ifndef Decompress_H
#define Decompress_H

#include "common.h"
//#include <io.h>
//#include "FileIO.h"
#include "Array.h"
#include "MemoryIO.h"


#include "Stats.h"
//#include "FileIO.h"
//#include "Parsers/BAMParser.h"
//#include "Parsers/SAMParser.h"
#include "Fields/Sequence.h"
#include "Fields/ReadName.h"
#include "Fields/MappingFlag.h"
//#include "Fields/EditOperation.h"
#include "Fields/MappingQuality.h"
#include "Fields/QualityScore.h"
#include "Fields/SAMComment.h"
//#include "Fields/PairedEnd.h"
//#include "Fields/OptionalField.h"


using namespace std;

struct index_t {
	size_t startPos, endPos;
	size_t zpos, currentBlockCount;
	size_t fS, fE;
	vector<Array<uint8_t>> fieldData;

	index_t(): fieldData(8) { }
};
typedef pair<pair<int, string>, pair<size_t, size_t>> range_t;

class FileDecompressor {
	vector<SAMComment> samComment;
	vector<SequenceDecompressor*> sequence;
	vector<EditOperationDecompressor*> editOp;
	vector<ReadNameDecompressor*> readName;
	vector<MappingFlagDecompressor*> mapFlag;
	vector<MappingQualityDecompressor*> mapQual;
	vector<QualityScoreDecompressor*> quality;
	vector<PairedEndDecompressor*> pairedEnd;
	//vector<shared_ptr<OptionalFieldDecompressor*> optField;
	
   	vector<string> fileNames;
#ifdef PRINT2FILE
	vector<FILE*> samFiles;
#endif
public:
	vector<MemoryIO*> samMemoryFiles;
	vector<map<string, map<size_t, index_t>>> indices;

	//File* inFile;
	MemoryIO* inMemoryFile;
	uint32_t magic;
	//gzFile idxFile;
	
	string genomeFile;
	string outFile;
	Stats* stats;
	size_t inFileSz;
    size_t blockSize;
   	vector<int> fileBlockCount;

   	bool finishedRange;

protected:
	const bool isAPI; // Ugly; hack for now
    //virtual inline void printRecord(const string &rname, int flag, const string &chr, const EditOperation &eo, int mqual,
    //    const string &qual, const string &optional, const PairedEndInfo &pe, int file, int thread);

   	vector<string> comments;
	inline void printRecord(const string &record, int file);
    virtual inline void printComment(int file);

public:
	static void printStats (const std::string &inFile, int filterFlag);
	int getnumFiles();
#ifdef PRINT2FILE
	void FileDecompressor::print2File();
#endif
public:
	//FileDecompressor (const std::string &inFile, const std::string &outFile, const std::string &genomeFile, int bs, bool isAPI = false);
	FileDecompressor (uint8_t* buffer0, size_t buffer0_size, const std::string &genomeFile, int bs, char *sealedDataMAC, bool isAPI = false);
	~FileDecompressor (void);

private:
	void getMagic (void);
	void getComment (void);
	size_t getBlock (int f, const std::string &chromosome, size_t start, size_t end, int filterFlag);
	void readBlock (Array<uint8_t> &in);
	void loadIndex (const char *outData, size_t len, size_t offset); 
	vector<range_t> getRanges (std::string range);
	

private: // TODO finish
	void query (const string &query, const string &range);

public:
    void decompress (int filterFlag);
	void decompress (const std::string &range, int filterFlag);
};

#endif // Decompress_H
