#include <stdarg.h>
#include <stdio.h>      /* vsnprintf */
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <memory>
#include <vector>


#include "DeezEnclave.h"
#include "DeezEnclave_t.h"  /* print_string */

#include "Header/common.h"
#include "Header/Utils.h"

#include "sgx_tkey_exchange.h"
#include "sgx_tcrypto.h"
#include "sgx_trts.h"
#include "DataSealing.h"
#include "Decompress.h"

//bcftool include
#include <ctype.h>
#include "htslib/hts.h"
#include "version.h"
#include "bcftools.h"
// end of bcftool include

#define min_t(a,b) (a) < (b) ? (a) : (b)

//#define ENCLAVE_DEBUG 1

using namespace std;
void PRINT_BYTE_ARRAY(void *mem, uint32_t len)
{
	if(!mem || !len)
	{
		printf("\n( null )\n");
		return;
	}
	uint8_t *array = (uint8_t *)mem;
	printf("%u bytes:\n{\n", len);
	uint32_t i = 0;
	for(i = 0; i < len - 1; i++)
	{
		printf("0x%x, ", array[i]);
		if(i % 8 == 7) printf("\n");
	}
	printf("0x%x ", array[i]);
	printf("\n}\n");
}


bool optTest 	= false;
bool optSort 	= false;
bool optForce 	= false;
bool optStdout  = false;
bool optStats   = false;
bool optNoQual  = false;
bool optReadLossy = false;
bool optInvalidChr = false;
size_t optBlock = 1000;
char optQuality = 0;
char optLossy   = 0;
int optThreads  = 4;
int optFlag     = 0;
int optLogLevel	= 2;
bool optBzip = false;
size_t optSortMemory = GB;

string optRef 	 = "";
vector<string> optInput;
string optRange  = "";
string optOutput = "";

int current_save_sam_file_pos = 0;

COUNT_DATA_TYPE** cnt;
COUNT_DATA_TYPE** cnt1;
COUNT_DATA_TYPE** cnt2;
COUNT_DATA_TYPE* tmp_buffer1;
COUNT_DATA_TYPE* tmp_buffer2;
int start1;
int end1;
char chr1[MAX_REFERENCE_NAME];
char chr_range[MAX_CHROMESOME_RANGE_NAME_LENGTH];
uint32_t len0;

int numOfSeq = 0;
int numOfSpecifiySQ = 0;
int numOfPrimary = 0;
int numOfSecondary = 0;
int numOfSupplementary = 0;
int numOfUnmapped = 0;
int numOfReverseComplementary = 0;

int * iv_counter;
bool iv_counter_init_flag = false;

uint8_t g_secret[8] = {0};

