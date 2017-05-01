#include "Decompress.h"
#include "DeezEnclave.h"
#include "common.h"

using namespace std;

extern COUNT_DATA_TYPE** cnt;
extern int start1;
extern int end1;
extern char chr1[MAX_REFERENCE_NAME];
extern uint32_t len0;

void printf(const char *fmt, ...);

#define BUFFER_SIZE 1000

FileDecompressor::FileDecompressor (uint8_t* buffer0, size_t buffer0_size, const string &genomeFile, int bs, char *inputSealedDataMAC, bool isAPI): 
	blockSize(bs), genomeFile(genomeFile), outFile(outFile), finishedRange(false), isAPI(isAPI)
{
	initCache();
	inFileSz = buffer0_size;
	//// handle file IO /////////////////////////////
	inMemoryFile = new MemoryIO(inFileSz, BUFFER_SIZE, MemoryIO::IOMethods::READ, inputSealedDataMAC);
	//inMemoryFile->set(buffer0);
	//// handle file IO /////////////////////////////
	//magic = inFile->readU32();
	magic = inMemoryFile->readU32();

	if ((magic & 0xff) < 0x20)
		throw DZException("Old DeeZ files are not supported");

	// seek to index
	inMemoryFile->seek(inFileSz - sizeof(size_t));
	size_t statPos = inMemoryFile->readU64();
	char keymagic[10] = {0};
	inMemoryFile->read(keymagic, 7, statPos);
	if (strcmp(keymagic, "DZSTATS"))
		throw DZException("Stats are corrupted ...%c%c%c%c%c%c%c", keymagic[0], keymagic[1], keymagic[2], keymagic[3], keymagic[4], keymagic[5], keymagic[6]);

	size_t sz = inMemoryFile->readU64();
	Array<uint8_t> in(sz);
	in.resize(sz);
	inMemoryFile->read(in.data(), sz);
	stats = new Stats(in, magic);

	keymagic[5] = 0;
	inMemoryFile->read(keymagic, 5);
	if (strcmp(keymagic, "DZIDX"))
		throw DZException("Index is corrupted ...%s", keymagic);

	size_t idxToRead = inFileSz - inMemoryFile->tell() - sizeof(size_t);
	//size_t de_buffer_size = 10*idxToRead;
	//Bytef *de_buffer = (Bytef*)malloc(de_buffer_size);
	//int c = zlib_local::uncompress(de_buffer, (uLongf *)&de_buffer_size, (Bytef *)inMemoryFile->getCurrentPtr((uint64_t)idxToRead), idxToRead); // tmp remove
	string inData;
	string outData;

	char* temp_buffer = new char[idxToRead];
	inMemoryFile->read(temp_buffer, idxToRead);
	//inData.assign((char *)inMemoryFile->getCurrentPtr((uint64_t)idxToRead), idxToRead);
	inData.assign(temp_buffer, idxToRead);
	delete[] temp_buffer;
	bool rc = zlib_local::gzipInflate(inData, outData); 

	/*FILE *tmp = tmpfile();
	char *buffer = (char*)malloc(MB);
	while (idxToRead && (sz = inMemoryFile->read(buffer, min(uint64_t(MB), (uint64_t)idxToRead)))) {
		fwrite(buffer, 1, sz, tmp);
		idxToRead -= sz;
	}
	free(buffer);
	
	int idx = dup(fileno(tmp));
	fseek(tmp, 0, SEEK_SET);
	lseek(idx, 0, SEEK_SET); // needed for gzdopen
	idxFile = gzdopen(idx, "rb");
	if (idxFile == Z_NULL)
		throw DZException("Cannot open the index");
	//*/

	inMemoryFile->seek(0);

	getMagic();
	getComment();
	loadIndex(outData.c_str(), outData.size(), 0);
}

int gzread_mem(const char *outData, void * buf, size_t len, size_t *outData_size, size_t * offset)
{
	if(*offset+len <= *outData_size)
	{
		memcpy(buf, outData+*offset, len);
		*offset += len;
		return len;
	}else{
		return  0;
	}
}

