#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <map>  

#define SEALING_BUFFER_SIZE 1024
#define SEALED_BUFFER_SIZE 1760
#define SECRET_MAC_SIZE 64
#define ENCRYPTED_ADD_SIZE 16
#define TRANSFERBOLCKS		5000
using namespace std; 

class Config
{
public:
	Config(void);
	~Config(void);

	void Trim(string& inout_s);  
	int Parse(char *filePath);
	string Read(string key);
	bool Write(string key, string value);

	int GetClientNumber(void);
	int GetAccountCount(void);
	int GetPort(void);
	int GetCompression(void);
	int GetRequestSummary(void);
	int GetSSL(void);
	string GetUsername(int user_number);
	string GetPassword(int user_number);
	int GetAlgorithm(void);
	int GetTopK(void);
	int GetSegmentLength(void);

	static const string KeyName[11];

private:
	map<std::string,std::string> contents;  //!< extracted keys and values
};

