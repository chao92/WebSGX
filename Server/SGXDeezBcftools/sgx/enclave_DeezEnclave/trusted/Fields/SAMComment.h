#ifndef SAMComment_H
#define SAMComment_H

#include <map>

#include "common.h"
#include "Utils.h"
//#include <unordered_map>

struct SAMComment
{
	std::vector<std::string> lines;
	std::map<std::string, int> PG, RG;
	std::map<std::string, std::map<std::string, std::string>> SQ;
	SAMComment(const std::string &comment);
};

#endif // SAMComment_H
