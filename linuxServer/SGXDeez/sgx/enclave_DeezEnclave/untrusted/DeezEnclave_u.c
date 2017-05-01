#include "DeezEnclave_u.h"
#include <errno.h>

typedef struct ms_ecall_DeezEnclave_sample_t {
	int ms_retval;
} ms_ecall_DeezEnclave_sample_t;

typedef struct ms_enclave_init_ra_t {
	sgx_status_t ms_retval;
	int ms_b_pse;
	int ms_client_id;
	sgx_ra_context_t* ms_p_context;
} ms_enclave_init_ra_t;

typedef struct ms_enclave_ra_close_t {
	sgx_status_t ms_retval;
	sgx_ra_context_t ms_context;
} ms_enclave_ra_close_t;

typedef struct ms_verify_att_result_mac_t {
	sgx_status_t ms_retval;
	sgx_ra_context_t ms_context;
	uint8_t* ms_message;
	size_t ms_message_size;
	uint8_t* ms_mac;
	size_t ms_mac_size;
} ms_verify_att_result_mac_t;

typedef struct ms_intializingEnclave_t {
	char* ms_chr0;
	int ms_start0;
	int ms_end0;
} ms_intializingEnclave_t;

typedef struct ms_initializeIV_t {
	int ms_client_num;
} ms_initializeIV_t;

typedef struct ms_useEnclaveGetFileSize_t {
	sgx_ra_context_t ms_context;
	char* ms_data;
	int ms_data_size;
	char* ms_decryptedData;
	int ms_decryptedData_size;
	int ms_numberofParties;
	int ms_client_i;
} ms_useEnclaveGetFileSize_t;

typedef struct ms_encryptData_t {
	sgx_ra_context_t ms_context;
	uint8_t* ms_p_src;
	uint32_t ms_src_len;
	uint8_t* ms_p_dst;
	uint32_t ms_dst_len;
} ms_encryptData_t;

typedef struct ms_ecall_decryped_seal_buffer_t {
	sgx_ra_context_t ms_context;
	char* ms_data2seal;
	int ms_data_buffer_size;
	char* ms_sealed_secret;
	int ms_sealed_size;
	int ms_true_size;
	int64_t ms_index;
	char* ms_sealedDataMAC;
	int ms_mac_size;
} ms_ecall_decryped_seal_buffer_t;

typedef struct ms_ecallHere_t {
	size_t ms_inFileSz;
	int ms_numFiles;
	char* ms_sealedDataMAC;
	int ms_mac_size;
} ms_ecallHere_t;

typedef struct ms_calDist_t {
	double* ms_dist;
} ms_calDist_t;

typedef struct ms_put_secret_data_t {
	sgx_status_t ms_retval;
	sgx_ra_context_t ms_context;
	uint8_t* ms_p_secret;
	uint32_t ms_secret_size;
	uint8_t* ms_gcm_mac;
} ms_put_secret_data_t;

typedef struct ms_sgx_ra_get_ga_t {
	sgx_status_t ms_retval;
	sgx_ra_context_t ms_context;
	sgx_ec256_public_t* ms_g_a;
} ms_sgx_ra_get_ga_t;

typedef struct ms_sgx_ra_proc_msg2_trusted_t {
	sgx_status_t ms_retval;
	sgx_ra_context_t ms_context;
	sgx_ra_msg2_t* ms_p_msg2;
	sgx_target_info_t* ms_p_qe_target;
	sgx_report_t* ms_p_report;
	sgx_quote_nonce_t* ms_p_nonce;
} ms_sgx_ra_proc_msg2_trusted_t;

typedef struct ms_sgx_ra_get_msg3_trusted_t {
	sgx_status_t ms_retval;
	sgx_ra_context_t ms_context;
	uint32_t ms_quote_size;
	sgx_report_t* ms_qe_report;
	sgx_ra_msg3_t* ms_p_msg3;
	uint32_t ms_msg3_size;
} ms_sgx_ra_get_msg3_trusted_t;

typedef struct ms_ocall_DeezEnclave_sample_t {
	char* ms_str;
} ms_ocall_DeezEnclave_sample_t;

typedef struct ms_ocall_reference_getCharAt_t {
	int ms_pos;
	char* ms_out;
} ms_ocall_reference_getCharAt_t;

typedef struct ms_ocall_reference_copy_trim_t {
	size_t ms_start;
	size_t ms_end;
	char** ms_out;
} ms_ocall_reference_copy_trim_t;

typedef struct ms_ocall_reference_scanChromosome_t {
	char* ms_s;
	char** ms_out;
} ms_ocall_reference_scanChromosome_t;