static const sgx_ec256_public_t g_sp_pub_key[] = {
	//pub_key No1  (the original)
	{
		{
			0x72, 0x12, 0x8a, 0x7a, 0x17, 0x52, 0x6e, 0xbf,
				0x85, 0xd0, 0x3a, 0x62, 0x37, 0x30, 0xae, 0xad,
				0x3e, 0x3d, 0xaa, 0xee, 0x9c, 0x60, 0x73, 0x1d,
				0xb0, 0x5b, 0xe8, 0x62, 0x1c, 0x4b, 0xeb, 0x38
		},
		{
			0xd4, 0x81, 0x40, 0xd9, 0x50, 0xe2, 0x57, 0x7b,
				0x26, 0xee, 0xb7, 0x41, 0xe7, 0xc6, 0x14, 0xe2,
				0x24, 0xb7, 0xbd, 0xc9, 0x03, 0xf2, 0x9a, 0x28,
				0xa8, 0x3c, 0xc8, 0x10, 0x11, 0x14, 0x5e, 0x06
			}
	},
		// endian test error
//			{
//				{
//						0x11, 0x27, 0x8c, 0x38, 0x4e, 0x36, 0x4e, 0x6f,
//						0x1e, 0xb4, 0x08, 0xd3, 0x98, 0x15, 0x30, 0x93,
//						0x87, 0xa5, 0xcc, 0xef, 0xef, 0xfb, 0x60, 0xde,
//						0x53, 0x00, 0x90, 0xc0, 0xa3, 0x32, 0x15, 0xa0
//
//
//				},
//				{
//						0x25, 0xe4, 0x7d, 0x66, 0x60, 0x1e, 0xa1, 0x7e,
//						0x04, 0x3c, 0xf2, 0xee, 0xd7, 0x41, 0x6a, 0x3e,
//						0x3e, 0xb7, 0x2c, 0x6c, 0xf4, 0x4f, 0x3d, 0x14,
//						0x94, 0x8c, 0x14, 0x99, 0xc5, 0x32, 0xf4, 0xbe
//					}
//			},
		// normal test
//					{
//						{
//								0x87, 0x5D, 0xF1, 0x65, 0xA1, 0xB2, 0xAF, 0x3E,
//								0x92, 0x24, 0x7D, 0xEA, 0x9C, 0xB4, 0x34, 0xC1,
//								0x1F, 0xC4, 0x82, 0x30, 0x6C, 0x84, 0x21, 0x55,
//								0xAC, 0xBC, 0x26, 0xC5, 0x39, 0xDE, 0x7A, 0x10
//
//
//						},
//						{	0xF5, 0x46, 0xD2, 0x41, 0x18, 0x6F, 0x8F, 0x9A,
//							0xBB, 0x8F, 0xB4, 0xC0, 0xBA, 0x4F, 0x98, 0x6E,
//							0xEA, 0x5A, 0xA5, 0xE5, 0x3A, 0xD4, 0xFD, 0xF7,
//							0x36, 0xED, 0x27, 0xFB, 0xA3, 0x75, 0xD7, 0x56
//							}
//					},

		//pub_key No2
	{
		{
			0xd1, 0x1e, 0x95, 0x94, 0xec, 0xa0, 0x1d, 0xaa,
				0x8a, 0x79, 0x39, 0xe9, 0x46, 0xb0, 0x33, 0xc2,
				0xf3, 0x23, 0xc5, 0x27, 0x34, 0x8e, 0x40, 0xb5,
				0xcd, 0x23, 0xa5, 0xcc, 0xea, 0x16, 0x1a, 0xa0
		},
		{
			0x44, 0xb3, 0x4a, 0xa1, 0x84, 0x7b, 0x81, 0x82,
				0x50, 0x16, 0xe2, 0x17, 0xd3, 0xcd, 0x21, 0x77,
				0xdd, 0x41, 0x05, 0xad, 0x9f, 0x32, 0xec, 0x49,
				0x1f, 0x29, 0x2a, 0xfa, 0xf8, 0xa7, 0x6c, 0xdf
			}
	},

		//pub_key No3
	{
		{
			0x9d, 0x5d, 0xcf, 0x57, 0x4d, 0x94, 0x6d, 0x90,
				0x21, 0x52, 0xaf, 0xb5, 0x28, 0x5e, 0x8e, 0xff,
				0xa8, 0xe2, 0x37, 0x3d, 0x39, 0x2e, 0x5b, 0xc0,
				0x1a, 0x8d, 0x16, 0xe0, 0xac, 0x89, 0xa4, 0x94
		},
		{
			0x77, 0x5b, 0xff, 0x94, 0xc5, 0xc3, 0x86, 0x53,
				0xc3, 0x6c, 0x2e, 0xfb, 0x39, 0x13, 0xb3, 0xcd,
				0x82, 0x61, 0x7d, 0x3b, 0x16, 0xc4, 0x7e, 0x26,
				0xd7, 0x43, 0x44, 0x11, 0xa8, 0xc5, 0xee, 0x21
			}
	},
		//pub_key No4
	{
		{
			0x63, 0x45, 0x44, 0x1c, 0x72, 0x4b, 0xdc, 0x82,
				0xb3, 0xbb, 0xf2, 0x34, 0x52, 0x88, 0x3c, 0xbf,
				0x99, 0x09, 0xdf, 0x47, 0x35, 0x6b, 0x88, 0xde,
				0x74, 0xff, 0x12, 0x39, 0x10, 0xda, 0xf5, 0x53
		},
		{
			0x5d, 0x2e, 0x6f, 0xae, 0xa1, 0x73, 0x75, 0x75,
				0xaf, 0x5c, 0x34, 0x6d, 0x5c, 0x5c, 0x6a, 0x42,
				0x94, 0x96, 0x9f, 0xe4, 0x30, 0x18, 0x12, 0x2d,
				0x36, 0x9d, 0x13, 0xb6, 0x3e, 0xbe, 0x7d, 0xcc
			}
	},
		//pub_key No5
	{
		{
			0xf9, 0x7b, 0xe1, 0x11, 0xa1, 0xf9, 0xd6, 0x15,
				0x7f, 0xc0, 0x94, 0x62, 0x9f, 0x13, 0xa9, 0x93,
				0x90, 0x8a, 0xf8, 0x86, 0xa4, 0xcb, 0xe8, 0x56,
				0x1e, 0xbd, 0x2d, 0x70, 0x4b, 0xe8, 0x32, 0x71
		},
		{
			0xa6, 0x16, 0x2f, 0xea, 0x2f, 0xaf, 0x8e, 0x23,
				0x77, 0x41, 0xff, 0x0e, 0xcb, 0x2c, 0xce, 0x72,
				0x90, 0x43, 0x55, 0xff, 0xc2, 0xf0, 0x5d, 0x09,
				0x1f, 0x98, 0x1a, 0xfb, 0x5b, 0x32, 0x05, 0xec
			}
	},
		//pub_key No6
	{
		{
			0xeb, 0xf0, 0x06, 0x5a, 0x9a, 0x96, 0x58, 0x73,
				0x66, 0x41, 0xc9, 0x3b, 0x82, 0x2d, 0x4f, 0x22,
				0x65, 0xe4, 0x95, 0x4a, 0xef, 0xcf, 0x8e, 0xac,
				0x76, 0x5b, 0xb8, 0x52, 0x17, 0xa6, 0xcb, 0x29
		},
		{
			0x5e, 0x52, 0xdc, 0x4b, 0xe5, 0x15, 0x56, 0xec,
				0xe8, 0x13, 0x74, 0xd7, 0x4b, 0x17, 0x8a, 0xed,
				0x87, 0xfa, 0x92, 0x82, 0xf5, 0x35, 0xcc, 0x5d,
				0xfc, 0xfd, 0xd5, 0x0e, 0x31, 0x69, 0x71, 0xe0
			}
	},
		//pub_key No7
	{
		{
			0x04, 0xda, 0xe0, 0x04, 0xea, 0xc2, 0x28, 0x7e,
				0xaa, 0x0b, 0xdd, 0x75, 0x16, 0x79, 0x48, 0x39,
				0xf2, 0xd6, 0x54, 0xeb, 0x67, 0x5f, 0x17, 0x59,
				0x59, 0xa3, 0xd9, 0xe2, 0x07, 0xad, 0x9c, 0x10
		},
		{
			0xbc, 0xfb, 0xe5, 0xa0, 0x24, 0xbc, 0x2a, 0xd4,
				0xb1, 0x6a, 0xcb, 0xf7, 0x68, 0xc2, 0x78, 0x63,
				0xc3, 0x9c, 0xdd, 0x7a, 0xcf, 0x33, 0x61, 0x8a,
				0x69, 0xcc, 0xc8, 0xb1, 0xf7, 0x3e, 0x09, 0xd2
			}
	},
		//pub_key No8
	{
		{
			0x69, 0x1e, 0x08, 0xd0, 0xaa, 0x42, 0x19, 0x73,
				0x69, 0x8a, 0x77, 0xbc, 0x0a, 0x7f, 0x66, 0xe9,
				0xb1, 0x8f, 0x6b, 0x7d, 0x20, 0x8d, 0x39, 0x6f,
				0xee, 0x81, 0x5c, 0x05, 0xe7, 0x19, 0x98, 0x3e
		},
		{
			0xd1, 0x65, 0x39, 0xf4, 0xd8, 0xba, 0x91, 0x7d,
				0x41, 0x93, 0xd4, 0x86, 0x16, 0x96, 0xf3, 0xd1,
				0x36, 0x5b, 0x22, 0x3e, 0x8f, 0x98, 0x77, 0x1c,
				0x88, 0x78, 0x11, 0xd7, 0xc8, 0xb0, 0xd2, 0x5a
			}
	},
		//pub_key No9
	{
		{
			0x96, 0xf7, 0xc9, 0x4e, 0xe4, 0x4d, 0xb1, 0xfb,
				0x72, 0xbd, 0x68, 0x00, 0x15, 0x77, 0x4d, 0x8d,
				0xd6, 0x14, 0x31, 0x51, 0x20, 0x8c, 0xce, 0xac,
				0x77, 0x6d, 0x04, 0xea, 0x71, 0x15, 0x2c, 0xe0
		},
		{
			0xcb, 0x94, 0x4a, 0x2d, 0x99, 0x28, 0xc9, 0xa5,
				0xb7, 0x53, 0x82, 0x12, 0xcf, 0x1b, 0x3f, 0xd0,
				0xf9, 0xb7, 0x78, 0x80, 0xe4, 0xca, 0x58, 0x6e,
				0xe0, 0xa0, 0xa1, 0xcd, 0xc5, 0xb0, 0x5f, 0xe0
			}
	},
		//pub_key No10
	{
		{
			0x71, 0x9b, 0xbc, 0x8e, 0x48, 0x05, 0x6d, 0xff,
				0xf2, 0xd5, 0x58, 0x72, 0x06, 0x7f, 0x8f, 0x14,
				0x5b, 0xed, 0xc5, 0xcd, 0xe4, 0xda, 0xe9, 0x6f,
				0x4b, 0x23, 0x12, 0x3f, 0x66, 0xae, 0x48, 0x7b
		},
		{
			0x3e, 0x56, 0xae, 0x4e, 0xa1, 0x96, 0x7f, 0xe7,
				0x95, 0x5e, 0x40, 0xff, 0xdc, 0xc5, 0x51, 0x76,
				0xbd, 0x78, 0x7f, 0x45, 0xbe, 0x6b, 0xe2, 0xb6,
				0x2d, 0x2b, 0x5c, 0xa6, 0x45, 0xe2, 0x3d, 0x2b
			}
	},
		//pub_key No11
	{
		{
			0xd7, 0x79, 0x7f, 0x71, 0x69, 0x10, 0x3e, 0x11,
				0x96, 0xbb, 0xc3, 0x89, 0xd8, 0x64, 0xd7, 0xa9,
				0xde, 0x3b, 0xb5, 0x68, 0xab, 0x78, 0x14, 0x42,
				0x53, 0xbc, 0x89, 0x5e, 0x34, 0x30, 0x8f, 0x2b
		},
		{
			0x79, 0x13, 0xc1, 0x51, 0xdb, 0x04, 0x19, 0x91,
				0xcf, 0x40, 0xea, 0x29, 0x22, 0x36, 0x94, 0xa8,
				0xb0, 0xd0, 0xdb, 0x96, 0x67, 0x7d, 0x97, 0xb5,
				0x87, 0x9e, 0x13, 0x86, 0x83, 0x43, 0x20, 0xb5
			}
	},
		//pub_key No12
	{
		{
			0x78, 0xcc, 0xbb, 0x16, 0x57, 0xf2, 0x28, 0xcc,
				0x94, 0x72, 0x75, 0x51, 0xc2, 0x15, 0x6e, 0x10,
				0x13, 0x5b, 0x2b, 0x76, 0x73, 0x11, 0x8d, 0x80,
				0x22, 0xbc, 0x43, 0xa6, 0x5f, 0xe4, 0x95, 0xd5
		},
		{
			0xdf, 0xf1, 0x70, 0x67, 0xd6, 0xe8, 0x19, 0x64,
				0x1b, 0x80, 0x82, 0x5d, 0x3e, 0x97, 0x95, 0xb3,
				0x26, 0x81, 0x8d, 0x0b, 0xe1, 0x15, 0x47, 0x72,
				0x11, 0x2d, 0xe7, 0xdc, 0x29, 0x53, 0xb4, 0x12
			}
	}
};

