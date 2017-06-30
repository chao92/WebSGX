#include "MemoryIO.h"
#include <assert.h>
#include <stdio.h>
#include"DataSealing.h"
#include "DeezEnclave_t.h"
#include "DeezEnclave.h"

void printf(const char *fmt, ...);
/*************************************************************************/

MemoryIO::MemoryIO () 
{
	fh = 0;
}

MemoryIO::MemoryIO (size_t _fsize, size_t _buffer_size, IOMethods myMethod, char* inputSealedDataMAC) 
{
	this->sealedDataMAC = inputSealedDataMAC;
	if (myMethod == IOMethods::READ )
	{
		fh = new MEMORY_FILE[_buffer_size];
		current_pos = 0;
		fsize = _fsize;
		buffer_size = _buffer_size;
	}
	else{
		//fh_out.resize(max(_fsize, 10000));
		current_pos = 0;
		fsize = 0;
		buffer_size = _buffer_size;
	}
	currentIOMethods = myMethod;

}

//#define  ocall_fetchBuffer(region2fetch, fh) 1

#define FETCHBUFFER_FAILED 1
int MemoryIO::fetchBuffer (pair<size_t, size_t> region2fetch)
{
	if(region2fetch.second > fsize || (region2fetch.second <= region2fetch.first) || region2fetch.first < 0)
	{
		return FETCHBUFFER_FAILED;
	}
	
	int64_t start_index = region2fetch.first / SEALING_BUFFER_SIZE;

	size_t start_position_of_the_first_seg2unseal = region2fetch.first / SEALING_BUFFER_SIZE * SEALED_BUFFER_SIZE;
	size_t start_position_of_the_last_seg2unseal = region2fetch.second/ SEALING_BUFFER_SIZE * SEALED_BUFFER_SIZE;
	size_t seg_count2unseal = (start_position_of_the_last_seg2unseal - start_position_of_the_first_seg2unseal) / SEALED_BUFFER_SIZE + 1;
	
	size_t size_to_fetch = seg_count2unseal * SEALED_BUFFER_SIZE;
	
	//int buffer_count_to_fetch = size_to_fetch / SEALING_BUFFER_SIZE + 2;
	
	uint8_t* fetched_buffers2unseal = new uint8_t [seg_count2unseal * SEALED_BUFFER_SIZE];
	uint8_t* unsealed_buffers = new uint8_t [seg_count2unseal * SEALING_BUFFER_SIZE];

	int out_res = 0;
	//printf("seg_count2unseal: %d\n", seg_count2unseal);
	int res = ocall_fetchBuffer(&out_res, fetched_buffers2unseal, size_to_fetch, start_position_of_the_first_seg2unseal);
	
	if(res != SGX_SUCCESS)
	{
		return FETCHBUFFER_FAILED;
	}
	if ( out_res != 0) {
		return FETCHBUFFER_FAILED;
	}


	//unseal(fh);
	//SGX_ERROR_INVALID_P

	//protected_secret unsealed;
	
	/* Modified by Shadow at 2016-7-14*/
	char* secret = new char[SEALING_BUFFER_SIZE];
	for (int i = 0; i < seg_count2unseal; i ++)
	{
		memset_s(secret,SEALING_BUFFER_SIZE,0,SEALING_BUFFER_SIZE);
		res = unseal_data((fetched_buffers2unseal + i * SEALED_BUFFER_SIZE),SEALED_BUFFER_SIZE,
			(uint8_t *)secret,SEALING_BUFFER_SIZE, start_index + i, (uint8_t *)(this->sealedDataMAC));
		if(res != SGX_SUCCESS)
		{
			ocall_print_string("unseal data failed, error code = ");
			ocall_print_string("\r\n");
			return FETCHBUFFER_FAILED;
		}
		//res = unseal((sgx_sealed_data_t*) (fetched_buffers2unseal + i * SEALED_BUFFER_SIZE), (char*)(&unsealed));
		//printf ("unseal result: %d\n", res);
		memcpy(unsealed_buffers + i * SEALING_BUFFER_SIZE, secret, SEALING_BUFFER_SIZE);
		if (res!=0) {
			return FETCHBUFFER_FAILED;
		}
	}

	memcpy(fh, unsealed_buffers + (region2fetch.first%SEALING_BUFFER_SIZE), region2fetch.second-region2fetch.first);

	delete[] fetched_buffers2unseal;
	delete[] unsealed_buffers;
	delete[] secret;
	current_region = region2fetch;
	//current_pos = 0;
	return !FETCHBUFFER_FAILED;
}