typedef struct ms_ocall_print2file1_t {
	char* ms_s;
	int ms_fileID;
} ms_ocall_print2file1_t;

typedef struct ms_ocall_fetchBuffer_t {
	int ms_retval;
	uint8_t* ms_fetched_buffers2unseal;
	size_t ms_size_to_fetch;
	size_t ms_start_position_of_the_first_seg2unseal;
} ms_ocall_fetchBuffer_t;

typedef struct ms_ocall_print_string_t {
	char* ms_str;
} ms_ocall_print_string_t;

typedef struct ms_create_session_ocall_t {
	sgx_status_t ms_retval;
	uint32_t* ms_sid;
	uint8_t* ms_dh_msg1;
	uint32_t ms_dh_msg1_size;
	uint32_t ms_timeout;
} ms_create_session_ocall_t;

typedef struct ms_exchange_report_ocall_t {
	sgx_status_t ms_retval;
	uint32_t ms_sid;
	uint8_t* ms_dh_msg2;
	uint32_t ms_dh_msg2_size;
	uint8_t* ms_dh_msg3;
	uint32_t ms_dh_msg3_size;
	uint32_t ms_timeout;
} ms_exchange_report_ocall_t;

typedef struct ms_close_session_ocall_t {
	sgx_status_t ms_retval;
	uint32_t ms_sid;
	uint32_t ms_timeout;
} ms_close_session_ocall_t;

typedef struct ms_invoke_service_ocall_t {
	sgx_status_t ms_retval;
	uint8_t* ms_pse_message_req;
	uint32_t ms_pse_message_req_size;
	uint8_t* ms_pse_message_resp;
	uint32_t ms_pse_message_resp_size;
	uint32_t ms_timeout;
} ms_invoke_service_ocall_t;

typedef struct ms_sgx_oc_cpuidex_t {
	int* ms_cpuinfo;
	int ms_leaf;
	int ms_subleaf;
} ms_sgx_oc_cpuidex_t;

typedef struct ms_sgx_thread_wait_untrusted_event_ocall_t {
	int ms_retval;
	void* ms_self;
} ms_sgx_thread_wait_untrusted_event_ocall_t;

typedef struct ms_sgx_thread_set_untrusted_event_ocall_t {
	int ms_retval;
	void* ms_waiter;
} ms_sgx_thread_set_untrusted_event_ocall_t;

typedef struct ms_sgx_thread_setwait_untrusted_events_ocall_t {
	int ms_retval;
	void* ms_waiter;
	void* ms_self;
} ms_sgx_thread_setwait_untrusted_events_ocall_t;

typedef struct ms_sgx_thread_set_multiple_untrusted_events_ocall_t {
	int ms_retval;
	void** ms_waiters;
	size_t ms_total;
} ms_sgx_thread_set_multiple_untrusted_events_ocall_t;

