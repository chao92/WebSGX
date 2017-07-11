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
#include "network_ra.h"

// for webclient
//#include "Server.h"

// new code for webclient


//int attestation(sgx_enclave_id_t enclave_id, sgx_ra_context_t *context, sgx_status_t status, Socket *S, int* socket_fd, int* client_id);

// new code for webclient
//int attestation_web(sgx_enclave_id_t enclave_id, sgx_ra_context_t *context, sgx_status_t status, Socket *S, S_C &S_client);
// end of new code for webclient

int attestation_sendMSG1(sgx_enclave_id_t enclave_id, sgx_ra_context_t *context, sgx_status_t status, Socket *S, int socket_fd, int client_id, ra_samp_request_header_t** pp_msg1_full);
int attestation_recvMSG2(sgx_enclave_id_t enclave_id, sgx_ra_context_t *context, sgx_status_t status, Socket *S, int socket_fd, int client_id, ra_samp_request_header_t** pp_msg3_full);
int attestation_sendMSG3(sgx_enclave_id_t enclave_id, sgx_ra_context_t *context, sgx_status_t status, Socket *S, int socket_fd, int client_id, ra_samp_request_header_t** pp_msg3_full);
int attestation_recvMSG4(sgx_enclave_id_t enclave_id, sgx_ra_context_t *context, sgx_status_t status, Socket *S, int socket_fd, int client_id, ra_samp_response_header_t** pp_msg4_full, int current_client_id);

int attestation_recvSAM(sgx_enclave_id_t enclave_id, sgx_ra_context_t *context, sgx_status_t status, Socket *S, int socket_fd, int client_id, uint8_t** pp_samFile, int current_client_id);
int attestation_recvREF(sgx_enclave_id_t enclave_id, sgx_ra_context_t *context, sgx_status_t status, Socket *S, int socket_fd, int client_id, uint8_t** pp_refFile, int current_client_id);
int attestation_recvINDEX(sgx_enclave_id_t enclave_id, sgx_ra_context_t *context, sgx_status_t status, Socket *S, int socket_fd, int client_id, uint8_t** pp_refIndexFile, int current_client_id);
#endif /* ENCLAVE_DEEZENCLAVE_UNTRUSTED_HEADER_ATTESTATION_H_ */