// This ecall is a wrapper of sgx_ra_init to create the trusted
// KE exchange key context needed for the remote attestation
// SIGMA API's. Input pointers aren't checked since the trusted stubs
// copy them into EPC memory.
//
// @param b_pse Indicates whether the ISV app is using the
//              platform services.
// @param p_context Pointer to the location where the returned
//                  key context is to be copied.
// @param client_id
//
// @return Any error return from the create PSE session if b_pse
//         is true.
// @return Any error returned from the trusted key exchange API
//         for creating a key context.

sgx_status_t enclave_init_ra(
	int b_pse,
	int client_id,
	sgx_ra_context_t *p_context)
{
	// isv enclave call to trusted key exchange library.
	sgx_status_t ret;
	if(b_pse)
	{
		int busy_retry_times = 2;
		do{
			ret = sgx_create_pse_session();
		}while (ret == SGX_ERROR_BUSY && busy_retry_times--);
		if (ret != SGX_SUCCESS)
			return ret;
	}



	ret = sgx_ra_init(&g_sp_pub_key[client_id], b_pse, p_context);
	if(b_pse)
	{
		sgx_close_pse_session();
		return ret;
	}
	return ret;
}


// Closes the tKE key context used during the SIGMA key
// exchange.
//
// @param context The trusted KE library key context.
//
// @return Return value from the key context close API

