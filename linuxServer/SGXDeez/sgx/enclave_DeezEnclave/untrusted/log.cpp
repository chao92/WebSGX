/*
 * log.cpp
 *
 *  Created on: Aug 9, 2016
 *      Author: weijiawu
 */
#include <iostream>
#include <string>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "Header/log.h"

using namespace std;

bool IsFileExist(const std::string& path)
{
    struct stat info;
    if (stat(path.c_str(), &info) != 0)
    {
        return false;
    }
    return (info.st_mode & S_IFDIR) != 0;
}

bool MakeDirectory(const std::string& path)
{
    mode_t mode = 0755;
    int ret = mkdir(path.c_str(), mode);

    if (ret == 0)
        return true;

    switch (errno)
    {
    case ENOENT:
        // parent didn't exist, try to create it
        {
        	std::cout<<"Error: A component of the path prefix specified by path does not name an existing directory or path is an empty string."
        			<<std::endl;
            int pos = path.find_last_of('/');
            if (pos == std::string::npos)
                return false;
            if (!MakeDirectory( path.substr(0, pos) ))
                return false;
        }

        std::cout<<"Now, try to create again"<<std::endl;

        return 0 == mkdir(path.c_str(), mode);

    case EEXIST:
        // done!
    	std::cout<<"Warning: The named file exists"<<std::endl;
        return IsFileExist(path);

    default:
        return false;
    }
}

const std::string CurrentDateTime()
{
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y-%m-%d-%H-%M-%S", &tstruct);

	return buf;
}

bool RecordResult(void)
{
	std::string file_directory = "result/" + CurrentDateTime();
//	std::string file_directory = "result/a.txt";
	return MakeDirectory(file_directory);
}