static sgx_status_t SGX_CDECL DeezEnclave_ocall_DeezEnclave_sample(void* pms)
{
	ms_ocall_DeezEnclave_sample_t* ms = SGX_CAST(ms_ocall_DeezEnclave_sample_t*, pms);
	ocall_DeezEnclave_sample((const char*)ms->ms_str);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL DeezEnclave_ocall_reference_getCharAt(void* pms)
{
	ms_ocall_reference_getCharAt_t* ms = SGX_CAST(ms_ocall_reference_getCharAt_t*, pms);
	ocall_reference_getCharAt(ms->ms_pos, ms->ms_out);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL DeezEnclave_ocall_reference_copy_trim(void* pms)
{
	ms_ocall_reference_copy_trim_t* ms = SGX_CAST(ms_ocall_reference_copy_trim_t*, pms);
	ocall_reference_copy_trim(ms->ms_start, ms->ms_end, ms->ms_out);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL DeezEnclave_ocall_reference_scanChromosome(void* pms)
{
	ms_ocall_reference_scanChromosome_t* ms = SGX_CAST(ms_ocall_reference_scanChromosome_t*, pms);
	ocall_reference_scanChromosome((const char*)ms->ms_s, ms->ms_out);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL DeezEnclave_ocall_print2file1(void* pms)
{
	ms_ocall_print2file1_t* ms = SGX_CAST(ms_ocall_print2file1_t*, pms);
	ocall_print2file1((const char*)ms->ms_s, ms->ms_fileID);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL DeezEnclave_ocall_fetchBuffer(void* pms)
{
	ms_ocall_fetchBuffer_t* ms = SGX_CAST(ms_ocall_fetchBuffer_t*, pms);
	ms->ms_retval = ocall_fetchBuffer(ms->ms_fetched_buffers2unseal, ms->ms_size_to_fetch, ms->ms_start_position_of_the_first_seg2unseal);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL DeezEnclave_ocall_print_string(void* pms)
{
	ms_ocall_print_string_t* ms = SGX_CAST(ms_ocall_print_string_t*, pms);
	ocall_print_string((const char*)ms->ms_str);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL DeezEnclave_create_session_ocall(void* pms)
{
	ms_create_session_ocall_t* ms = SGX_CAST(ms_create_session_ocall_t*, pms);
	ms->ms_retval = create_session_ocall(ms->ms_sid, ms->ms_dh_msg1, ms->ms_dh_msg1_size, ms->ms_timeout);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL DeezEnclave_exchange_report_ocall(void* pms)
{
	ms_exchange_report_ocall_t* ms = SGX_CAST(ms_exchange_report_ocall_t*, pms);
	ms->ms_retval = exchange_report_ocall(ms->ms_sid, ms->ms_dh_msg2, ms->ms_dh_msg2_size, ms->ms_dh_msg3, ms->ms_dh_msg3_size, ms->ms_timeout);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL DeezEnclave_close_session_ocall(void* pms)
{
	ms_close_session_ocall_t* ms = SGX_CAST(ms_close_session_ocall_t*, pms);
	ms->ms_retval = close_session_ocall(ms->ms_sid, ms->ms_timeout);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL DeezEnclave_invoke_service_ocall(void* pms)
{
	ms_invoke_service_ocall_t* ms = SGX_CAST(ms_invoke_service_ocall_t*, pms);
	ms->ms_retval = invoke_service_ocall(ms->ms_pse_message_req, ms->ms_pse_message_req_size, ms->ms_pse_message_resp, ms->ms_pse_message_resp_size, ms->ms_timeout);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL DeezEnclave_sgx_oc_cpuidex(void* pms)
{
	ms_sgx_oc_cpuidex_t* ms = SGX_CAST(ms_sgx_oc_cpuidex_t*, pms);
	sgx_oc_cpuidex(ms->ms_cpuinfo, ms->ms_leaf, ms->ms_subleaf);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL DeezEnclave_sgx_thread_wait_untrusted_event_ocall(void* pms)
{
	ms_sgx_thread_wait_untrusted_event_ocall_t* ms = SGX_CAST(ms_sgx_thread_wait_untrusted_event_ocall_t*, pms);
	ms->ms_retval = sgx_thread_wait_untrusted_event_ocall((const void*)ms->ms_self);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL DeezEnclave_sgx_thread_set_untrusted_event_ocall(void* pms)
{
	ms_sgx_thread_set_untrusted_event_ocall_t* ms = SGX_CAST(ms_sgx_thread_set_untrusted_event_ocall_t*, pms);
	ms->ms_retval = sgx_thread_set_untrusted_event_ocall((const void*)ms->ms_waiter);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL DeezEnclave_sgx_thread_setwait_untrusted_events_ocall(void* pms)
{
	ms_sgx_thread_setwait_untrusted_events_ocall_t* ms = SGX_CAST(ms_sgx_thread_setwait_untrusted_events_ocall_t*, pms);
	ms->ms_retval = sgx_thread_setwait_untrusted_events_ocall((const void*)ms->ms_waiter, (const void*)ms->ms_self);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL DeezEnclave_sgx_thread_set_multiple_untrusted_events_ocall(void* pms)
{
	ms_sgx_thread_set_multiple_untrusted_events_ocall_t* ms = SGX_CAST(ms_sgx_thread_set_multiple_untrusted_events_ocall_t*, pms);
	ms->ms_retval = sgx_thread_set_multiple_untrusted_events_ocall((const void**)ms->ms_waiters, ms->ms_total);

	return SGX_SUCCESS;
}

static const struct {
	size_t nr_ocall;
	void * table[16];
} ocall_table_DeezEnclave = {
	16,
	{
		(void*)DeezEnclave_ocall_DeezEnclave_sample,
		(void*)DeezEnclave_ocall_reference_getCharAt,
		(void*)DeezEnclave_ocall_reference_copy_trim,
		(void*)DeezEnclave_ocall_reference_scanChromosome,
		(void*)DeezEnclave_ocall_print2file1,
		(void*)DeezEnclave_ocall_fetchBuffer,
		(void*)DeezEnclave_ocall_print_string,
		(void*)DeezEnclave_create_session_ocall,
		(void*)DeezEnclave_exchange_report_ocall,
		(void*)DeezEnclave_close_session_ocall,
		(void*)DeezEnclave_invoke_service_ocall,
		(void*)DeezEnclave_sgx_oc_cpuidex,
		(void*)DeezEnclave_sgx_thread_wait_untrusted_event_ocall,
		(void*)DeezEnclave_sgx_thread_set_untrusted_event_ocall,
		(void*)DeezEnclave_sgx_thread_setwait_untrusted_events_ocall,
		(void*)DeezEnclave_sgx_thread_set_multiple_untrusted_events_ocall,
	}
};
sgx_status_t ecall_DeezEnclave_sample(sgx_enclave_id_t eid, int* retval)
{
	sgx_status_t status;
	ms_ecall_DeezEnclave_sample_t ms;
	status = sgx_ecall(eid, 0, &ocall_table_DeezEnclave, &ms);
	if (status == SGX_SUCCESS && retval) *retval = ms.ms_retval;
	return status;
}

sgx_status_t enclave_init_ra(sgx_enclave_id_t eid, sgx_status_t* retval, int b_pse, int client_id, sgx_ra_context_t* p_context)
{
	sgx_status_t status;
	ms_enclave_init_ra_t ms;
	ms.ms_b_pse = b_pse;
	ms.ms_client_id = client_id;
	ms.ms_p_context = p_context;
	status = sgx_ecall(eid, 1, &ocall_table_DeezEnclave, &ms);
	if (status == SGX_SUCCESS && retval) *retval = ms.ms_retval;
	return status;
}

sgx_status_t enclave_ra_close(sgx_enclave_id_t eid, sgx_status_t* retval, sgx_ra_context_t context)
{
	sgx_status_t status;
	ms_enclave_ra_close_t ms;
	ms.ms_context = context;
	status = sgx_ecall(eid, 2, &ocall_table_DeezEnclave, &ms);
	if (status == SGX_SUCCESS && retval) *retval = ms.ms_retval;
	return status;
}

sgx_status_t verify_att_result_mac(sgx_enclave_id_t eid, sgx_status_t* retval, sgx_ra_context_t context, uint8_t* message, size_t message_size, uint8_t* mac, size_t mac_size)
{
	sgx_status_t status;
	ms_verify_att_result_mac_t ms;
	ms.ms_context = context;
	ms.ms_message = message;
	ms.ms_message_size = message_size;
	ms.ms_mac = mac;
	ms.ms_mac_size = mac_size;
	status = sgx_ecall(eid, 3, &ocall_table_DeezEnclave, &ms);
	if (status == SGX_SUCCESS && retval) *retval = ms.ms_retval;
	return status;
}

sgx_status_t intializingEnclave(sgx_enclave_id_t eid, const char* chr0, int start0, int end0)
{
	sgx_status_t status;
	ms_intializingEnclave_t ms;
	ms.ms_chr0 = (char*)chr0;
	ms.ms_start0 = start0;
	ms.ms_end0 = end0;
	status = sgx_ecall(eid, 4, &ocall_table_DeezEnclave, &ms);
	return status;
}

sgx_status_t initializeIV(sgx_enclave_id_t eid, int client_num)
{
	sgx_status_t status;
	ms_initializeIV_t ms;
	ms.ms_client_num = client_num;
	status = sgx_ecall(eid, 5, &ocall_table_DeezEnclave, &ms);
	return status;
}

sgx_status_t useEnclaveGetFileSize(sgx_enclave_id_t eid, sgx_ra_context_t context, char* data, int data_size, char* decryptedData, int decryptedData_size, int numberofParties, int client_i)
{
	sgx_status_t status;
	ms_useEnclaveGetFileSize_t ms;
	ms.ms_context = context;
	ms.ms_data = data;
	ms.ms_data_size = data_size;
	ms.ms_decryptedData = decryptedData;
	ms.ms_decryptedData_size = decryptedData_size;
	ms.ms_numberofParties = numberofParties;
	ms.ms_client_i = client_i;
	status = sgx_ecall(eid, 6, &ocall_table_DeezEnclave, &ms);
	return status;
}

sgx_status_t encryptData(sgx_enclave_id_t eid, sgx_ra_context_t context, uint8_t* p_src, uint32_t src_len, uint8_t* p_dst, uint32_t dst_len)
{
	sgx_status_t status;
	ms_encryptData_t ms;
	ms.ms_context = context;
	ms.ms_p_src = p_src;
	ms.ms_src_len = src_len;
	ms.ms_p_dst = p_dst;
	ms.ms_dst_len = dst_len;
	status = sgx_ecall(eid, 7, &ocall_table_DeezEnclave, &ms);
	return status;
}

sgx_status_t ecall_decryped_seal_buffer(sgx_enclave_id_t eid, sgx_ra_context_t context, char* data2seal, int data_buffer_size, char* sealed_secret, int sealed_size, int true_size, int64_t index, char* sealedDataMAC, int mac_size)
{
	sgx_status_t status;
	ms_ecall_decryped_seal_buffer_t ms;
	ms.ms_context = context;
	ms.ms_data2seal = data2seal;
	ms.ms_data_buffer_size = data_buffer_size;
	ms.ms_sealed_secret = sealed_secret;
	ms.ms_sealed_size = sealed_size;
	ms.ms_true_size = true_size;
	ms.ms_index = index;
	ms.ms_sealedDataMAC = sealedDataMAC;
	ms.ms_mac_size = mac_size;
	status = sgx_ecall(eid, 8, &ocall_table_DeezEnclave, &ms);
	return status;
}

sgx_status_t ecallHere(sgx_enclave_id_t eid, size_t inFileSz, int numFiles, char* sealedDataMAC, int mac_size)
{
	sgx_status_t status;
	ms_ecallHere_t ms;
	ms.ms_inFileSz = inFileSz;
	ms.ms_numFiles = numFiles;
	ms.ms_sealedDataMAC = sealedDataMAC;
	ms.ms_mac_size = mac_size;
	status = sgx_ecall(eid, 9, &ocall_table_DeezEnclave, &ms);
	return status;
}

sgx_status_t calDist(sgx_enclave_id_t eid, double* dist)
{
	sgx_status_t status;
	ms_calDist_t ms;
	ms.ms_dist = dist;
	status = sgx_ecall(eid, 10, &ocall_table_DeezEnclave, &ms);
	return status;
}

sgx_status_t put_secret_data(sgx_enclave_id_t eid, sgx_status_t* retval, sgx_ra_context_t context, uint8_t* p_secret, uint32_t secret_size, uint8_t* gcm_mac)
{
	sgx_status_t status;
	ms_put_secret_data_t ms;
	ms.ms_context = context;
	ms.ms_p_secret = p_secret;
	ms.ms_secret_size = secret_size;
	ms.ms_gcm_mac = gcm_mac;
	status = sgx_ecall(eid, 11, &ocall_table_DeezEnclave, &ms);
	if (status == SGX_SUCCESS && retval) *retval = ms.ms_retval;
	return status;
}

sgx_status_t sgx_ra_get_ga(sgx_enclave_id_t eid, sgx_status_t* retval, sgx_ra_context_t context, sgx_ec256_public_t* g_a)
{
	sgx_status_t status;
	ms_sgx_ra_get_ga_t ms;
	ms.ms_context = context;
	ms.ms_g_a = g_a;
	status = sgx_ecall(eid, 12, &ocall_table_DeezEnclave, &ms);
	if (status == SGX_SUCCESS && retval) *retval = ms.ms_retval;
	return status;
}

sgx_status_t sgx_ra_proc_msg2_trusted(sgx_enclave_id_t eid, sgx_status_t* retval, sgx_ra_context_t context, const sgx_ra_msg2_t* p_msg2, const sgx_target_info_t* p_qe_target, sgx_report_t* p_report, sgx_quote_nonce_t* p_nonce)
{
	sgx_status_t status;
	ms_sgx_ra_proc_msg2_trusted_t ms;
	ms.ms_context = context;
	ms.ms_p_msg2 = (sgx_ra_msg2_t*)p_msg2;
	ms.ms_p_qe_target = (sgx_target_info_t*)p_qe_target;
	ms.ms_p_report = p_report;
	ms.ms_p_nonce = p_nonce;
	status = sgx_ecall(eid, 13, &ocall_table_DeezEnclave, &ms);
	if (status == SGX_SUCCESS && retval) *retval = ms.ms_retval;
	return status;
}

sgx_status_t sgx_ra_get_msg3_trusted(sgx_enclave_id_t eid, sgx_status_t* retval, sgx_ra_context_t context, uint32_t quote_size, sgx_report_t* qe_report, sgx_ra_msg3_t* p_msg3, uint32_t msg3_size)
{
	sgx_status_t status;
	ms_sgx_ra_get_msg3_trusted_t ms;
	ms.ms_context = context;
	ms.ms_quote_size = quote_size;
	ms.ms_qe_report = qe_report;
	ms.ms_p_msg3 = p_msg3;
	ms.ms_msg3_size = msg3_size;
	status = sgx_ecall(eid, 14, &ocall_table_DeezEnclave, &ms);
	if (status == SGX_SUCCESS && retval) *retval = ms.ms_retval;
	return status;
}