sgx_status_t SGXAPI enclave_ra_close(
	sgx_ra_context_t context)
{
	sgx_status_t ret;
	ret = sgx_ra_close(context);
	return ret;
}

// Verify the mac sent in att_result_msg from the SP using the
// MK key. Input pointers aren't checked since the trusted stubs
// copy them into EPC memory.
//
//
// @param context The trusted KE library key context.
// @param p_message Pointer to the message used to produce MAC
// @param message_size Size in bytes of the message.
// @param p_mac Pointer to the MAC to compare to.
// @param mac_size Size in bytes of the MAC
//
// @return SGX_ERROR_INVALID_PARAMETER - MAC size is incorrect.
// @return Any error produced by tKE  API to get SK key.
// @return Any error produced by the AESCMAC function.
// @return SGX_ERROR_MAC_MISMATCH - MAC compare fails.

sgx_status_t verify_att_result_mac(sgx_ra_context_t context,
								   uint8_t* p_message,
								   size_t message_size,
								   uint8_t* p_mac,
								   size_t mac_size)
{
	sgx_status_t ret;
	sgx_ec_key_128bit_t mk_key;

	if(mac_size != sizeof(sgx_mac_t))
	{
		ret = SGX_ERROR_INVALID_PARAMETER;
		return ret;
	}
	if(message_size > UINT32_MAX)
	{
		ret = SGX_ERROR_INVALID_PARAMETER;
		return ret;
	}

	do {
		uint8_t mac[SGX_CMAC_MAC_SIZE] = {0};

		ret = sgx_ra_get_keys(context, SGX_RA_KEY_MK, &mk_key);
		if(SGX_SUCCESS != ret)
		{
			break;
		}
		ret = sgx_rijndael128_cmac_msg(&mk_key,
			p_message,
			(uint32_t)message_size,
			&mac);
		if(SGX_SUCCESS != ret)
		{
			break;
		}
		if(memcmp(p_mac, mac, sizeof(mac)))//TODO : Before is consttime_memequal //by wwj
		{
			ret = SGX_ERROR_MAC_MISMATCH;
			break;
		}

	}
	while(0);

	return ret;
}