//@param: [input]outData
//the outData is the index file, load index file information into the decompressor object
void FileDecompressor::loadIndex (const char *outData, size_t outData_size, size_t offset) 
{
	ZAMAN_START_P(LoadIndex);
	fileBlockCount.clear();
	bool firstRead = 1;
	indices.resize(fileNames.size());
	while (1) {
		//WARN("Here...%x",this->magic&0xff);
		//index_t idx;
		index_t idx_mem;
		//string chr;
		string chr_mem;
		//size_t sz;
		size_t sz_mem;
		if (firstRead) 
			firstRead = 0;
		else for (int i = 0; i < 8; i++) {
			//gzread(idxFile, &sz, sizeof(size_t));
			gzread_mem(outData, &sz_mem, sizeof(size_t), &outData_size, &offset);
			//idx.fieldData[i].resize(sz); 
			idx_mem.fieldData[i].resize(sz_mem); 
			/*if (sz) {
				gzread(idxFile, idx.fieldData[i].data(), sz); 
			}//*/
			if (sz_mem)
			{
				gzread_mem(outData, idx_mem.fieldData[i].data(), sz_mem, &outData_size, &offset);
			}
		}

		//int16_t f = 0;
		int16_t f_mem = 0;
		if ((this->magic & 0xff) >= 0x11) {
			/*if (gzread(idxFile, &f, sizeof(int16_t)) != sizeof(int16_t)) {
				//break;
			} //*/
			if (gzread_mem(outData, &f_mem, sizeof(int16_t), &outData_size, &offset) != sizeof(int16_t)) {
				break;
			}
			//gzread(idxFile, &idx.zpos, sizeof(size_t));
			gzread_mem(outData, &idx_mem.zpos, sizeof(size_t), &outData_size, &offset);
		}
		else {
			/*if (gzread(idxFile, &idx.zpos, sizeof(size_t)) != sizeof(size_t)){
				break;
			} //*/
			if (gzread_mem(outData, &idx_mem.zpos, sizeof(size_t), &outData_size, &offset) != sizeof(size_t)){
				break;
			}
		}

		fileBlockCount.push_back(f_mem);
		
		/*gzread(idxFile, &idx.currentBlockCount, sizeof(size_t));
		char c; while (gzread(idxFile, &c, 1) && c) chr += c;
		gzread(idxFile, &idx.startPos, sizeof(size_t));
		gzread(idxFile, &idx.endPos, sizeof(size_t));
		gzread(idxFile, &idx.fS, sizeof(size_t));
		gzread(idxFile, &idx.fE, sizeof(size_t)); //*/

		gzread_mem(outData, &idx_mem.currentBlockCount, sizeof(size_t), &outData_size, &offset);
		char c_mem; while (gzread_mem(outData, &c_mem, 1, &outData_size, &offset) && c_mem) chr_mem += c_mem;
		gzread_mem(outData, &idx_mem.startPos, sizeof(size_t), &outData_size, &offset);
		gzread_mem(outData, &idx_mem.endPos, sizeof(size_t), &outData_size, &offset);
		gzread_mem(outData, &idx_mem.fS, sizeof(size_t), &outData_size, &offset);
		gzread_mem(outData, &idx_mem.fE, sizeof(size_t), &outData_size, &offset);

		//if (inMemory)
		indices[f_mem][chr_mem][idx_mem.startPos] = idx_mem;
	}
	ZAMAN_END_P(LoadIndex);
}

void FileDecompressor::getComment (void) 
{
	comments.resize(fileNames.size());
	for (int f = 0; f < fileNames.size(); f++) {
		size_t arcsz = inMemoryFile->readU64();
		string comment = "";
		if (arcsz) {
			Array<uint8_t> arc;
			arc.resize(arcsz);
			arcsz = inMemoryFile->readU64();
			Array<uint8_t> commentArr;
			commentArr.resize(arcsz);
			inMemoryFile->read(commentArr.data(), arcsz);
			
			GzipDecompressionStream gzc;
			gzc.decompress(commentArr.data(), commentArr.size(), arc, 0);
			comment = string((char*)arc.data(), arc.size());
		}
		comments[f] = comment;
		samComment.push_back(SAMComment(comments[f]));
	}
}

