#include "DeezEnclave_t.h"

#include "sgx_trts.h" /* for sgx_ocalloc, sgx_is_outside_enclave */

#include <errno.h>
#include <string.h> /* for memcpy etc */
#include <stdlib.h> /* for malloc/free etc */

#define CHECK_REF_POINTER(ptr, siz) do {	\
	if (!(ptr) || ! sgx_is_outside_enclave((ptr), (siz)))	\
		return SGX_ERROR_INVALID_PARAMETER;\
} while (0)

#define CHECK_UNIQUE_POINTER(ptr, siz) do {	\
	if ((ptr) && ! sgx_is_outside_enclave((ptr), (siz)))	\
		return SGX_ERROR_INVALID_PARAMETER;\
} while (0)


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

static sgx_status_t SGX_CDECL sgx_ecall_DeezEnclave_sample(void* pms)
{
	ms_ecall_DeezEnclave_sample_t* ms = SGX_CAST(ms_ecall_DeezEnclave_sample_t*, pms);
	sgx_status_t status = SGX_SUCCESS;

	CHECK_REF_POINTER(pms, sizeof(ms_ecall_DeezEnclave_sample_t));

	ms->ms_retval = ecall_DeezEnclave_sample();


	return status;
}

static sgx_status_t SGX_CDECL sgx_enclave_init_ra(void* pms)
{
	ms_enclave_init_ra_t* ms = SGX_CAST(ms_enclave_init_ra_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	sgx_ra_context_t* _tmp_p_context = ms->ms_p_context;
	size_t _len_p_context = sizeof(*_tmp_p_context);
	sgx_ra_context_t* _in_p_context = NULL;

	CHECK_REF_POINTER(pms, sizeof(ms_enclave_init_ra_t));
	CHECK_UNIQUE_POINTER(_tmp_p_context, _len_p_context);

	if (_tmp_p_context != NULL) {
		if ((_in_p_context = (sgx_ra_context_t*)malloc(_len_p_context)) == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memset((void*)_in_p_context, 0, _len_p_context);
	}
	ms->ms_retval = enclave_init_ra(ms->ms_b_pse, ms->ms_client_id, _in_p_context);
err:
	if (_in_p_context) {
		memcpy(_tmp_p_context, _in_p_context, _len_p_context);
		free(_in_p_context);
	}

	return status;
}

static sgx_status_t SGX_CDECL sgx_enclave_ra_close(void* pms)
{
	ms_enclave_ra_close_t* ms = SGX_CAST(ms_enclave_ra_close_t*, pms);
	sgx_status_t status = SGX_SUCCESS;

	CHECK_REF_POINTER(pms, sizeof(ms_enclave_ra_close_t));

	ms->ms_retval = enclave_ra_close(ms->ms_context);


	return status;
}

static sgx_status_t SGX_CDECL sgx_verify_att_result_mac(void* pms)
{
	ms_verify_att_result_mac_t* ms = SGX_CAST(ms_verify_att_result_mac_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	uint8_t* _tmp_message = ms->ms_message;
	size_t _tmp_message_size = ms->ms_message_size;
	size_t _len_message = _tmp_message_size;
	uint8_t* _in_message = NULL;
	uint8_t* _tmp_mac = ms->ms_mac;
	size_t _tmp_mac_size = ms->ms_mac_size;
	size_t _len_mac = _tmp_mac_size;
	uint8_t* _in_mac = NULL;

	CHECK_REF_POINTER(pms, sizeof(ms_verify_att_result_mac_t));
	CHECK_UNIQUE_POINTER(_tmp_message, _len_message);
	CHECK_UNIQUE_POINTER(_tmp_mac, _len_mac);

	if (_tmp_message != NULL) {
		_in_message = (uint8_t*)malloc(_len_message);
		if (_in_message == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memcpy(_in_message, _tmp_message, _len_message);
	}
	if (_tmp_mac != NULL) {
		_in_mac = (uint8_t*)malloc(_len_mac);
		if (_in_mac == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memcpy(_in_mac, _tmp_mac, _len_mac);
	}
	ms->ms_retval = verify_att_result_mac(ms->ms_context, _in_message, _tmp_message_size, _in_mac, _tmp_mac_size);
err:
	if (_in_message) free(_in_message);
	if (_in_mac) free(_in_mac);

	return status;
}

static sgx_status_t SGX_CDECL sgx_intializingEnclave(void* pms)
{
	ms_intializingEnclave_t* ms = SGX_CAST(ms_intializingEnclave_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	char* _tmp_chr0 = ms->ms_chr0;
	size_t _len_chr0 = _tmp_chr0 ? strlen(_tmp_chr0) + 1 : 0;
	char* _in_chr0 = NULL;

	CHECK_REF_POINTER(pms, sizeof(ms_intializingEnclave_t));
	CHECK_UNIQUE_POINTER(_tmp_chr0, _len_chr0);

	if (_tmp_chr0 != NULL) {
		_in_chr0 = (char*)malloc(_len_chr0);
		if (_in_chr0 == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memcpy((void*)_in_chr0, _tmp_chr0, _len_chr0);
		_in_chr0[_len_chr0 - 1] = '\0';
	}
	intializingEnclave((const char*)_in_chr0, ms->ms_start0, ms->ms_end0);
err:
	if (_in_chr0) free((void*)_in_chr0);

	return status;
}

static sgx_status_t SGX_CDECL sgx_initializeIV(void* pms)
{
	ms_initializeIV_t* ms = SGX_CAST(ms_initializeIV_t*, pms);
	sgx_status_t status = SGX_SUCCESS;

	CHECK_REF_POINTER(pms, sizeof(ms_initializeIV_t));

	initializeIV(ms->ms_client_num);


	return status;
}

static sgx_status_t SGX_CDECL sgx_useEnclaveGetFileSize(void* pms)
{
	ms_useEnclaveGetFileSize_t* ms = SGX_CAST(ms_useEnclaveGetFileSize_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	char* _tmp_data = ms->ms_data;
	int _tmp_data_size = ms->ms_data_size;
	size_t _len_data = _tmp_data_size;
	char* _in_data = NULL;
	char* _tmp_decryptedData = ms->ms_decryptedData;
	int _tmp_decryptedData_size = ms->ms_decryptedData_size;
	size_t _len_decryptedData = _tmp_decryptedData_size;
	char* _in_decryptedData = NULL;

	CHECK_REF_POINTER(pms, sizeof(ms_useEnclaveGetFileSize_t));
	CHECK_UNIQUE_POINTER(_tmp_data, _len_data);
	CHECK_UNIQUE_POINTER(_tmp_decryptedData, _len_decryptedData);

	if (_tmp_data != NULL) {
		_in_data = (char*)malloc(_len_data);
		if (_in_data == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memcpy(_in_data, _tmp_data, _len_data);
	}
	if (_tmp_decryptedData != NULL) {
		if ((_in_decryptedData = (char*)malloc(_len_decryptedData)) == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memset((void*)_in_decryptedData, 0, _len_decryptedData);
	}
	useEnclaveGetFileSize(ms->ms_context, _in_data, _tmp_data_size, _in_decryptedData, _tmp_decryptedData_size, ms->ms_numberofParties, ms->ms_client_i);
err:
	if (_in_data) free(_in_data);
	if (_in_decryptedData) {
		memcpy(_tmp_decryptedData, _in_decryptedData, _len_decryptedData);
		free(_in_decryptedData);
	}

	return status;
}

static sgx_status_t SGX_CDECL sgx_encryptData(void* pms)
{
	ms_encryptData_t* ms = SGX_CAST(ms_encryptData_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	uint8_t* _tmp_p_src = ms->ms_p_src;
	uint32_t _tmp_src_len = ms->ms_src_len;
	size_t _len_p_src = _tmp_src_len;
	uint8_t* _in_p_src = NULL;
	uint8_t* _tmp_p_dst = ms->ms_p_dst;
	uint32_t _tmp_dst_len = ms->ms_dst_len;
	size_t _len_p_dst = _tmp_dst_len;
	uint8_t* _in_p_dst = NULL;

	CHECK_REF_POINTER(pms, sizeof(ms_encryptData_t));
	CHECK_UNIQUE_POINTER(_tmp_p_src, _len_p_src);
	CHECK_UNIQUE_POINTER(_tmp_p_dst, _len_p_dst);

	if (_tmp_p_src != NULL) {
		_in_p_src = (uint8_t*)malloc(_len_p_src);
		if (_in_p_src == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memcpy(_in_p_src, _tmp_p_src, _len_p_src);
	}
	if (_tmp_p_dst != NULL) {
		if ((_in_p_dst = (uint8_t*)malloc(_len_p_dst)) == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memset((void*)_in_p_dst, 0, _len_p_dst);
	}
	encryptData(ms->ms_context, _in_p_src, _tmp_src_len, _in_p_dst, _tmp_dst_len);
err:
	if (_in_p_src) free(_in_p_src);
	if (_in_p_dst) {
		memcpy(_tmp_p_dst, _in_p_dst, _len_p_dst);
		free(_in_p_dst);
	}

	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_decryped_seal_buffer(void* pms)
{
	ms_ecall_decryped_seal_buffer_t* ms = SGX_CAST(ms_ecall_decryped_seal_buffer_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	char* _tmp_data2seal = ms->ms_data2seal;
	int _tmp_data_buffer_size = ms->ms_data_buffer_size;
	size_t _len_data2seal = _tmp_data_buffer_size;
	char* _in_data2seal = NULL;
	char* _tmp_sealed_secret = ms->ms_sealed_secret;
	int _tmp_sealed_size = ms->ms_sealed_size;
	size_t _len_sealed_secret = _tmp_sealed_size;
	char* _in_sealed_secret = NULL;
	char* _tmp_sealedDataMAC = ms->ms_sealedDataMAC;
	int _tmp_mac_size = ms->ms_mac_size;
	size_t _len_sealedDataMAC = _tmp_mac_size;
	char* _in_sealedDataMAC = NULL;

	CHECK_REF_POINTER(pms, sizeof(ms_ecall_decryped_seal_buffer_t));
	CHECK_UNIQUE_POINTER(_tmp_data2seal, _len_data2seal);
	CHECK_UNIQUE_POINTER(_tmp_sealed_secret, _len_sealed_secret);
	CHECK_UNIQUE_POINTER(_tmp_sealedDataMAC, _len_sealedDataMAC);

	if (_tmp_data2seal != NULL) {
		_in_data2seal = (char*)malloc(_len_data2seal);
		if (_in_data2seal == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memcpy(_in_data2seal, _tmp_data2seal, _len_data2seal);
	}
	if (_tmp_sealed_secret != NULL) {
		if ((_in_sealed_secret = (char*)malloc(_len_sealed_secret)) == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memset((void*)_in_sealed_secret, 0, _len_sealed_secret);
	}
	if (_tmp_sealedDataMAC != NULL) {
		_in_sealedDataMAC = (char*)malloc(_len_sealedDataMAC);
		if (_in_sealedDataMAC == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memcpy(_in_sealedDataMAC, _tmp_sealedDataMAC, _len_sealedDataMAC);
	}
	ecall_decryped_seal_buffer(ms->ms_context, _in_data2seal, _tmp_data_buffer_size, _in_sealed_secret, _tmp_sealed_size, ms->ms_true_size, ms->ms_index, _in_sealedDataMAC, _tmp_mac_size);
err:
	if (_in_data2seal) free(_in_data2seal);
	if (_in_sealed_secret) {
		memcpy(_tmp_sealed_secret, _in_sealed_secret, _len_sealed_secret);
		free(_in_sealed_secret);
	}
	if (_in_sealedDataMAC) free(_in_sealedDataMAC);

	return status;
}

static sgx_status_t SGX_CDECL sgx_ecallHere(void* pms)
{
	ms_ecallHere_t* ms = SGX_CAST(ms_ecallHere_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	char* _tmp_sealedDataMAC = ms->ms_sealedDataMAC;
	int _tmp_mac_size = ms->ms_mac_size;
	size_t _len_sealedDataMAC = _tmp_mac_size;
	char* _in_sealedDataMAC = NULL;

	CHECK_REF_POINTER(pms, sizeof(ms_ecallHere_t));
	CHECK_UNIQUE_POINTER(_tmp_sealedDataMAC, _len_sealedDataMAC);

	if (_tmp_sealedDataMAC != NULL) {
		_in_sealedDataMAC = (char*)malloc(_len_sealedDataMAC);
		if (_in_sealedDataMAC == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memcpy(_in_sealedDataMAC, _tmp_sealedDataMAC, _len_sealedDataMAC);
	}
	ecallHere(ms->ms_inFileSz, ms->ms_numFiles, _in_sealedDataMAC, _tmp_mac_size);
err:
	if (_in_sealedDataMAC) free(_in_sealedDataMAC);

	return status;
}

static sgx_status_t SGX_CDECL sgx_calDist(void* pms)
{
	ms_calDist_t* ms = SGX_CAST(ms_calDist_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	double* _tmp_dist = ms->ms_dist;
	size_t _len_dist = 1 * sizeof(*_tmp_dist);
	double* _in_dist = NULL;

	if (1 > (SIZE_MAX / sizeof(*_tmp_dist))) {
		status = SGX_ERROR_INVALID_PARAMETER;
		goto err;
	}

	CHECK_REF_POINTER(pms, sizeof(ms_calDist_t));
	CHECK_UNIQUE_POINTER(_tmp_dist, _len_dist);

	if (_tmp_dist != NULL) {
		if ((_in_dist = (double*)malloc(_len_dist)) == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memset((void*)_in_dist, 0, _len_dist);
	}
	calDist(_in_dist);
err:
	if (_in_dist) {
		memcpy(_tmp_dist, _in_dist, _len_dist);
		free(_in_dist);
	}

	return status;
}

static sgx_status_t SGX_CDECL sgx_put_secret_data(void* pms)
{
	ms_put_secret_data_t* ms = SGX_CAST(ms_put_secret_data_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	uint8_t* _tmp_p_secret = ms->ms_p_secret;
	uint32_t _tmp_secret_size = ms->ms_secret_size;
	size_t _len_p_secret = _tmp_secret_size;
	uint8_t* _in_p_secret = NULL;
	uint8_t* _tmp_gcm_mac = ms->ms_gcm_mac;
	size_t _len_gcm_mac = 16 * sizeof(*_tmp_gcm_mac);
	uint8_t* _in_gcm_mac = NULL;

	if (16 > (SIZE_MAX / sizeof(*_tmp_gcm_mac))) {
		status = SGX_ERROR_INVALID_PARAMETER;
		goto err;
	}

	CHECK_REF_POINTER(pms, sizeof(ms_put_secret_data_t));
	CHECK_UNIQUE_POINTER(_tmp_p_secret, _len_p_secret);
	CHECK_UNIQUE_POINTER(_tmp_gcm_mac, _len_gcm_mac);

	if (_tmp_p_secret != NULL) {
		_in_p_secret = (uint8_t*)malloc(_len_p_secret);
		if (_in_p_secret == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memcpy(_in_p_secret, _tmp_p_secret, _len_p_secret);
	}
	if (_tmp_gcm_mac != NULL) {
		_in_gcm_mac = (uint8_t*)malloc(_len_gcm_mac);
		if (_in_gcm_mac == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memcpy(_in_gcm_mac, _tmp_gcm_mac, _len_gcm_mac);
	}
	ms->ms_retval = put_secret_data(ms->ms_context, _in_p_secret, _tmp_secret_size, _in_gcm_mac);
err:
	if (_in_p_secret) free(_in_p_secret);
	if (_in_gcm_mac) free(_in_gcm_mac);

	return status;
}

static sgx_status_t SGX_CDECL sgx_sgx_ra_get_ga(void* pms)
{
	ms_sgx_ra_get_ga_t* ms = SGX_CAST(ms_sgx_ra_get_ga_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	sgx_ec256_public_t* _tmp_g_a = ms->ms_g_a;
	size_t _len_g_a = sizeof(*_tmp_g_a);
	sgx_ec256_public_t* _in_g_a = NULL;

	CHECK_REF_POINTER(pms, sizeof(ms_sgx_ra_get_ga_t));
	CHECK_UNIQUE_POINTER(_tmp_g_a, _len_g_a);

	if (_tmp_g_a != NULL) {
		if ((_in_g_a = (sgx_ec256_public_t*)malloc(_len_g_a)) == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memset((void*)_in_g_a, 0, _len_g_a);
	}
	ms->ms_retval = sgx_ra_get_ga(ms->ms_context, _in_g_a);
err:
	if (_in_g_a) {
		memcpy(_tmp_g_a, _in_g_a, _len_g_a);
		free(_in_g_a);
	}

	return status;
}

static sgx_status_t SGX_CDECL sgx_sgx_ra_proc_msg2_trusted(void* pms)
{
	ms_sgx_ra_proc_msg2_trusted_t* ms = SGX_CAST(ms_sgx_ra_proc_msg2_trusted_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	sgx_ra_msg2_t* _tmp_p_msg2 = ms->ms_p_msg2;
	size_t _len_p_msg2 = sizeof(*_tmp_p_msg2);
	sgx_ra_msg2_t* _in_p_msg2 = NULL;
	sgx_target_info_t* _tmp_p_qe_target = ms->ms_p_qe_target;
	size_t _len_p_qe_target = sizeof(*_tmp_p_qe_target);
	sgx_target_info_t* _in_p_qe_target = NULL;
	sgx_report_t* _tmp_p_report = ms->ms_p_report;
	size_t _len_p_report = sizeof(*_tmp_p_report);
	sgx_report_t* _in_p_report = NULL;
	sgx_quote_nonce_t* _tmp_p_nonce = ms->ms_p_nonce;
	size_t _len_p_nonce = sizeof(*_tmp_p_nonce);
	sgx_quote_nonce_t* _in_p_nonce = NULL;

	CHECK_REF_POINTER(pms, sizeof(ms_sgx_ra_proc_msg2_trusted_t));
	CHECK_UNIQUE_POINTER(_tmp_p_msg2, _len_p_msg2);
	CHECK_UNIQUE_POINTER(_tmp_p_qe_target, _len_p_qe_target);
	CHECK_UNIQUE_POINTER(_tmp_p_report, _len_p_report);
	CHECK_UNIQUE_POINTER(_tmp_p_nonce, _len_p_nonce);

	if (_tmp_p_msg2 != NULL) {
		_in_p_msg2 = (sgx_ra_msg2_t*)malloc(_len_p_msg2);
		if (_in_p_msg2 == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memcpy((void*)_in_p_msg2, _tmp_p_msg2, _len_p_msg2);
	}
	if (_tmp_p_qe_target != NULL) {
		_in_p_qe_target = (sgx_target_info_t*)malloc(_len_p_qe_target);
		if (_in_p_qe_target == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memcpy((void*)_in_p_qe_target, _tmp_p_qe_target, _len_p_qe_target);
	}
	if (_tmp_p_report != NULL) {
		if ((_in_p_report = (sgx_report_t*)malloc(_len_p_report)) == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memset((void*)_in_p_report, 0, _len_p_report);
	}
	if (_tmp_p_nonce != NULL) {
		if ((_in_p_nonce = (sgx_quote_nonce_t*)malloc(_len_p_nonce)) == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memset((void*)_in_p_nonce, 0, _len_p_nonce);
	}
	ms->ms_retval = sgx_ra_proc_msg2_trusted(ms->ms_context, (const sgx_ra_msg2_t*)_in_p_msg2, (const sgx_target_info_t*)_in_p_qe_target, _in_p_report, _in_p_nonce);
err:
	if (_in_p_msg2) free((void*)_in_p_msg2);
	if (_in_p_qe_target) free((void*)_in_p_qe_target);
	if (_in_p_report) {
		memcpy(_tmp_p_report, _in_p_report, _len_p_report);
		free(_in_p_report);
	}
	if (_in_p_nonce) {
		memcpy(_tmp_p_nonce, _in_p_nonce, _len_p_nonce);
		free(_in_p_nonce);
	}

	return status;
}

static sgx_status_t SGX_CDECL sgx_sgx_ra_get_msg3_trusted(void* pms)
{
	ms_sgx_ra_get_msg3_trusted_t* ms = SGX_CAST(ms_sgx_ra_get_msg3_trusted_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	sgx_report_t* _tmp_qe_report = ms->ms_qe_report;
	size_t _len_qe_report = sizeof(*_tmp_qe_report);
	sgx_report_t* _in_qe_report = NULL;
	sgx_ra_msg3_t* _tmp_p_msg3 = ms->ms_p_msg3;

	CHECK_REF_POINTER(pms, sizeof(ms_sgx_ra_get_msg3_trusted_t));
	CHECK_UNIQUE_POINTER(_tmp_qe_report, _len_qe_report);

	if (_tmp_qe_report != NULL) {
		_in_qe_report = (sgx_report_t*)malloc(_len_qe_report);
		if (_in_qe_report == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memcpy(_in_qe_report, _tmp_qe_report, _len_qe_report);
	}
	ms->ms_retval = sgx_ra_get_msg3_trusted(ms->ms_context, ms->ms_quote_size, _in_qe_report, _tmp_p_msg3, ms->ms_msg3_size);
err:
	if (_in_qe_report) free(_in_qe_report);

	return status;
}

SGX_EXTERNC const struct {
	size_t nr_ecall;
	struct {void* ecall_addr; uint8_t is_priv;} ecall_table[15];
} g_ecall_table = {
	15,
	{
		{(void*)(uintptr_t)sgx_ecall_DeezEnclave_sample, 0},
		{(void*)(uintptr_t)sgx_enclave_init_ra, 0},
		{(void*)(uintptr_t)sgx_enclave_ra_close, 0},
		{(void*)(uintptr_t)sgx_verify_att_result_mac, 0},
		{(void*)(uintptr_t)sgx_intializingEnclave, 0},
		{(void*)(uintptr_t)sgx_initializeIV, 0},
		{(void*)(uintptr_t)sgx_useEnclaveGetFileSize, 0},
		{(void*)(uintptr_t)sgx_encryptData, 0},
		{(void*)(uintptr_t)sgx_ecall_decryped_seal_buffer, 0},
		{(void*)(uintptr_t)sgx_ecallHere, 0},
		{(void*)(uintptr_t)sgx_calDist, 0},
		{(void*)(uintptr_t)sgx_put_secret_data, 0},
		{(void*)(uintptr_t)sgx_sgx_ra_get_ga, 0},
		{(void*)(uintptr_t)sgx_sgx_ra_proc_msg2_trusted, 0},
		{(void*)(uintptr_t)sgx_sgx_ra_get_msg3_trusted, 0},
	}
};

SGX_EXTERNC const struct {
	size_t nr_ocall;
	uint8_t entry_table[16][15];
} g_dyn_entry_table = {
	16,
	{
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
	}
};


sgx_status_t SGX_CDECL ocall_DeezEnclave_sample(const char* str)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_str = str ? strlen(str) + 1 : 0;

	ms_ocall_DeezEnclave_sample_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_DeezEnclave_sample_t);
	void *__tmp = NULL;

	ocalloc_size += (str != NULL && sgx_is_within_enclave(str, _len_str)) ? _len_str : 0;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_DeezEnclave_sample_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_DeezEnclave_sample_t));

	if (str != NULL && sgx_is_within_enclave(str, _len_str)) {
		ms->ms_str = (char*)__tmp;
		__tmp = (void *)((size_t)__tmp + _len_str);
		memcpy((void*)ms->ms_str, str, _len_str);
	} else if (str == NULL) {
		ms->ms_str = NULL;
	} else {
		sgx_ocfree();
		return SGX_ERROR_INVALID_PARAMETER;
	}
	
	status = sgx_ocall(0, ms);


	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_reference_getCharAt(int pos, char* out)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_out = 1;

	ms_ocall_reference_getCharAt_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_reference_getCharAt_t);
	void *__tmp = NULL;

	ocalloc_size += (out != NULL && sgx_is_within_enclave(out, _len_out)) ? _len_out : 0;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_reference_getCharAt_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_reference_getCharAt_t));

	ms->ms_pos = pos;
	if (out != NULL && sgx_is_within_enclave(out, _len_out)) {
		ms->ms_out = (char*)__tmp;
		__tmp = (void *)((size_t)__tmp + _len_out);
		memset(ms->ms_out, 0, _len_out);
	} else if (out == NULL) {
		ms->ms_out = NULL;
	} else {
		sgx_ocfree();
		return SGX_ERROR_INVALID_PARAMETER;
	}
	
	status = sgx_ocall(1, ms);

	if (out) memcpy((void*)out, ms->ms_out, _len_out);

	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_reference_copy_trim(size_t start, size_t end, char** out)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_out = sizeof(*out);

	ms_ocall_reference_copy_trim_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_reference_copy_trim_t);
	void *__tmp = NULL;

	ocalloc_size += (out != NULL && sgx_is_within_enclave(out, _len_out)) ? _len_out : 0;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_reference_copy_trim_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_reference_copy_trim_t));

	ms->ms_start = start;
	ms->ms_end = end;
	if (out != NULL && sgx_is_within_enclave(out, _len_out)) {
		ms->ms_out = (char**)__tmp;
		__tmp = (void *)((size_t)__tmp + _len_out);
		memset(ms->ms_out, 0, _len_out);
	} else if (out == NULL) {
		ms->ms_out = NULL;
	} else {
		sgx_ocfree();
		return SGX_ERROR_INVALID_PARAMETER;
	}
	
	status = sgx_ocall(2, ms);

	if (out) memcpy((void*)out, ms->ms_out, _len_out);

	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_reference_scanChromosome(const char* s, char** out)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_s = s ? strlen(s) + 1 : 0;
	size_t _len_out = sizeof(*out);

	ms_ocall_reference_scanChromosome_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_reference_scanChromosome_t);
	void *__tmp = NULL;

	ocalloc_size += (s != NULL && sgx_is_within_enclave(s, _len_s)) ? _len_s : 0;
	ocalloc_size += (out != NULL && sgx_is_within_enclave(out, _len_out)) ? _len_out : 0;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_reference_scanChromosome_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_reference_scanChromosome_t));

	if (s != NULL && sgx_is_within_enclave(s, _len_s)) {
		ms->ms_s = (char*)__tmp;
		__tmp = (void *)((size_t)__tmp + _len_s);
		memcpy((void*)ms->ms_s, s, _len_s);
	} else if (s == NULL) {
		ms->ms_s = NULL;
	} else {
		sgx_ocfree();
		return SGX_ERROR_INVALID_PARAMETER;
	}
	
	if (out != NULL && sgx_is_within_enclave(out, _len_out)) {
		ms->ms_out = (char**)__tmp;
		__tmp = (void *)((size_t)__tmp + _len_out);
		memset(ms->ms_out, 0, _len_out);
	} else if (out == NULL) {
		ms->ms_out = NULL;
	} else {
		sgx_ocfree();
		return SGX_ERROR_INVALID_PARAMETER;
	}
	
	status = sgx_ocall(3, ms);

	if (out) memcpy((void*)out, ms->ms_out, _len_out);

	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_print2file1(const char* s, int fileID)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_s = s ? strlen(s) + 1 : 0;

	ms_ocall_print2file1_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_print2file1_t);
	void *__tmp = NULL;

	ocalloc_size += (s != NULL && sgx_is_within_enclave(s, _len_s)) ? _len_s : 0;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_print2file1_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_print2file1_t));

	if (s != NULL && sgx_is_within_enclave(s, _len_s)) {
		ms->ms_s = (char*)__tmp;
		__tmp = (void *)((size_t)__tmp + _len_s);
		memcpy((void*)ms->ms_s, s, _len_s);
	} else if (s == NULL) {
		ms->ms_s = NULL;
	} else {
		sgx_ocfree();
		return SGX_ERROR_INVALID_PARAMETER;
	}
	
	ms->ms_fileID = fileID;
	status = sgx_ocall(4, ms);


	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_fetchBuffer(int* retval, uint8_t* fetched_buffers2unseal, size_t size_to_fetch, size_t start_position_of_the_first_seg2unseal)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_fetched_buffers2unseal = size_to_fetch;

	ms_ocall_fetchBuffer_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_fetchBuffer_t);
	void *__tmp = NULL;

	ocalloc_size += (fetched_buffers2unseal != NULL && sgx_is_within_enclave(fetched_buffers2unseal, _len_fetched_buffers2unseal)) ? _len_fetched_buffers2unseal : 0;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_fetchBuffer_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_fetchBuffer_t));

	if (fetched_buffers2unseal != NULL && sgx_is_within_enclave(fetched_buffers2unseal, _len_fetched_buffers2unseal)) {
		ms->ms_fetched_buffers2unseal = (uint8_t*)__tmp;
		__tmp = (void *)((size_t)__tmp + _len_fetched_buffers2unseal);
		memset(ms->ms_fetched_buffers2unseal, 0, _len_fetched_buffers2unseal);
	} else if (fetched_buffers2unseal == NULL) {
		ms->ms_fetched_buffers2unseal = NULL;
	} else {
		sgx_ocfree();
		return SGX_ERROR_INVALID_PARAMETER;
	}
	
	ms->ms_size_to_fetch = size_to_fetch;
	ms->ms_start_position_of_the_first_seg2unseal = start_position_of_the_first_seg2unseal;
	status = sgx_ocall(5, ms);

	if (retval) *retval = ms->ms_retval;
	if (fetched_buffers2unseal) memcpy((void*)fetched_buffers2unseal, ms->ms_fetched_buffers2unseal, _len_fetched_buffers2unseal);

	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_print_string(const char* str)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_str = str ? strlen(str) + 1 : 0;

	ms_ocall_print_string_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_print_string_t);
	void *__tmp = NULL;

	ocalloc_size += (str != NULL && sgx_is_within_enclave(str, _len_str)) ? _len_str : 0;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_print_string_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_print_string_t));

	if (str != NULL && sgx_is_within_enclave(str, _len_str)) {
		ms->ms_str = (char*)__tmp;
		__tmp = (void *)((size_t)__tmp + _len_str);
		memcpy((void*)ms->ms_str, str, _len_str);
	} else if (str == NULL) {
		ms->ms_str = NULL;
	} else {
		sgx_ocfree();
		return SGX_ERROR_INVALID_PARAMETER;
	}
	
	status = sgx_ocall(6, ms);


	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL create_session_ocall(sgx_status_t* retval, uint32_t* sid, uint8_t* dh_msg1, uint32_t dh_msg1_size, uint32_t timeout)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_sid = sizeof(*sid);
	size_t _len_dh_msg1 = dh_msg1_size;

	ms_create_session_ocall_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_create_session_ocall_t);
	void *__tmp = NULL;

	ocalloc_size += (sid != NULL && sgx_is_within_enclave(sid, _len_sid)) ? _len_sid : 0;
	ocalloc_size += (dh_msg1 != NULL && sgx_is_within_enclave(dh_msg1, _len_dh_msg1)) ? _len_dh_msg1 : 0;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_create_session_ocall_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_create_session_ocall_t));

	if (sid != NULL && sgx_is_within_enclave(sid, _len_sid)) {
		ms->ms_sid = (uint32_t*)__tmp;
		__tmp = (void *)((size_t)__tmp + _len_sid);
		memset(ms->ms_sid, 0, _len_sid);
	} else if (sid == NULL) {
		ms->ms_sid = NULL;
	} else {
		sgx_ocfree();
		return SGX_ERROR_INVALID_PARAMETER;
	}
	
	if (dh_msg1 != NULL && sgx_is_within_enclave(dh_msg1, _len_dh_msg1)) {
		ms->ms_dh_msg1 = (uint8_t*)__tmp;
		__tmp = (void *)((size_t)__tmp + _len_dh_msg1);
		memset(ms->ms_dh_msg1, 0, _len_dh_msg1);
	} else if (dh_msg1 == NULL) {
		ms->ms_dh_msg1 = NULL;
	} else {
		sgx_ocfree();
		return SGX_ERROR_INVALID_PARAMETER;
	}
	
	ms->ms_dh_msg1_size = dh_msg1_size;
	ms->ms_timeout = timeout;
	status = sgx_ocall(7, ms);

	if (retval) *retval = ms->ms_retval;
	if (sid) memcpy((void*)sid, ms->ms_sid, _len_sid);
	if (dh_msg1) memcpy((void*)dh_msg1, ms->ms_dh_msg1, _len_dh_msg1);

	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL exchange_report_ocall(sgx_status_t* retval, uint32_t sid, uint8_t* dh_msg2, uint32_t dh_msg2_size, uint8_t* dh_msg3, uint32_t dh_msg3_size, uint32_t timeout)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_dh_msg2 = dh_msg2_size;
	size_t _len_dh_msg3 = dh_msg3_size;

	ms_exchange_report_ocall_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_exchange_report_ocall_t);
	void *__tmp = NULL;

	ocalloc_size += (dh_msg2 != NULL && sgx_is_within_enclave(dh_msg2, _len_dh_msg2)) ? _len_dh_msg2 : 0;
	ocalloc_size += (dh_msg3 != NULL && sgx_is_within_enclave(dh_msg3, _len_dh_msg3)) ? _len_dh_msg3 : 0;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_exchange_report_ocall_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_exchange_report_ocall_t));

	ms->ms_sid = sid;
	if (dh_msg2 != NULL && sgx_is_within_enclave(dh_msg2, _len_dh_msg2)) {
		ms->ms_dh_msg2 = (uint8_t*)__tmp;
		__tmp = (void *)((size_t)__tmp + _len_dh_msg2);
		memcpy(ms->ms_dh_msg2, dh_msg2, _len_dh_msg2);
	} else if (dh_msg2 == NULL) {
		ms->ms_dh_msg2 = NULL;
	} else {
		sgx_ocfree();
		return SGX_ERROR_INVALID_PARAMETER;
	}
	
	ms->ms_dh_msg2_size = dh_msg2_size;
	if (dh_msg3 != NULL && sgx_is_within_enclave(dh_msg3, _len_dh_msg3)) {
		ms->ms_dh_msg3 = (uint8_t*)__tmp;
		__tmp = (void *)((size_t)__tmp + _len_dh_msg3);
		memset(ms->ms_dh_msg3, 0, _len_dh_msg3);
	} else if (dh_msg3 == NULL) {
		ms->ms_dh_msg3 = NULL;
	} else {
		sgx_ocfree();
		return SGX_ERROR_INVALID_PARAMETER;
	}
	
	ms->ms_dh_msg3_size = dh_msg3_size;
	ms->ms_timeout = timeout;
	status = sgx_ocall(8, ms);

	if (retval) *retval = ms->ms_retval;
	if (dh_msg3) memcpy((void*)dh_msg3, ms->ms_dh_msg3, _len_dh_msg3);

	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL close_session_ocall(sgx_status_t* retval, uint32_t sid, uint32_t timeout)
{
	sgx_status_t status = SGX_SUCCESS;

	ms_close_session_ocall_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_close_session_ocall_t);
	void *__tmp = NULL;


	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_close_session_ocall_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_close_session_ocall_t));

	ms->ms_sid = sid;
	ms->ms_timeout = timeout;
	status = sgx_ocall(9, ms);

	if (retval) *retval = ms->ms_retval;

	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL invoke_service_ocall(sgx_status_t* retval, uint8_t* pse_message_req, uint32_t pse_message_req_size, uint8_t* pse_message_resp, uint32_t pse_message_resp_size, uint32_t timeout)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_pse_message_req = pse_message_req_size;
	size_t _len_pse_message_resp = pse_message_resp_size;

	ms_invoke_service_ocall_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_invoke_service_ocall_t);
	void *__tmp = NULL;

	ocalloc_size += (pse_message_req != NULL && sgx_is_within_enclave(pse_message_req, _len_pse_message_req)) ? _len_pse_message_req : 0;
	ocalloc_size += (pse_message_resp != NULL && sgx_is_within_enclave(pse_message_resp, _len_pse_message_resp)) ? _len_pse_message_resp : 0;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_invoke_service_ocall_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_invoke_service_ocall_t));

	if (pse_message_req != NULL && sgx_is_within_enclave(pse_message_req, _len_pse_message_req)) {
		ms->ms_pse_message_req = (uint8_t*)__tmp;
		__tmp = (void *)((size_t)__tmp + _len_pse_message_req);
		memcpy(ms->ms_pse_message_req, pse_message_req, _len_pse_message_req);
	} else if (pse_message_req == NULL) {
		ms->ms_pse_message_req = NULL;
	} else {
		sgx_ocfree();
		return SGX_ERROR_INVALID_PARAMETER;
	}
	
	ms->ms_pse_message_req_size = pse_message_req_size;
	if (pse_message_resp != NULL && sgx_is_within_enclave(pse_message_resp, _len_pse_message_resp)) {
		ms->ms_pse_message_resp = (uint8_t*)__tmp;
		__tmp = (void *)((size_t)__tmp + _len_pse_message_resp);
		memset(ms->ms_pse_message_resp, 0, _len_pse_message_resp);
	} else if (pse_message_resp == NULL) {
		ms->ms_pse_message_resp = NULL;
	} else {
		sgx_ocfree();
		return SGX_ERROR_INVALID_PARAMETER;
	}
	
	ms->ms_pse_message_resp_size = pse_message_resp_size;
	ms->ms_timeout = timeout;
	status = sgx_ocall(10, ms);

	if (retval) *retval = ms->ms_retval;
	if (pse_message_resp) memcpy((void*)pse_message_resp, ms->ms_pse_message_resp, _len_pse_message_resp);

	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL sgx_oc_cpuidex(int cpuinfo[4], int leaf, int subleaf)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_cpuinfo = 4 * sizeof(*cpuinfo);

	ms_sgx_oc_cpuidex_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_sgx_oc_cpuidex_t);
	void *__tmp = NULL;

	ocalloc_size += (cpuinfo != NULL && sgx_is_within_enclave(cpuinfo, _len_cpuinfo)) ? _len_cpuinfo : 0;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_sgx_oc_cpuidex_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_sgx_oc_cpuidex_t));

	if (cpuinfo != NULL && sgx_is_within_enclave(cpuinfo, _len_cpuinfo)) {
		ms->ms_cpuinfo = (int*)__tmp;
		__tmp = (void *)((size_t)__tmp + _len_cpuinfo);
		memcpy(ms->ms_cpuinfo, cpuinfo, _len_cpuinfo);
	} else if (cpuinfo == NULL) {
		ms->ms_cpuinfo = NULL;
	} else {
		sgx_ocfree();
		return SGX_ERROR_INVALID_PARAMETER;
	}
	
	ms->ms_leaf = leaf;
	ms->ms_subleaf = subleaf;
	status = sgx_ocall(11, ms);

	if (cpuinfo) memcpy((void*)cpuinfo, ms->ms_cpuinfo, _len_cpuinfo);

	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL sgx_thread_wait_untrusted_event_ocall(int* retval, const void* self)
{
	sgx_status_t status = SGX_SUCCESS;

	ms_sgx_thread_wait_untrusted_event_ocall_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_sgx_thread_wait_untrusted_event_ocall_t);
	void *__tmp = NULL;


	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_sgx_thread_wait_untrusted_event_ocall_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_sgx_thread_wait_untrusted_event_ocall_t));

	ms->ms_self = SGX_CAST(void*, self);
	status = sgx_ocall(12, ms);

	if (retval) *retval = ms->ms_retval;

	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL sgx_thread_set_untrusted_event_ocall(int* retval, const void* waiter)
{
	sgx_status_t status = SGX_SUCCESS;

	ms_sgx_thread_set_untrusted_event_ocall_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_sgx_thread_set_untrusted_event_ocall_t);
	void *__tmp = NULL;


	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_sgx_thread_set_untrusted_event_ocall_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_sgx_thread_set_untrusted_event_ocall_t));

	ms->ms_waiter = SGX_CAST(void*, waiter);
	status = sgx_ocall(13, ms);

	if (retval) *retval = ms->ms_retval;

	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL sgx_thread_setwait_untrusted_events_ocall(int* retval, const void* waiter, const void* self)
{
	sgx_status_t status = SGX_SUCCESS;

	ms_sgx_thread_setwait_untrusted_events_ocall_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_sgx_thread_setwait_untrusted_events_ocall_t);
	void *__tmp = NULL;


	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_sgx_thread_setwait_untrusted_events_ocall_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_sgx_thread_setwait_untrusted_events_ocall_t));

	ms->ms_waiter = SGX_CAST(void*, waiter);
	ms->ms_self = SGX_CAST(void*, self);
	status = sgx_ocall(14, ms);

	if (retval) *retval = ms->ms_retval;

	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL sgx_thread_set_multiple_untrusted_events_ocall(int* retval, const void** waiters, size_t total)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_waiters = total * sizeof(*waiters);

	ms_sgx_thread_set_multiple_untrusted_events_ocall_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_sgx_thread_set_multiple_untrusted_events_ocall_t);
	void *__tmp = NULL;

	ocalloc_size += (waiters != NULL && sgx_is_within_enclave(waiters, _len_waiters)) ? _len_waiters : 0;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_sgx_thread_set_multiple_untrusted_events_ocall_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_sgx_thread_set_multiple_untrusted_events_ocall_t));

	if (waiters != NULL && sgx_is_within_enclave(waiters, _len_waiters)) {
		ms->ms_waiters = (void**)__tmp;
		__tmp = (void *)((size_t)__tmp + _len_waiters);
		memcpy((void*)ms->ms_waiters, waiters, _len_waiters);
	} else if (waiters == NULL) {
		ms->ms_waiters = NULL;
	} else {
		sgx_ocfree();
		return SGX_ERROR_INVALID_PARAMETER;
	}
	
	ms->ms_total = total;
	status = sgx_ocall(15, ms);

	if (retval) *retval = ms->ms_retval;

	sgx_ocfree();
	return status;
}