void intializingEnclave(const char *chr0, int start0, int end0)
{
	memcpy(chr1, chr0, strlen(chr0));
	chr1[strlen(chr0)] = '\0';
	start1 = start0 - 1;
	end1 = end0 - 1;

	//vsnprintf(chr_range, MAX_CHROMESOME_RANGE_NAME_LENGTH, "%s:%d-%d", chr0, start0, end0);
	sprintf(chr_range,"%s:%d-%d", chr0, start0 ,end0);

	len0 = end1 - start1 + 1;

	cnt1 = new COUNT_DATA_TYPE*[DNA_BASE_NUM];
	cnt2 = new COUNT_DATA_TYPE*[DNA_BASE_NUM];

	tmp_buffer1 = new COUNT_DATA_TYPE[DNA_BASE_NUM * len0];
	tmp_buffer2 = new COUNT_DATA_TYPE[DNA_BASE_NUM * len0];

	for(int m = 0; m < DNA_BASE_NUM; m++){
		*(cnt1 + m) = tmp_buffer1 + len0 * m;
		*(cnt2 + m) = tmp_buffer2 + len0 * m;
	}

	memset(tmp_buffer1, 0, DNA_BASE_NUM * len0 * sizeof(COUNT_DATA_TYPE));
	memset(tmp_buffer2, 0, DNA_BASE_NUM * len0 * sizeof(COUNT_DATA_TYPE));
}

void initializeIV(int client_num)
{
	if (! iv_counter_init_flag) {
		iv_counter_init_flag = true;
		iv_counter = new int[client_num];
		for ( int i = 0; i < client_num; i ++) {
			iv_counter[i] = 0;
		}
	}
}

void useEnclaveGetFileSize(sgx_ra_context_t context, char* data, int size_in,
				 char*decryptedData, int decryptedData_size,
				 int numberofParties, int client_i)
{
//	ocall_print_string("call function useEnclaveGetFileSize successfully!\n");

	//decryption
	sgx_status_t ret = SGX_SUCCESS;
	sgx_ec_key_128bit_t sk_key;
	ret = sgx_ra_get_keys(context, SGX_RA_KEY_SK, &sk_key);

	if(SGX_SUCCESS != ret)
		printf("get sk key failed!!\n");

	uint8_t aes_gcm_iv[12] = {0};

	memcpy( aes_gcm_iv, &iv_counter[context], sizeof(int));
	iv_counter[context] ++;

	uint32_t src_len = sizeof(MESSAGE_LENGTH_TYPE);
	MESSAGE_LENGTH_TYPE fileSize = 0;

	ret = sgx_rijndael128GCM_decrypt(&sk_key,
		(uint8_t *)data,
		src_len,
		(uint8_t *)decryptedData,
		&aes_gcm_iv[0],
		12,
		NULL,
		0,
		(const sgx_aes_gcm_128bit_tag_t *)(data + src_len));

	if (ret != SGX_SUCCESS)
	{
		printf("The file size decryption is failed!");
		abort();
	}

#if defined ENCLAVE_DEBUG
	printf("**ENCLAVE_DEBUG** file size: %u \n", fileSize);
	printf("**ENCLAVE_DEBUG** size_in: %d\n", size_in);
	printf("**ENCLAVE_DEBUG**decyper result: %#x \n", ret);
#endif
}