MemoryIO::MemoryIO (MEMORY_FILE *handle, size_t _fsize, size_t _buffer_size, char* inputSealedDataMAC) 
{
	this->sealedDataMAC = inputSealedDataMAC;
	fh = handle;
	current_pos = 0;
	fsize = _fsize;
	buffer_size = _buffer_size;
}

MemoryIO::~MemoryIO () 
{
	close();
}

void MemoryIO::close () 
{ 
	if(currentIOMethods == IOMethods::READ){
		if (fh) 
			delete [] fh;
	}
}

ssize_l_t MemoryIO::read (void *buffer, size_t size) 
{ 
	assert(currentIOMethods == IOMethods::READ);
	if (fsize >= current_pos + size)
	{
		//fread(buffer, 1, size, fh); 
		//assert(current_pos >= current_region.first && current_pos <= current_region.second);		
		if (current_pos < current_region.first || current_pos > current_region.second) {
			fetchBuffer(pair<size_t, size_t>(current_pos, current_pos+min(buffer_size, fsize-current_pos)));
		}
		int read_size = min(current_region.second-current_pos, size);
		memcpy(buffer, fh + (current_pos-current_region.first), read_size);
		current_pos += read_size;
		while (read_size != size)
		{
			if(!(fetchBuffer(pair<size_t, size_t>(current_pos, current_pos+min(buffer_size, fsize-current_pos)))== FETCHBUFFER_FAILED))
			{
				int tmp_size = min(current_region.second-current_pos, size-read_size);
				memcpy((char*)buffer + read_size, fh + (current_pos-current_region.first), tmp_size);
				current_pos += tmp_size;
				read_size += tmp_size;
			}
			else{
				return 0;
			}

		}
		assert(read_size == size);
		return read_size;

	}else
	{
		return 0;
	}
}

/*void * MemoryIO::getCurrentPtr(size_t size) 
{ 
assert(currentIOMethods == IOMethods::READ);
if (fsize >= current_pos + size)
{
return fh + current_pos;
}else
{
return NULL;
}
}*/

ssize_l_t MemoryIO::read (void *buffer, size_t size, size_t offset) 
{
	//fseek(fh, offset, SEEK_SET); // Beginning of file
	assert(currentIOMethods == IOMethods::READ);
	current_pos = offset;
	return read(buffer, size);
}

size_t MemoryIO::advance(size_t size)
{
	assert(currentIOMethods == IOMethods::READ);
	//fseek(fh, size, SEEK_CUR); // Current position of the file pointer
	current_pos += size;
	return current_pos; //ftell(fh);
}	


char MemoryIO::my_getc()
{
	char c;
	if (read(&c, 1) == 0)
	{
		c = EOF;
	}
	return c;
}

uint8_t MemoryIO::readU8 () 
{
	uint8_t var;
	if (read(&var, sizeof(uint8_t)) != sizeof(uint8_t))
	{
		printf("uint8_t read failed");
		abort();
	}
//		throw DZException("uint8_t read failed");
	return var;
}

uint16_t MemoryIO::readU16 () 
{
	uint16_t var;
	if (read(&var, sizeof(uint16_t)) != sizeof(uint16_t))
	{
		printf("uint16_t read failed");
		abort();
	}
//		throw DZException("uint16_t read failed");
	return var;
}