void FileDecompressor::readBlock (Array<uint8_t> &in) 
{
	size_t sz = inMemoryFile->readU64();
	in.resize(sz);
	if (sz) inMemoryFile->read(in.data(), sz);
}


FileDecompressor::~FileDecompressor (void) 
{
	for (int f = 0; f < fileNames.size(); f++) {
#ifdef PRINT2FILE
		if (samFiles[f]) 
			fclose(samFiles[f]);
#endif
//#ifdef NON_SGX
		if (samMemoryFiles[f])
			delete samMemoryFiles[f];
//#endif
	}
//	if (idxFile) 
//		gzclose(idxFile);
//#ifdef NON_SGX //by wwj
//	if(inMemoryFile)
//	{
//		delete inMemoryFile;
//	}
//#endif
}


#ifdef PRINT2FILE
void FileDecompressor::print2File(){
	for (int f = 0; f < samMemoryFiles.size(); f++)
	{
		char tmp = '\0';
		samMemoryFiles[f]->write(&tmp, 1);
		fputs(samMemoryFiles[f]->fh_out.data(), samFiles[f]);
	}
}
#endif

inline void FileDecompressor::printRecord(const string &record, int file) 
{
	//fputs(record.c_str(), samFiles[file]);
	//fputc('\n', samFiles[file]);
	samMemoryFiles[file]->write(record.c_str(), record.size());
	char tmp = '\n';
	samMemoryFiles[file]->write(&tmp, 1);

}

inline void FileDecompressor::printComment(int file) 
{
    //fputs(comments[file].c_str(), samFiles[file]);
	samMemoryFiles[file]->write(comments[file].c_str(), comments[file].size());
}

int FileDecompressor::getnumFiles(){
	return samMemoryFiles.size();
}
void FileDecompressor::getMagic(void) {
    magic = inMemoryFile->readU32();
    LOG("File format: %c%c v%d.%d",
            (magic >> 16) & 0xff,
            (magic >> 8) & 0xff,
            (magic >> 4) & 0xf,
            magic & 0xf
            );
    optQuality = inMemoryFile->readU8();
    optBzip = inMemoryFile->readU8();
    if (optBzip) LOG("Using bzip decoding");

    uint16_t numFiles = 1;
    if ((magic & 0xff) >= 0x11) // DeeZ v1.1
        numFiles = inMemoryFile->readU16();

    for (int f = 0; f < numFiles; f++) {
        sequence.push_back(new SequenceDecompressor(genomeFile, blockSize));
        editOp.push_back(new EditOperationDecompressor(blockSize, (*sequence.back())));
        readName.push_back(new ReadNameDecompressor(blockSize));
        mapFlag.push_back(new MappingFlagDecompressor(blockSize));
        mapQual.push_back(new MappingQualityDecompressor(blockSize));
        pairedEnd.push_back(new PairedEndDecompressor(blockSize));
        //optField.push_back(new OptionalFieldDecompressor(blockSize));
        quality.push_back(new QualityScoreDecompressor(blockSize));
    }
    fileNames.resize(numFiles);

    if ((magic & 0xff) >= 0x11) { // DeeZ v1.1 file name information
        size_t arcsz = inMemoryFile->readU64();
        if (arcsz) {
            Array<uint8_t> arc;
            arc.resize(arcsz);
            arcsz = inMemoryFile->readU64();
            Array<uint8_t> files;
            files.resize(arcsz);
            inMemoryFile->read(files.data(), arcsz);

            GzipDecompressionStream gzc;
            gzc.decompress(files.data(), files.size(), arc, 0);

            arcsz = 0;
            for (int f = 0; f < fileNames.size(); f++) {
                fileNames[f] = string((const char*) (arc.data()) + arcsz);
                arcsz += fileNames[f].size();
            }
        }
    }

	samMemoryFiles.resize(numFiles);
    for (int f = 0; f < numFiles; f++) {
        samMemoryFiles[f] = new MemoryIO(10000, BUFFER_SIZE, MemoryIO::IOMethods::WRITE);
    }

#ifdef PRINT2FILE
	samFiles.resize(numFiles);
    for (int f = 0; f < numFiles; f++) {
        if (optStdout) {
            samFiles[f] = stdout;
        } else if (outFile != "") {
            string fn = outFile;
            if (numFiles > 1) fn += S("_%d", f + 1);
            samFiles[f] = fopen(fn.c_str(), "wb");
            if (samFiles[f] == NULL)
                throw DZException("Cannot open the file %s", fn.c_str());
        }
    }
#endif
}