void encryptData(sgx_ra_context_t context,
				 uint8_t *p_src, uint32_t src_len,
				 uint8_t *p_dst, uint32_t dst_len)
{
	sgx_status_t ret = SGX_SUCCESS;

	//load the key
	sgx_ec_key_128bit_t sk_key;
	ret = sgx_ra_get_keys(context, SGX_RA_KEY_SK, &sk_key);
	if(SGX_SUCCESS != ret)
		printf("get sk key failed!!\n");

	uint8_t aes_gcm_iv[12] = {0};

	memcpy( aes_gcm_iv, &iv_counter[context], sizeof(int));
	iv_counter[context] ++;

	if(iv_counter[context] == 128129)
	{
		ocall_print_string("iv_counter[context] == 128129");
	}
	else
	{
		ocall_print_string("iv_counter[context] != 128129");
	}

	ret = sgx_rijndael128GCM_encrypt(&sk_key,
		p_src,
		src_len,
		p_dst,
		&aes_gcm_iv[0],
		12,
		NULL,
		0,
		(sgx_aes_gcm_128bit_tag_t *)(p_dst + src_len));

	if (ret != SGX_SUCCESS)
	{
		printf("Data encryted failed!");
		abort();
	}
}

void ecall_decryped_seal_buffer(sgx_ra_context_t context, char* data2seal, int data_buffer_size, char* sealed_secret, int sealed_size,
								int true_size, int64_t index, char* sealedDataMAC, int MAC_size)
{
	char decryptedData[SEALING_BUFFER_SIZE];
	getDecryptedBlock(context, (uint8_t*) data2seal, true_size-ENCRYPTED_ADD_SIZE, (uint8_t*)decryptedData);

	uint8_t secret_buffer[SEALING_BUFFER_SIZE];
	//protected_secret secret_buffer;
	memcpy(secret_buffer, decryptedData, true_size-ENCRYPTED_ADD_SIZE);
	//secret_buffer.true_size = true_size-ENCRYPTED_ADD_SIZE;
	uint32_t ret = 0;
	ret = seal_data((uint8_t *)secret_buffer, SEALING_BUFFER_SIZE, (uint8_t *)sealed_secret, SEALED_BUFFER_SIZE,
		index, (uint8_t *)sealedDataMAC, false, false);

	//delete[] decryptedData;
	//printf("return value from function seal: %d\n", ret);
}

sgx_status_t getDecryptedBlock(sgx_ra_context_t context,
				 const uint8_t *p_src, uint32_t src_len, uint8_t *p_dst)
{
	//uint32_t src_len = SEALING_BUFFER_SIZE;
	sgx_status_t ret = SGX_SUCCESS;
	sgx_ec_key_128bit_t sk_key;
	ret = sgx_ra_get_keys(context, SGX_RA_KEY_SK, &sk_key);



	if(SGX_SUCCESS != ret)
		printf("get sk key failed!!\n");

	uint8_t aes_gcm_iv[12] = {0};

	memcpy( aes_gcm_iv, &iv_counter[context], sizeof(int));
	iv_counter[context] ++;

	ret = sgx_rijndael128GCM_decrypt(&sk_key,
		p_src,
		src_len,
		(uint8_t *)p_dst,
		&aes_gcm_iv[0],
		12,
		NULL,
		0,
		(const sgx_aes_gcm_128bit_tag_t *)(p_src + src_len));

	return ret;
}

void ecallHere(size_t inFileSz,  int deezOrder, char* sealedDataMAC, int mac_size)
{

	if(deezOrder == 0)
	{
		cnt = cnt1;
	}
	else
	{
		cnt = cnt2;
	}

	FileDecompressor sd(NULL, inFileSz, optRef, optBlock, sealedDataMAC);

	string Range(chr_range);

	if (Range.size() == 0)
	{
		sd.decompress(optFlag);
	}
	else
	{
		sd.decompress(Range, optFlag);
	}
}

