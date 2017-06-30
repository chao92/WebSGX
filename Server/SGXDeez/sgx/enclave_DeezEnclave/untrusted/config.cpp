#include  "Header/config.h"
#include "Header/common.h"

Config::Config(void)
{
}


Config::~Config(void)
{
}

void Config::Trim( string& inout_s )  
{  
    // Remove leading and trailing whitespace  
    static const char whitespace[] = " \n\t\v\r\f";  
    inout_s.erase( 0, inout_s.find_first_not_of(whitespace) );  
    inout_s.erase( inout_s.find_last_not_of(whitespace) + 1U );  
}  

int Config::Parse(char *filePath)
{
    std::ifstream in(filePath, ios::in);
    if(!in) 
    {
    	printf("Config file not found\n");
    	return -1;
    }

//    in.open(filePath, ios::in);


    const string delim  = "=";  // separator
    const string comm   = "#";    // comment

    string nextline = "";  // might need to read ahead to see where value ends  
  
    while (in || nextline.length() > 0)  
    {  
        // Read an entire line at a time  
        string line;  
        if( nextline.length() > 0 )  
        {  
            line = nextline;  // we read ahead; use it now  
            nextline = "";  
        }  
        else  
        {  
            getline(in, line, '\n');
        }  
  
        // Ignore comments  
        line = line.substr( 0, line.find(comm) );  
  
        // Parse the line if it contains a delimiter  
        int delimPos = line.find( delim );  
        if( delimPos < string::npos )  
        {  
            // Extract the key  
            string key = line.substr( 0, delimPos );  
            line.replace( 0, delimPos+1, "" );  
  
            // See if value continues on the next line  
            // Stop at blank line, next line with a key, end of stream,  
            // or end of file sentry  
            bool terminate = false;  
            while( !terminate && in )  
            {  
                getline( in, nextline, '\n');  
                terminate = true;  
  
                string nlcopy = nextline;  
                Config::Trim(nlcopy);  
                if( nlcopy == "" ) continue;  
  
                nextline = nextline.substr( 0, nextline.find(comm) );  
                if( nextline.find(delim) != string::npos )  
                    continue;  
  
                nlcopy = nextline;  
                Config::Trim(nlcopy);  
                if( nlcopy != "" ) line += "\n";  
                line += nextline;  
                terminate = false;  
            }  
  
            // Store key and value  
            Config::Trim(key);  
            Config::Trim(line);  
            
            contents[key] = line;
        }  
    }  
  
    return 1;  
}  


string Config::Read(string key)
{
    std::map<string,string>::iterator iter = contents.find(key);

    if( contents.end() != iter )
    {
	return iter->second;
    }

    return NULL;
}

bool Config::Write(string key, string value)
{
  map<string, string>::iterator iter = contents.find(key);
  if(contents.end() != iter)
  {
      iter->second = value;
      return true;
  }
  else
  {
      printf("There is no such key word in client's information.");
      return false;
  }
}

const string Config::KeyName[11] =
{ "ClientNum", "AccountCount", "ServerPort", "Compression", "RequestSummary",
		"SSL", "Username", "Password", "Algorithm", "TopK", "SegmentLength" }; //TODO:Username 0 1 2 3

int Config::GetClientNumber(void)
{
	return StringToInt(this->Read(this->KeyName[0]));
}

int Config::GetAccountCount(void)
{
	return StringToInt(this->Read(this->KeyName[1]));
}

int Config::GetPort(void)
{
	return StringToInt(this->Read(this->KeyName[2]));
}

int Config::GetCompression(void)
{
	return StringToInt(this->Read(this->KeyName[3]));
}

int Config::GetRequestSummary(void)
{
	return StringToInt(this->Read(this->KeyName[4]));
}

int Config::GetSSL(void)
{
	return StringToInt(this->Read(this->KeyName[5]));
}

string Config::GetUsername(int user_number)
{
	string number = IntToString(user_number);
	return this->Read(this->KeyName[6] + number);
}

string Config::GetPassword(int user_number)
{
	string number = IntToString(user_number);
	return this->Read(this->KeyName[7] + number);
}

int Config::GetAlgorithm(void)
{
	return StringToInt(this->Read(this->KeyName[8]));
}

int Config::GetTopK(void)
{
	return StringToInt(this->Read(this->KeyName[9]));
}

int Config::GetSegmentLength(void)
{
	return StringToInt(this->Read(this->KeyName[10]));
}