size_t FileDecompressor::getBlock (int f, const string &chromosome, 
	size_t start, size_t end, int filterFlag) 
{
	static string chr;
	if (finishedRange) 
		return 0;
	if (chromosome != "")
		chr = chromosome;

	char chflag;
	if (inMemoryFile->read(&chflag, 1) != 1)
		return 0;
	if (chflag > 1) // index!
		return 0;
	if (chflag)
	{
		chr = "";
		chflag = inMemoryFile->readU8();

		while (chflag)
			chr += chflag, chflag = inMemoryFile->readU8();

		if (chromosome != "" && chr != chromosome)
			return 0;
	}

	ocall_print_string("Before while (chr != sequence[f]->getChromosome())\n");

	while (chr != sequence[f]->getChromosome())
	{
		ocall_print_string("Before one sequence[f]->scanChromosome(chr, samComment[f]);\n");
		sequence[f]->scanChromosome(chr, samComment[f]);
		ocall_print_string("After one sequence[f]->scanChromosome(chr, samComment[f]);\n");
	}
	ocall_print_string("After while (chr != sequence[f]->getChromosome())\n");

	ZAMAN_START_P(Blocks);
	Array<uint8_t> in[8];

	for (int ti = 0; ti < 8; ti++)
	{
		if(ti > 6) break; // break here as we do not need to handle optional field. 6 is enough
		readBlock(in[ti]);
	}

	ocall_print_string("Finish 6 readBlock.\n");

	//ctpl::thread_pool threadPool(optThreads);
	/*threadPool.push([&](int t, Array<uint8_t> &buffer) {
		optField[f]->importRecords(buffer.data(), buffer.size());
		ZAMAN_THREAD_JOIN();
		// LOG("opt done");
	}, ref(in[7]));//*/
        //optField[f]->importRecords(in[7].data(), in[7].size());
	/*threadPool.push([&](int t, Array<uint8_t> &buffer) {
		sequence[f]->importRecords(buffer.data(), buffer.size());
		ZAMAN_THREAD_JOIN();
		// LOG("seq done");
	}, ref(in[0])); //*/
        sequence[f]->importRecords(in[0].data(), in[0].size());
	/*threadPool.push([&](int t, Array<uint8_t> &buffer) {
		editOp[f]->importRecords(buffer.data(), buffer.size());
		ZAMAN_THREAD_JOIN();
		// LOG("seq done");
	}, ref(in[1])); //*/
        editOp[f]->importRecords(in[1].data(), in[1].size());
	/*threadPool.push([&](int t, Array<uint8_t> &buffer) {
		readName[f]->importRecords(buffer.data(), buffer.size());
		ZAMAN_THREAD_JOIN();
		// LOG("rname done");
	}, ref(in[2])); //*/
        readName[f]->importRecords(in[2].data(), in[2].size());
	/*threadPool.push([&](int t, Array<uint8_t> &buffer) {
		mapFlag[f]->importRecords(buffer.data(), buffer.size());
		ZAMAN_THREAD_JOIN();
		// LOG("mflag done");
	}, ref(in[3])); //*/
        mapFlag[f]->importRecords(in[3].data(), in[3].size());
	/*threadPool.push([&](int t, Array<uint8_t> &buffer) {
		mapQual[f]->importRecords(buffer.data(), buffer.size());
		ZAMAN_THREAD_JOIN();
		// LOG("mqual done");
	}, ref(in[4])); //*/
        mapQual[f]->importRecords(in[4].data(), in[4].size());
	/*threadPool.push([&](int t, Array<uint8_t> &buffer) {
		quality[f]->importRecords(buffer.data(), buffer.size());
		ZAMAN_THREAD_JOIN();
		// LOG("qual done");
	}, ref(in[5])); //*/
        quality[f]->importRecords(in[5].data(), in[5].size());
	/*threadPool.push([&](int t, Array<uint8_t> &buffer) {
		pairedEnd[f]->importRecords(buffer.data(), buffer.size());
		ZAMAN_THREAD_JOIN();
		// LOG("pe done");
	}, ref(in[6])); //*/
        pairedEnd[f]->importRecords(in[6].data(), in[6].size());
	//optField[f]->decompressThreads(threadPool);
	//threadPool.stop(true);
	ZAMAN_END_P(Blocks);

	ocall_print_string("Stop early if slice/random access.\n");

	// TODO: stop early if slice/random access
	ZAMAN_START_P(CheckMate);
	for (int i = 0, j = 0; i < editOp[f]->size(); i++) {
		PairedEndInfo &pe = (*pairedEnd[f])[i];
		if (pe.bit == PairedEndInfo::Bits::LOOK_BACK) {
			int prevPos = i - readName[f]->getPaired(j++);
			(*readName[f])[i] = (*readName[f])[prevPos];
						
			EditOperation &eo = (*editOp[f])[i];
			EditOperation &peo = (*editOp[f])[prevPos];
			PairedEndInfo &ppe = (*pairedEnd[f])[prevPos];

			ppe.tlen = eo.start + (peo.end - peo.start) - peo.start;
			ppe.pos = eo.start;
			pe.pos = peo.start;
			pe.tlen = -ppe.tlen;

			if (ppe.bit == PairedEndInfo::Bits::LOOK_AHEAD_1) 
				pe.tlen++, ppe.tlen--;
		}
	}
	ZAMAN_END_P(CheckMate);

	ZAMAN_START_P(Parse);
	size_t recordCount = editOp[f]->size();
	/*size_t threadSz = recordCount / optThreads + 1;
	vector<thread> threads(optThreads);
	vector<vector<string>> records(optThreads);
	vector<char> finishedRangeThread(optThreads, 0);
	size_t count = 0;

	for (int ti = 0; ti < optThreads; ti++) {
		if (ti) records[ti].reserve(threadSz);
		threads[ti] = thread([&](int ti, size_t S, size_t E) {
			ZAMAN_START(Thread);
			size_t maxLen = 255;
			string record;
			for (size_t i = S; i < E; i++) {
				int flag = mapFlag[f]->getRecord(i);

				if (filterFlag) {
					if (filterFlag > 0 && (flag & filterFlag) != filterFlag)
						continue;
					if (filterFlag < 0 && (flag & -filterFlag) == -filterFlag)
						continue;
				}

				auto &eo = editOp[f]->getRecord(i);
				auto &pe = pairedEnd[f]->getRecord(i, eo.start, eo.end - eo.start, flag & 0x10);

				if (chr != "*") 
					eo.start++;
				if (pe.chr != "*") 
					pe.pos++;

				if (eo.start < start)
					continue;
				if (eo.start > end) {
					finishedRangeThread[ti] = true;
					return;
				}

				if (isAPI) {
					string of;
					optField[f]->getRecord(i, eo, of);
					printRecord(readName[f]->getRecord(i), 
						flag, chr, eo, 
						mapQual[f]->getRecord(i),
						quality[f]->getRecord(i, eo.seq.size(), flag), 
						of, pe, f, ti);
					continue;
				}

				record.resize(0);
				record += readName[f]->getRecord(i);
				record += '\t';
				inttostr(flag, record); 
				record += '\t';
				record += chr; 
				record += '\t';
				inttostr(eo.start, record); 
				record += '\t';
				inttostr(mapQual[f]->getRecord(i), record); 
				record += '\t';
				record += eo.op; 
				record += '\t';
				record += pe.chr; 
				record += '\t';
				inttostr(pe.pos, record); 
				record += '\t';
				inttostr(pe.tlen, record); 
				record += '\t';
				record += eo.seq; 
				record += '\t';
				record += quality[f]->getRecord(i, eo.seq.size(), flag);
				optField[f]->getRecord(i, eo, record);
				maxLen = max(record.size(), maxLen);
				record.reserve(maxLen);

				if (ti == 0) {
					printRecord(record, f);
					count++;
				} else {
					records[ti].push_back(record);
				}
			}
			ZAMAN_END(Thread);
			ZAMAN_THREAD_JOIN();
		}, ti, threadSz * ti, min(recordCount, (ti + 1) * threadSz));
	}
	for (int ti = 0; ti < optThreads; ti++) {
		threads[ti].join();
		if (finishedRangeThread[ti])
			finishedRange = true;
	}//*/
        
        // changed by Shuang Wang
        string record;
        vector<string> records; //(recordCount);
        for (size_t i = 0; i < recordCount; i++) {
			size_t maxLen = 255;
                int flag = mapFlag[f]->getRecord(i);
                if (filterFlag) {
                        if (filterFlag > 0 && (flag & filterFlag) != filterFlag)
                                continue;
                        if (filterFlag < 0 && (flag & -filterFlag) == -filterFlag)
                                continue;
                }

                auto &eo = editOp[f]->getRecord(i);
                auto &pe = pairedEnd[f]->getRecord(i, eo.start, eo.end - eo.start, flag & 0x10);

                if (chr != "*") 
                        eo.start++;
                if (pe.chr != "*") 
                        pe.pos++;

                if (eo.start < start)
                        continue;
                if (eo.start > end) {
                        //finishedRangeThread[ti] = true;
                        finishedRange = true;
                        break;
                }

                record.resize(0);
                record += readName[f]->getRecord(i);
                record += '\t';
                inttostr(flag, record); 
                record += '\t';
                record += chr; 
                record += '\t';
                inttostr(eo.start, record); 
                record += '\t';
                inttostr(mapQual[f]->getRecord(i), record); 
                record += '\t';
                record += eo.op; 
                record += '\t';
                record += pe.chr; 
                record += '\t';
                inttostr(pe.pos, record); 
                record += '\t';
                inttostr(pe.tlen, record); 
                record += '\t';
                record += eo.seq; 
                record += '\t';
                record += quality[f]->getRecord(i, eo.seq.size(), flag);
                //optField[f]->getRecord(i, eo, record);
                maxLen = max(record.size(), maxLen);
                record.reserve(maxLen);

				int tmp = updatingCounts(chr1, (uint8_t*)record.c_str(), start1, end1, cnt);

                //records.push_back(record);
        }
		finishedRange = true;
        // changed by Shuang Wang
	ZAMAN_END_P(Parse);
	
	ZAMAN_START_P(Write);
	/*for (auto &v: records)
		for (auto &r: v) {
			printRecord(r, f);
			count++;
		}
        //*/
        size_t count = 0;
        for (auto &r: records) {
			printRecord(r, f);
			count++;
        }
	LOGN("\r\t%5.2lf%% [Chr %-10s]", (100.0 * inMemoryFile->tell()) / inFileSz, chr.substr(0, 10).c_str());
	ZAMAN_END_P(Write);
	
	ZAMAN_THREAD_JOIN();
	return count;
}

