#ifndef DATA_PROCESS_H
#define DATA_PROCESS_H
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <assert.h>
#include <math.h>
#include <ctime>
#include <cstring>
#include <iomanip>
#include <queue>

#include "sgx_urts.h" 

#include "Header/config.h"
#include "network_ra.h"
// Needed for definition of remote attestation messages.
#include "remote_attestation_result.h"

#include "pthread.h"
#include "ssl_server.h"
#include "Header/Reference.h"
#include "FileIO.h"
#include "common.h"
#include "sgx_key_exchange.h"

//File* sealedFile;
//File* inFile;
//extern Reference* reference;

using namespace std;

MESSAGE_LENGTH_TYPE getFileSize(sgx_ra_context_t context, char* fileBuffer,
										 int numberofParties, int client_i, sgx_enclave_id_t eid);

MESSAGE_LENGTH_TYPE getEncryptedFileSize(MESSAGE_LENGTH_TYPE fileSize);

void sealDZFile(sgx_ra_context_t context, char* data, MESSAGE_LENGTH_TYPE inputFileSize, string sealed_dz,
				int client_num, int i, char * sealedDataMAC, sgx_enclave_id_t eid);

void decompressSealedDZ(string sealed_deez_file, size_t inFileSz, string optRef, int DZ_order, sgx_enclave_id_t eid);

#endif
