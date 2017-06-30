/*
 * Server.h
 *
 *  Created on: Aug 12, 2016
 *      Author: weijiawu
 */

#ifndef ENCLAVE_DEEZENCLAVE_UNTRUSTED_SERVER_H_
#define ENCLAVE_DEEZENCLAVE_UNTRUSTED_SERVER_H_

#include <sys/time.h>
//#include <iostream>
#include <stdio.h>
#include "Header/Socket.h"
#include <string>


#include <sgx_urts.h>
#include <sgx_key_exchange.h>

#include "Header/config.h"

#define _T(x) 				x
#define ENCLAVE_FILE		_T("DeezEnclave.signed.so")//TODO:change the path
#define CERTIFICATE_FILE 	_T("enclave_server.cert.pem")
#define PRIVATE_KEY_FILE 	_T("enclave_server.key.pem")

typedef struct socket_client_pair
{
	int socket_fd;
	int client_id;
	char username[128];
}S_C;

struct receive_data_parameter_struct
{
	S_C* s_c;
	Socket* socket;
};

typedef struct Genome_data
{
	int SNP_count;
	int case_count;
	int control_count;
	char *case_data;
	char *control_data;
}GNOMEDATA;

void decompressSealedDZ(string sealed_deez_file, size_t inFileSz, string optRef, int DZ_order, char* sealedDataMAC, sgx_enclave_id_t eid);
void sendBackMessage(Socket* serverSocket, int socketID, double dist, sgx_ra_context_t context, sgx_enclave_id_t eid);
typedef

class Server
{
public:
	Server(Config * client_information);
	virtual ~Server();

//	bool InitializeCommunication(void);
	void StartListening(void);
	void StartDataExchangeListening(void);
	bool StartCommunication(void);


	void TimeStart();
	long long TimeEnd();//Return milliseconds

private :
	Config* client_information;

	sgx_enclave_id_t eid;
	sgx_launch_token_t token;
	sgx_ra_context_t *context;
	sgx_status_t status = SGX_SUCCESS;

	int ret = SGX_SUCCESS;
	int updated = 0;

	Socket server_socket;

	// add for web client sync.
	Socket data_exchange_socket;
	// end of add for web client


	struct timeval start, end;

	static const string Certificate_File;
	static const string Private_Key_File;
};

#endif /* ENCLAVE_DEEZENCLAVE_UNTRUSTED_SERVER_H_ */