void calDist(double *dist)
{
	*dist = 0;
	double tmp;
	int tmp_cunt1;
	int tmp_cunt2;

	double hist1 = 0;
	double hist2 = 0;

	for(int n = 0; n < len0; n++){
		tmp_cunt1 = 0;
		tmp_cunt2 = 0;
		for(int m = 0; m < DNA_BASE_NUM; m++){
			tmp_cunt1 += cnt1[m][n];
			tmp_cunt2 += cnt2[m][n];
		}

		for(int m = 0; m < DNA_BASE_NUM; m++){
			hist1 = tmp_cunt1 == 0 ? 0 : ((double)cnt1[m][n]/tmp_cunt1);
			hist2 = tmp_cunt2 == 0 ? 0 : ((double)cnt2[m][n]/tmp_cunt2);
			tmp = hist1 > hist2 ? (hist1 - hist2) : (hist2 - hist1);
			*dist += tmp;
		}
	}

	*dist /= len0;
	*dist /= DNA_BASE_NUM;
}

/* 
 * printf: 
 *   Invokes OCALL to display the enclave buffer to the terminal.
 */
void printf(const char *fmt, ...)
{
    char buf[BUFSIZ] = {'\0'};
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, BUFSIZ, fmt, ap);
    va_end(ap);
    ocall_DeezEnclave_sample(buf);
}

void sprintf(char* buf, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buf, BUFSIZ, fmt, ap);
	va_end(ap);
}

int ecall_DeezEnclave_sample()
{
  printf("IN DEEZENCLAVE\n");
  return 0;
}

int updatingCounts(char* chrome_name, uint8_t*buffer, uint32_t start, uint32_t end, COUNT_DATA_TYPE** cnt)
{
	if(*buffer == '@'){
		return 0;
	}

	uint32_t current_cigar_pos;
	uint32_t current_seq_pos = 0;
	char ref_name[MAX_REFERENCE_NAME];
	uint32_t pos;
	char op;
	uint32_t cigar_count;
	bool noCigar = false;

	nextWord(buffer);

	int flag = readFlag(buffer);

	if((flag & 0x900) == 0){
		numOfPrimary++;
	}

	if((flag & 0x100) != 0){
		numOfSecondary++;
		noCigar = true;
	}

	if((flag & 0x800) != 0){
		numOfSupplementary++;
	}

	if((flag & 0x4) != 0){
		numOfUnmapped++;
		noCigar = true;
	}

	if((flag & 0x10) != 0){
		numOfReverseComplementary++;
	}

	nextWord(buffer);

	copyReferenceName1(buffer, ref_name);

	if(strcmp(ref_name, chrome_name)!= 0){
		noCigar = true;
	}

	numOfSpecifiySQ++;

	if(noCigar){
		return 0;
	}


	nextWord(buffer);

	uint32_t len0 = getWordEndPos(buffer);

	char* tmp = new char[len0 + 1];

	memcpy(tmp, buffer, len0 + 1);
	tmp[len0] = '\0';
	pos = atoi(tmp);
	pos--; // 1-based postion

	current_seq_pos += pos;

	if(pos > end){
		return 0;
	}

	nextWord(buffer);
	nextWord(buffer);

	uint8_t* buffer_sigar = buffer;

	nextWord(buffer);
	nextWord(buffer);
	nextWord(buffer);
	nextWord(buffer);

	const char* buffer_seq = (char*)buffer;

	if(*buffer_seq == '*'){
		return 0;
	}

	bool stop_read = false;
	while(getACigar(buffer_sigar, op, cigar_count)){
		if(op == 'M' || op == 'D'|| op == 'N' || op == 'S' || op == '=' || op == 'X'){
			if(current_seq_pos + cigar_count > end){
				cigar_count = end - current_seq_pos + 1;
				stop_read = true;
			}
		}
		switch(op){
		case '=':
		case 'M':
		case 'S':
		case 'X':
			updatingCount(cnt, buffer_seq, current_seq_pos, cigar_count, start, end);
			current_seq_pos += cigar_count;
			buffer_seq += cigar_count;
			break;
		case 'D':
			current_seq_pos += cigar_count;
			//buffer_seq += cigar_count;
			break;
		case 'N':
			current_seq_pos += cigar_count;
			//buffer_seq += cigar_count;
			break;
		case 'I':
			buffer_seq += cigar_count;
		default:
			break;
		}

		if(stop_read){
			break;
		}
	}

	delete[] tmp;

	return 1;

}

int nextWord(uint8_t* buffer)
{
	while(*(buffer++) != '\t');

	return 0;
}

int copyReferenceName1(uint8_t* buffer, char* name0)
{
	int len = getWordEndPos(buffer);


	memcpy(name0, buffer, len);
	name0[len] = 0;

	return 1;
}