vector<range_t> FileDecompressor::getRanges (string range)
{
	if (range.size() && range[range.size() - 1] != ';') 
		range += ';';
	vector<range_t> ranges;

	size_t p;
	while ((p = range.find(';')) != string::npos) {
		string chr;
		size_t start, end;
		//char *dup = strdup(range.substr(0, p).c_str());
		char *dup = new char[strlen(range.substr(0, p).c_str()) + 1];
		memcpy(dup, range.substr(0, p).c_str(), strlen(range.substr(0, p).c_str()) + 1);
		char *tok = strtok(dup, ":-");
		
		if (tok) {
			chr = tok, tok = strtok(0, ":-");
			if (tok) {
				start = atol(tok), tok = strtok(0, ":-");
				if (tok) 
					end = atol(tok), tok = strtok(0, ":-");
				else 
					end = -1;
			}
			else {
				start = 1; end = -1;
			}
		}
		else throw DZException("Range string %s invalid", range.substr(0, p - 1).c_str());
		if (end < start)
			swap(start, end);
		if (start) start--; 
		if (end) end--;

		range = range.substr(p + 1);
		int f = 0;
		if ((p = chr.find(',')) != string::npos) {
			f = atoi(chr.substr(0, p).c_str());
			chr = chr.substr(p + 1);
		}

		ranges.push_back(make_pair(make_pair(f, chr), make_pair(start, end)));
		delete[] dup;
	}

	return ranges;
}

