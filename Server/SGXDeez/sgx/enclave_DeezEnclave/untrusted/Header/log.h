#ifndef LOG_H
#define LOG_H

bool IsFileExist(const std::string& path);

bool MakeDirectory(const std::string& path);

const std::string CurrentDateTime();

extern bool RecordResult(void);

#endif
