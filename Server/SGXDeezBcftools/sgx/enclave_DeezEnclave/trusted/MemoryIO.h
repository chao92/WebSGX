#ifndef MemoryIO_H
#define MemoryIO_H

#include "common.h"
#include "Utils.h"
//#include "zlib/zlib.h"
//#include "Utils.h"

using namespace std;

//#ifdef  __cplusplus
//extern "C" {
//#endif

#define MEMORY_FILE uint8_t


class MemoryIO
{
public:
	enum IOMethods
	{
		READ,
		WRITE
	};

	MEMORY_FILE *fh;
	vector<char> fh_out;
	size_t current_pos;
	size_t fsize;
	size_t buffer_size;
	pair<size_t, size_t> current_region;
	IOMethods currentIOMethods;

protected:
	MemoryIO ();

public:
	//void * getCurrentPtr(size_t size);
	MemoryIO (MEMORY_FILE *handle, size_t _fsize, size_t _buffer_size, char* sealedDataMAC = NULL);
	MemoryIO (size_t _fsize, size_t _buffer_size, IOMethods myMethod, char* sealedDataMAC = NULL);
	int fetchBuffer (pair<size_t, size_t> region2fetch);
	//MemoryIO (const string &path, const char *mode);
	virtual ~MemoryIO ();

	//virtual void open (const string &path, const char *mode);
	virtual void close ();

	virtual ssize_l_t read (void *buffer, size_t size);
	virtual ssize_l_t read (void *buffer, size_t size, size_t offset);
	virtual ssize_l_t write (void const *buffer, size_t size);
	virtual size_t advance(size_t size);

	char my_getc(void);
	virtual uint8_t readU8();
	virtual uint16_t readU16();
	virtual uint32_t readU32();
	virtual uint64_t readU64();

	virtual ssize_l_t tell ();
	virtual ssize_l_t seek (size_t pos);

	virtual size_t size ();
	virtual bool eof ();
	virtual void *handle ();

	void set(void * _fh);

private:
	//virtual void get_size ();

public:
	static MemoryIO* Open(const string &path, const char *mode);
	static bool Exists (const string &path);
	static string FullPath (const string &s);
	static string RemoveExtension (const string &s);
	char* sealedDataMAC;
	//static bool IsWeb (const string &path);
	//static bool IsS3 (const string &path);
	//static string GetURLforS3 (string url, CURL *ch, string method = "GET");
};

/*class GzMemory_File: public MemoryIO
{
	MEMORY_FILE *fh;

public:
	GzMEMORY_FILE (MEMORY_FILE *handle);
	GzMEMORY_FILE (const string &path, const char *mode);
	~GzMEMORY_FILE ();

	void open (const string &path, const char *mode);
	void close ();

	ssize_l_t read (void *buffer, size_t size);
	ssize_l_t read(void *buffer, size_t size, size_t offset);
	ssize_l_t write (void *buffer, size_t size);

	ssize_l_t tell ();
	ssize_l_t seek (size_t pos);

	size_t size ();
	bool eof ();
	void *handle ();

private:
	void get_size ();
}; //*/

//#ifdef  __cplusplus
//}
//#endif

#endif // MEMORYIO_H