void FileDecompressor::decompress (int filterFlag) 
{
	ZAMAN_START_P(Decompress);
	for (int f = 0; f < comments.size(); f++)
		printComment(f);

	size_t blockSz = 0, 
		   totalSz = 0, 
		   blockCount = 0;
	while (blockCount < fileBlockCount.size() && (blockSz = getBlock(fileBlockCount[blockCount], "", 0, -1, filterFlag)) != 0) {
		totalSz += blockSz;
		blockCount++;
	}
	LOGN("\nDecompressed %'lu records, %'lu blocks\n", totalSz, blockCount);
	ZAMAN_END_P(Decompress);
}

void FileDecompressor::decompress (const string &range, int filterFlag)
{
	ZAMAN_START_P(Decompress);
	auto ranges = getRanges(range);
	size_t 	blockSz = 0, 
			totalSz = 0, 
			blockCount = 0;

	//foreach (r, ranges) {
	for (auto r = ranges.begin(); r != ranges.end(); ++r)
	{
		int f = r->first.first;
		string chr = r->first.second;
		if (f < 0 || f >= fileNames.size())
			throw DZException("Invalid sample ID %d", f);
		if (indices[f].find(chr) == indices[f].end())
			throw DZException("Invalid chromosome %s for sample ID %d", chr.c_str(), f);

		auto idx = indices[f][chr];
		auto i = idx.upper_bound(r->second.first);
		if (i == idx.begin() || idx.size() == 0)
			throw DZException("Region %s:%d-%d not found for sample ID %d", 
				chr.c_str(), r->second.first, r->second.second, f);
		i--;
		
		// prepare reference
		//foreach (j, idx) { // TODO speed up
		for (auto j = idx.begin(); j != idx.end(); ++j)
		{
			if (j == i) break;
			if (r->second.first >= j->second.fS && r->second.first <= j->second.fE)
			{
				inMemoryFile->seek(j->second.zpos);
				char chflag = inMemoryFile->readU8();
				while (chflag) chflag = inMemoryFile->readU8();
				while (chr != sequence[f]->getChromosome())
					sequence[f]->scanChromosome(chr, samComment[f]);

				Array<uint8_t> in;
				readBlock(in);
				sequence[f]->importRecords(in.data(), in.size());
			}
		}
		// set up field data
		while (r->second.first >= i->second.startPos && r->second.first <= i->second.endPos) {		
			inMemoryFile->seek(i->second.zpos);
			Decompressor* di[] = { 
				sequence[f], editOp[f], readName[f], mapFlag[f], 
				mapQual[f], quality[f], pairedEnd[f], NULL//, optField[f] 
			};

			for (int ti = 0; ti < 8; ti++) 
			{
				if (i->second.fieldData[ti].size()) {
					di[ti]->setIndexData(i->second.fieldData[ti].data(), i->second.fieldData[ti].size());
				}
			}
			size_t blockSz = getBlock(f, chr, r->second.first, r->second.second, filterFlag);
			if (!blockSz) break;
			totalSz += blockSz;
			blockCount++;

			if (finishedRange) {
				finishedRange = false;
				break;
			}

			i++;
		}
	}
	ZAMAN_END_P(Decompress);
	LOGN("\nDecompressed %'lu records, %'lu blocks\n", totalSz, blockCount);
}