uint32_t getWordEndPos(uint8_t* buffer)
{
	uint32_t word_end = 0;

	while(*(buffer + word_end) != '\t'){
		word_end++;
	}

	return word_end;
}

int updatingCount(COUNT_DATA_TYPE** cnt, const char* buffer, uint32_t pos_seq, uint32_t len, uint32_t start, uint32_t end){
	int tmp = 0;

	if(pos_seq < start){
		if(pos_seq + len < start){
			return 0;
		}else{
			do{
				--len;
				pos_seq++;
				buffer++;
			}while(pos_seq < start);
		}
	}

	tmp = pos_seq -start;

	while((len > 0) && (pos_seq <= end)){
		updatingPosCount(cnt,  tmp, *buffer);
		len--;
		pos_seq++;
		tmp++;
		buffer++;
	}

}

int getACigar(uint8_t *buffer, char op, uint32_t count)
{
	char tmp[20];
	int tmp_pos = 0;

	if(!isdigit(buffer[0])){
		return 0;
	}


	while(isdigit(*buffer)){
		tmp[tmp_pos++] = *buffer++;
	}
	tmp[tmp_pos] = '\0';

	count = atoi(tmp);
	op = *buffer++;

	return 1;
}

int readFlag(uint8_t* buffer)
{
	int len = getTheEndOfWord_1(buffer);
	char* tmp = new char[len+1];

	memcpy(tmp, buffer, len);


	tmp[len] = '\0';

	int flag = atoi(tmp);

	delete[] tmp;

	return flag;
}

int getTheEndOfWord_1(uint8_t* buffer)
{
	int n = 0;
	while(*buffer != '\t'){
		buffer++;
		n++;
	}
	return n;
}

int updatingPosCount(COUNT_DATA_TYPE** cnt, int tmp, const char ch)
{

	switch(ch){
	case 'A':
		cnt[0][tmp]++;
		break;
	case 'G':
		cnt[1][tmp]++;
		break;
	case 'C':
		cnt[2][tmp]++;
		break;
	case 'T':
		cnt[3][tmp]++;
		break;
	}

	return 1;
}


sgx_status_t put_secret_data(
    sgx_ra_context_t context,
    uint8_t *p_secret,
    uint32_t secret_size,
    uint8_t *p_gcm_mac)
{
    sgx_status_t ret = SGX_SUCCESS;
    sgx_ec_key_128bit_t sk_key;

    do {
        if(secret_size != 8)
        {
            ret = SGX_ERROR_INVALID_PARAMETER;
            break;
        }

        ret = sgx_ra_get_keys(context, SGX_RA_KEY_SK, &sk_key);
        PRINT_BYTE_ARRAY(&sk_key,16);
        PRINT_BYTE_ARRAY(p_secret,8);

//        ocall_print_string(sk_key,128);
        if(SGX_SUCCESS != ret)
        {
            break;
        }

        uint8_t aes_gcm_iv[12] = {0};
        ret = sgx_rijndael128GCM_decrypt(&sk_key,
                                         p_secret,
                                         secret_size,
                                         &g_secret[0],
                                         &aes_gcm_iv[0],
                                         12,
                                         NULL,
                                         0,
                                         (const sgx_aes_gcm_128bit_tag_t *)
                                            (p_gcm_mac));

        uint32_t i;
        bool secret_match = true;
        printf("print decrypted secre\n");
        for(i=0;i<secret_size;i++)
        {
        	printf("%d,",g_secret[i]);
//            if(g_secret[i] != i)
//            {
//                secret_match = false;
//            }
        }

        if(!secret_match)
        {
            ret = SGX_ERROR_UNEXPECTED;
        }

        // Once the server has the shared secret, it should be sealed to
        // persistent storage for future use. This will prevents having to
        // perform remote attestation until the secret goes stale. Once the
        // enclave is created again, the secret can be unsealed.
    } while(0);
    return ret;
}

// bcftool ecall

char *sgxstrdup(const char *s)
{
    size_t len = strlen(s) + 1;
    void  *mem = malloc(len);

    if (mem == NULL)
        return (char*)mem;

    return (char*)memcpy(mem, s, len);
}


int ecall_bcfenclave_sample()
{
  printf("IN BCFENCLAVE\n");
  int argc = 7;
  char* argv[] = {"bcftools",
                    "mpileup",
                    "-f",             // Default paramater
                    "mpileup.ref.fa", // reference file
                    "mpileup1.sam",   // Input Sam file used for variant call
                    "-o",             // Default paramater
                    "mpileup1.vcf"    // Output File
                    };

  bam_mpileup(argc - 1, argv + 1);
  return 0;

}
