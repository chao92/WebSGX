/*
 * attestation.h
 *
 *  Created on: Aug 16, 2016
 *      Author: weijiawu
 */

#ifndef ENCLAVE_DEEZENCLAVE_UNTRUSTED_HEADER_ATTESTATION_H_
#define ENCLAVE_DEEZENCLAVE_UNTRUSTED_HEADER_ATTESTATION_H_


#include "Socket.h"
#include "DeezEnclave_u.h"

// for webclient
//#include "Server.h"

// new code for webclient


int attestation(sgx_enclave_id_t enclave_id, sgx_ra_context_t *context, sgx_status_t status, Socket *S, int* socket_fd, int* client_id);

// new code for webclient
//int attestation_web(sgx_enclave_id_t enclave_id, sgx_ra_context_t *context, sgx_status_t status, Socket *S, S_C &S_client);
// end of new code for webclient

#endif /* ENCLAVE_DEEZENCLAVE_UNTRUSTED_HEADER_ATTESTATION_H_ */