uint32_t MemoryIO::readU32 () 
{
	uint32_t var;
	if (read(&var, sizeof(uint32_t)) != sizeof(uint32_t))
	{
		printf("uint32_t read failed");
		abort();
	}
//		throw DZException("uint32_t read failed");
	return var;
}

uint64_t MemoryIO::readU64 () 
{
	uint64_t var;
	if (read(&var, sizeof(uint64_t)) != sizeof(uint64_t))
	{
		printf("uint64_t read failed");
		abort();
	}
//		throw DZException("uint64_t read failed");
	return var;
}

ssize_l_t MemoryIO::write (void const *buffer, size_t size) 
{ 
	//return fwrite(buffer, 1, size, fh); 
	assert(currentIOMethods == IOMethods::WRITE);
	fh_out.insert(fh_out.end(), (char*)buffer, (char*)buffer+size);
	fsize += size;
	return size;
}

ssize_l_t MemoryIO::tell ()
{
	assert(currentIOMethods == IOMethods::READ);
	return current_pos; //ftell(fh);
}

ssize_l_t MemoryIO::seek (size_t pos)
{
	assert(currentIOMethods == IOMethods::READ);
	//return fseek(fh, pos, SEEK_SET); //SEEK_SET	Beginning of file

	if (fsize >= pos)
	{
		if (pos < current_region.first || pos >= current_region.second)
		{
			current_pos = pos;
			if(fetchBuffer(pair<size_t, size_t>(current_pos, current_pos+min(buffer_size, fsize-current_pos))))
			{		return 0;
			}
			else
			{		return 1;
			}
		}
	}
	return -1;
}

size_t MemoryIO::size () 
{
	assert(currentIOMethods == IOMethods::READ);
	return fsize;
}

bool MemoryIO::eof () 
{ 
	assert(currentIOMethods == IOMethods::READ);
	return current_pos >= fsize; 
}

void *MemoryIO::handle ()
{
	return fh;
}


void MemoryIO::set(void * _fh)
{
	fh = (uint8_t *)_fh;
}

/*GzMemory_File::GzMemory_File (const string &path, const char *mode) 
{ 
open(path, mode); 
}

GzMemory_File::GzMemory_File (MEMORY_FILE *handle) 
{
fh = gzdopen(_fileno(handle), "rb");
if (!fh) throw DZException("Cannot open GZ file via handle");
}

GzMemory_File::~GzMemory_File () 
{
close();
}

void GzMemory_File::open (const string &path, const char *mode) 
{ 
fh = gzopen(path.c_str(), mode); 
if (!fh) throw DZException("Cannot open file %s", path.c_str());
}

void GzMemory_File::close () 
{ 
if (fh) gzclose(fh);
fh = 0; 
}

ssize_l_t GzMemory_File::read (void *buffer, size_t size) 
{ 
const size_t offset = 1 * (size_t)GB;
if (size > offset) {
return gzread(fh, buffer, offset) + read((char*)buffer + offset, size - offset); 
} else {
return gzread(fh, buffer, size);
}
}

ssize_l_t GzMemory_File::read(void *buffer, size_t size, size_t offset) 
{
throw DZException("GZ random access is not yet supported");
}

ssize_l_t GzMemory_File::write (void *buffer, size_t size) 
{ 
return gzwrite(fh, buffer, size); 
}

ssize_l_t GzMemory_File::tell ()
{
return gztell(fh);
}

ssize_l_t GzMemory_File::seek (size_t pos)
{
return gzseek(fh, pos, SEEK_SET);
}

size_t GzMemory_File::size () 
{
throw DZException("GZ file size is not supported");
}

bool GzMemory_File::eof () 
{ 
return gzeof(fh); 
}

void GzMemory_File::get_size () 
{
throw DZException("GZ file size is not supported");
}

void *GzMemory_File::handle ()
{
return fh;
} //*/
