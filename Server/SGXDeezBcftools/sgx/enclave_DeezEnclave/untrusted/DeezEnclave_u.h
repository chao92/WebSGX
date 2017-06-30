#ifndef DEEZENCLAVE_U_H__
#define DEEZENCLAVE_U_H__

#include <stdint.h>
#include <wchar.h>
#include <stddef.h>
#include <string.h>
#include "sgx_edger8r.h" /* for sgx_satus_t etc. */

#include "sgx_key_exchange.h"
#include "sgx_trts.h"

#include <stdlib.h> /* for size_t */

#define SGX_CAST(type, item) ((type)(item))

#ifdef __cplusplus
extern "C" {
#endif

void SGX_UBRIDGE(SGX_NOCONVENTION, ocall_DeezEnclave_sample, (const char* str));
void SGX_UBRIDGE(SGX_NOCONVENTION, ocall_reference_getCharAt, (int pos, char* out));
void SGX_UBRIDGE(SGX_NOCONVENTION, ocall_reference_copy_trim, (size_t start, size_t end, char** out));
void SGX_UBRIDGE(SGX_NOCONVENTION, ocall_reference_scanChromosome, (const char* s, char** out));
void SGX_UBRIDGE(SGX_NOCONVENTION, ocall_print2file1, (const char* s, int fileID));
int SGX_UBRIDGE(SGX_NOCONVENTION, ocall_fetchBuffer, (uint8_t* fetched_buffers2unseal, size_t size_to_fetch, size_t start_position_of_the_first_seg2unseal));
void SGX_UBRIDGE(SGX_NOCONVENTION, ocall_print_string, (const char* str));
void SGX_UBRIDGE(SGX_NOCONVENTION, ocall_bcfenclave_sample, (const char* str));
int SGX_UBRIDGE(SGX_NOCONVENTION, ocall_hfile_oflags, (const char* mode));
int SGX_UBRIDGE(SGX_NOCONVENTION, ocall_open, (const char* filename, int mode));
int SGX_UBRIDGE(SGX_NOCONVENTION, ocall_read, (int file, void* buf, unsigned int size));
int SGX_UBRIDGE(SGX_NOCONVENTION, ocall_write, (int file, void* buf, unsigned int size));
int SGX_UBRIDGE(SGX_NOCONVENTION, ocall_close, (int file));
int SGX_UBRIDGE(SGX_NOCONVENTION, ocall_fsync, (int file));
sgx_status_t SGX_UBRIDGE(SGX_NOCONVENTION, create_session_ocall, (uint32_t* sid, uint8_t* dh_msg1, uint32_t dh_msg1_size, uint32_t timeout));
sgx_status_t SGX_UBRIDGE(SGX_NOCONVENTION, exchange_report_ocall, (uint32_t sid, uint8_t* dh_msg2, uint32_t dh_msg2_size, uint8_t* dh_msg3, uint32_t dh_msg3_size, uint32_t timeout));
sgx_status_t SGX_UBRIDGE(SGX_NOCONVENTION, close_session_ocall, (uint32_t sid, uint32_t timeout));
sgx_status_t SGX_UBRIDGE(SGX_NOCONVENTION, invoke_service_ocall, (uint8_t* pse_message_req, uint32_t pse_message_req_size, uint8_t* pse_message_resp, uint32_t pse_message_resp_size, uint32_t timeout));
void SGX_UBRIDGE(SGX_CDECL, sgx_oc_cpuidex, (int cpuinfo[4], int leaf, int subleaf));
int SGX_UBRIDGE(SGX_CDECL, sgx_thread_wait_untrusted_event_ocall, (const void* self));
int SGX_UBRIDGE(SGX_CDECL, sgx_thread_set_untrusted_event_ocall, (const void* waiter));
int SGX_UBRIDGE(SGX_CDECL, sgx_thread_setwait_untrusted_events_ocall, (const void* waiter, const void* self));
int SGX_UBRIDGE(SGX_CDECL, sgx_thread_set_multiple_untrusted_events_ocall, (const void** waiters, size_t total));

sgx_status_t ecall_DeezEnclave_sample(sgx_enclave_id_t eid, int* retval);
sgx_status_t enclave_init_ra(sgx_enclave_id_t eid, sgx_status_t* retval, int b_pse, int client_id, sgx_ra_context_t* p_context);
sgx_status_t enclave_ra_close(sgx_enclave_id_t eid, sgx_status_t* retval, sgx_ra_context_t context);
sgx_status_t verify_att_result_mac(sgx_enclave_id_t eid, sgx_status_t* retval, sgx_ra_context_t context, uint8_t* message, size_t message_size, uint8_t* mac, size_t mac_size);
sgx_status_t intializingEnclave(sgx_enclave_id_t eid, const char* chr0, int start0, int end0);
sgx_status_t initializeIV(sgx_enclave_id_t eid, int client_num);
sgx_status_t useEnclaveGetFileSize(sgx_enclave_id_t eid, sgx_ra_context_t context, char* data, int data_size, char* decryptedData, int decryptedData_size, int numberofParties, int client_i);
sgx_status_t encryptData(sgx_enclave_id_t eid, sgx_ra_context_t context, uint8_t* p_src, uint32_t src_len, uint8_t* p_dst, uint32_t dst_len);
sgx_status_t ecall_decryped_seal_buffer(sgx_enclave_id_t eid, sgx_ra_context_t context, char* data2seal, int data_buffer_size, char* sealed_secret, int sealed_size, int true_size, int64_t index, char* sealedDataMAC, int mac_size);
sgx_status_t ecallHere(sgx_enclave_id_t eid, size_t inFileSz, int numFiles, char* sealedDataMAC, int mac_size);
sgx_status_t calDist(sgx_enclave_id_t eid, double* dist);
sgx_status_t put_secret_data(sgx_enclave_id_t eid, sgx_status_t* retval, sgx_ra_context_t context, uint8_t* p_secret, uint32_t secret_size, uint8_t* gcm_mac);
sgx_status_t ecall_bcfenclave_sample(sgx_enclave_id_t eid, int* retval);
sgx_status_t sgx_ra_get_ga(sgx_enclave_id_t eid, sgx_status_t* retval, sgx_ra_context_t context, sgx_ec256_public_t* g_a);
sgx_status_t sgx_ra_proc_msg2_trusted(sgx_enclave_id_t eid, sgx_status_t* retval, sgx_ra_context_t context, const sgx_ra_msg2_t* p_msg2, const sgx_target_info_t* p_qe_target, sgx_report_t* p_report, sgx_quote_nonce_t* p_nonce);
sgx_status_t sgx_ra_get_msg3_trusted(sgx_enclave_id_t eid, sgx_status_t* retval, sgx_ra_context_t context, uint32_t quote_size, sgx_report_t* qe_report, sgx_ra_msg3_t* p_msg3, uint32_t msg3_size);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
