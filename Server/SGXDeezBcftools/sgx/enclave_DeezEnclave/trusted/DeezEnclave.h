#ifndef _DEEZENCLAVE_H_
#define _DEEZENCLAVE_H_

#include <stdlib.h>
#include <assert.h>

#include "stdint.h"
#include "sgx_defs.h"
#include "sgx_tkey_exchange.h"
#include "sgx_tcrypto.h"
#include "sgx_trts.h"

#define NUM_OF_PARTIES 2
#define DNA_BASE_NUM 4
#define MAX_REFERENCE_NAME 256
#define MAX_CHROMESOME_RANGE_NAME_LENGTH 256
typedef short COUNT_DATA_TYPE;
typedef float HIST_DATA_TYPE;



#if defined(__cplusplus)
extern "C" {
#endif

sgx_status_t enclave_init_ra(int b_pse,	int client_id, sgx_ra_context_t *p_context);

sgx_status_t SGXAPI enclave_ra_close(sgx_ra_context_t context);

sgx_status_t verify_att_result_mac(sgx_ra_context_t context, uint8_t* p_message, size_t message_size, uint8_t* p_mac, size_t mac_size);

void intializingEnclave(const char *chr0, int start0, int end0);

void initializeIV(int client_num);

void useEnclaveGetFileSize(sgx_ra_context_t context, char* data, int size_in,
				 char*decryptedData, int decryptedData_size,
				 int numberofParties, int client_i);

void encryptData(sgx_ra_context_t context,
				 uint8_t *p_src, uint32_t src_len,
				 uint8_t *p_dst, uint32_t dst_len);

void ecall_decryped_seal_buffer(sgx_ra_context_t context, char* data2seal, int data_buffer_size, char* sealed_secret, int sealed_size,
								int true_size, int64_t index, char* sealedDataMAC, int MAC_size);

sgx_status_t getDecryptedBlock(sgx_ra_context_t context, const uint8_t *p_src, uint32_t src_len, uint8_t *p_dst);

void ecallHere(size_t inFileSz,  int deezOrder, char* sealedDataMAC, int mac_size);

void calDist(double *dist);

void printf(const char *fmt, ...);

void sprintf(char* buf, const char *fmt, ...);

int updatingCounts(char* chrome_name, uint8_t*buffer, uint32_t start, uint32_t end, COUNT_DATA_TYPE** cnt);

int updatingCount(COUNT_DATA_TYPE** cnt, const char* buffer, uint32_t pos_seq, uint32_t len, uint32_t start, uint32_t end);

int copyReferenceName1(uint8_t* buffer, char* name0);

int getACigar(uint8_t *buffer, char op, uint32_t count);

uint32_t getWordEndPos(uint8_t* buffer);

int nextWord(uint8_t* buffer);

int readFlag(uint8_t* buffer);

int updatingPosCount(COUNT_DATA_TYPE** cnt, int tmp, const char ch);

int getTheEndOfWord_1(uint8_t* buffer);

// bcftools 
char *strdup(const char *s);
int bam_mpileup(int argc, char *argv[]);

#if defined(__cplusplus)
}
#endif

#endif /* !_DEEZENCLAVE_H_ */
