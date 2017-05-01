#include "DeezEnclave_t.h"

#include "DataSealing.h"

#include "sgx_tae_service.h"
#include "sgx_trts.h"
#include "sgx_tseal.h"
#include "string.h"
#include "Header/common.h"


typedef struct _protected_secret_pack
{
	/* secret part */
	uint8_t secret[SEALING_BUFFER_SIZE];

	/* reserved */
	uint8_t reserved[SECRET_RESERVED_SIZE];

	/* index part */
	bool index_enabled;
	uint64_t index;

	/* MAC part */
	bool MAC_enabled;
	uint8_t MAC[SECRET_MAC_SIZE];

	/* timing part */
	bool time_enabled;
	sgx_time_source_nonce_t nonce;
    sgx_time_t timestamp_base;
    sgx_time_t lease_duration;
	
	/* replay part */
	bool replay_enabled;
	uint32_t release_version;
    uint32_t max_release_version;
	sgx_mc_uuid_t mc;
    uint32_t mc_value;

}protected_secret_pack;

uint32_t seal_data(const uint8_t *secret, uint32_t secret_len,
				   uint8_t *sealed_secret, uint32_t sealed_secret_len,
				   int64_t secret_index, const uint8_t *secret_MAC,
				   bool time_policy, bool replay_policy)
{
	uint32_t ret = 0;
	int busy_retry_times = PSE_SESSION_MAX_RETRY_TIME;
	protected_secret_pack data2seal;
	memset_s(&data2seal, sizeof(protected_secret_pack), 0, sizeof(protected_secret_pack));
    uint32_t size = sgx_calc_sealed_data_size(0, sizeof(protected_secret_pack));
    if(sealed_secret_len != size) 
        return SGX_ERROR_INVALID_PARAMETER;
	if(secret_len != SEALING_BUFFER_SIZE)
		return SGX_ERROR_INVALID_PARAMETER;
	if(time_policy || replay_policy)
	{
		do
		{
			ret = sgx_create_pse_session();
		}while (ret == SGX_ERROR_BUSY && busy_retry_times--);
		if (ret != SGX_SUCCESS)
			return ret;
	}
	do
	{
		if(secret_index >= 0)
		{
			data2seal.index_enabled = true;
			data2seal.index = secret_index;
		}
		else
			data2seal.index_enabled = false;
		if(secret_MAC != NULL)
		{
			data2seal.MAC_enabled = true;
			memcpy(data2seal.MAC,secret_MAC,SECRET_MAC_SIZE);
		}
		else
			data2seal.MAC_enabled = false;
		if(time_policy)
		{
			data2seal.time_enabled = true;
			ret = sgx_get_trusted_time(&data2seal.timestamp_base, &data2seal.nonce);
			if(ret != SGX_SUCCESS)
			{
				switch(ret)
				{
				case SGX_ERROR_SERVICE_UNAVAILABLE:
					/* Architecture Enclave Service Manager is not installed or not
					working properly.*/
					break;
				case SGX_ERROR_SERVICE_TIMEOUT:
					/* retry the operation*/
					break;
				case SGX_ERROR_BUSY:
					/* retry the operation later*/
					break;
				default:
					/*other errors*/
					break;
				}
				break;
			}
			data2seal.lease_duration = TIME_BASED_LEASE_DURATION_SECOND;
		}
		else
			data2seal.time_enabled = false;
		if(replay_policy)
		{
			data2seal.replay_enabled = true;
			ret = sgx_create_monotonic_counter(&data2seal.mc,&data2seal.mc_value);
        if(ret != SGX_SUCCESS)
        {
            switch(ret)
            {
            case SGX_ERROR_SERVICE_UNAVAILABLE:
                /* Architecture Enclave Service Manager is not installed or not
                working properly.*/
                break;
            case SGX_ERROR_SERVICE_TIMEOUT:
                /* retry the operation later*/
                break;
            case SGX_ERROR_BUSY:
                /* retry the operation later*/
                break;
            case SGX_ERROR_MC_OVER_QUOTA:
                /* SGX Platform Service enforces a quota scheme on the Monotonic
                Counters a SGX app can maintain. the enclave has reached the
                quota.*/
                break;
            case SGX_ERROR_MC_USED_UP:
                /* the Monotonic Counter has been used up and cannot create
                Monotonic Counter anymore.*/
                break;
            default:
                /*other errors*/
                break;
            }
            break;
        }

        data2seal.release_version = 0;
        /* the secret can be updated for 20 times */
        data2seal.max_release_version = 
            REPLAY_PROTECTED_PAY_LOAD_MAX_RELEASE_VERSION;
		}
		else
			data2seal.replay_enabled = false;

		memcpy(data2seal.secret,secret,secret_len);

		ret = sgx_seal_data(0, NULL,sizeof(protected_secret_pack),(uint8_t*)&data2seal,
				sealed_secret_len, (sgx_sealed_data_t*)sealed_secret);
	}while(0);


	/* remember to clear secret data after been used by memset_s */
    memset_s(&data2seal, sizeof(protected_secret_pack), 0, sizeof(protected_secret_pack));

	if(time_policy || replay_policy)
		sgx_close_pse_session();

	return ret;
}

uint32_t unseal_data(uint8_t *sealed_secret, uint32_t sealed_secret_len,
					 uint8_t *secret, uint32_t secret_len,
					 int64_t check_secret_index, const uint8_t *check_secret_MAC)
{
	uint32_t ret = 0;
	uint32_t ret_copy = 0;
	bool need_close = false;
    int busy_retry_times = PSE_SESSION_MAX_RETRY_TIME;
	protected_secret_pack temp_unsealed_data;
	memset_s(&temp_unsealed_data, sizeof(protected_secret_pack), 0, sizeof(protected_secret_pack));

	if(sealed_secret_len != sgx_calc_sealed_data_size(0, sizeof(protected_secret_pack))) 
        return SGX_ERROR_INVALID_PARAMETER;

	if(secret_len != SEALING_BUFFER_SIZE)
		return SGX_ERROR_INVALID_PARAMETER;

	uint32_t unseal_length = sizeof(protected_secret_pack);
	ret = sgx_unseal_data((sgx_sealed_data_t*)sealed_secret, NULL, 0, (uint8_t*)&temp_unsealed_data, &unseal_length);
	ret_copy = ret;
	if((temp_unsealed_data.time_enabled || temp_unsealed_data.replay_enabled) && ret)
	{
		need_close = true;
		do
		{
			ret = sgx_create_pse_session();
		}while (ret == SGX_ERROR_BUSY && busy_retry_times--);
		if (ret != SGX_SUCCESS)
			return ret;
	}
	do
    {
		if(ret_copy != SGX_SUCCESS)
		{
			switch(ret_copy)
			{
			case SGX_ERROR_MAC_MISMATCH:
				/* MAC of the sealed data is incorrect.
				The sealed data has been tampered.*/
				break;
			case SGX_ERROR_INVALID_ATTRIBUTE:
				/*Indicates attribute field of the sealed data is incorrect.*/
				break;
			case SGX_ERROR_INVALID_ISVSVN:
				/* Indicates isv_svn field of the sealed data is greater than
				the enclave ISVSVN. This is a downgraded enclave.*/
				break;
			case SGX_ERROR_INVALID_CPUSVN:
				/* Indicates cpu_svn field of the sealed data is greater than
				the platform cpu_svn. enclave is on a downgraded platform.*/
				break;
			case SGX_ERROR_INVALID_KEYNAME:
				/*Indicates key_name field of the sealed data is incorrect.*/
				break;
			default:
				/*other errors*/
				break;
			}
			break;
		}
		if(temp_unsealed_data.index_enabled == true)
		{
			if(check_secret_index < 0 || check_secret_index != temp_unsealed_data.index)
			{
				ret = INDEX_CHANGED;
				break;
			}
		}
		if(temp_unsealed_data.MAC_enabled == true)
		{
			if(check_secret_MAC == NULL)
			{
				ret = MAC_CHANGED;
				break;
			}
			if(memcmp(temp_unsealed_data.MAC,check_secret_MAC,SECRET_MAC_SIZE) != 0)
			{
				ret = MAC_CHANGED;
				break;
			}
		}
		if(temp_unsealed_data.time_enabled == true)
		{
			sgx_time_source_nonce_t nonce = {0};
			sgx_time_t current_timestamp;
			ret = sgx_get_trusted_time(&current_timestamp, &nonce);
			if(ret != SGX_SUCCESS)
			{
				switch(ret)
				{
				case SGX_ERROR_SERVICE_UNAVAILABLE:
					/* Architecture Enclave Service Manager is not installed or not
					working properly.*/
					break;
				case SGX_ERROR_SERVICE_TIMEOUT:
					/* retry the operation*/
					break;
				case SGX_ERROR_BUSY:
					/* retry the operation later*/
					break;
				default:
					/*other errors*/
					break;
				}
				break;
			}
			/*source nonce must be the same, otherwise time source is changed and
			the two timestamps are not comparable.*/
			if (memcmp(&nonce,&temp_unsealed_data.nonce, sizeof(sgx_time_source_nonce_t)))
			{
				ret = TIMESOURCE_CHANGED;
				break;
			}

			/* This should not happen. 
			SGX Platform service guarantees that the time stamp reading moves
			forward, unless the time source is changed.*/
			if(current_timestamp < temp_unsealed_data.timestamp_base)
			{
				ret = TIMESTAMP_UNEXPECTED;
				break;
			}
			/*compare lease_duration and timestamp_diff
			if lease_duration is less than difference of current time and base time,
			lease tern has expired.*/
			if(current_timestamp - temp_unsealed_data.timestamp_base > temp_unsealed_data.lease_duration)
			{
				ret = LEASE_EXPIRED;
				break;
			}
		}
		if(temp_unsealed_data.replay_enabled == true)
		{
			uint32_t mc_value_from_memory;
			ret = sgx_read_monotonic_counter(&temp_unsealed_data.mc,&mc_value_from_memory);
			if(ret != SGX_SUCCESS)
			{
				switch(ret)
				{
				case SGX_ERROR_SERVICE_UNAVAILABLE:
					/* Architecture Enclave Service Manager is not installed or not
					working properly.*/
						break;
				case SGX_ERROR_SERVICE_TIMEOUT:
					/* retry the operation later*/
						break;
				case SGX_ERROR_BUSY:
					/* retry the operation later*/
						break;
				case SGX_ERROR_MC_NOT_FOUND:
					/* the the Monotonic Counter ID is invalid.*/
						break;
				default:
					/*other errors*/
					break;
				}
				break;
			}
			if(mc_value_from_memory != temp_unsealed_data.mc_value)
			{
				ret = REPLAY_DETECTED;
				break;
			}
			ret = sgx_increment_monotonic_counter(&temp_unsealed_data.mc, &temp_unsealed_data.mc_value);
			if(ret != SGX_SUCCESS)
			{
				switch(ret)
				{
				case SGX_ERROR_SERVICE_UNAVAILABLE:
					/* Architecture Enclave Service Manager is not installed or not
					working properly.*/
					break;
				case SGX_ERROR_SERVICE_TIMEOUT:
					/* retry the operation*/
					break;
				case SGX_ERROR_BUSY:
					/* retry the operation later*/
					break;
				case SGX_ERROR_MC_NOT_FOUND:
					/* The Monotonic Counter was deleted or invalidated.
					This might happen under certain conditions.
					For example, the Monotonic Counter has been deleted, the SGX
					Platform Service lost its data or the system is under attack. */
					break;
				case SGX_ERROR_MC_NO_ACCESS_RIGHT:
					/* The Monotonic Counter is not accessible by this enclave.
					This might happen under certain conditions.
					For example, the SGX Platform Service lost its data or the
					system is under attack. */
					break;
				default:
					/*other errors*/
					break;
				}
				break;
			}
			if(temp_unsealed_data.release_version >= temp_unsealed_data.max_release_version)
			{
				/* the max release version has reached, cannot update. Delete the
				monotonic_counter, whether the deleting is successful or not. */
				(void)sgx_destroy_monotonic_counter(&temp_unsealed_data.mc);
				ret= MAX_RELEASE_REACHED;
				break;
			}
			temp_unsealed_data.release_version ++;
			ret = sgx_seal_data(0, NULL, sizeof(protected_secret_pack), (uint8_t*)&temp_unsealed_data,
            sealed_secret_len, (sgx_sealed_data_t*)sealed_secret);
		}
		memcpy(secret, temp_unsealed_data.secret, secret_len);
	} while (0);

	memset_s(&temp_unsealed_data, sizeof(protected_secret_pack), 0, sizeof(protected_secret_pack));
	if(need_close)
	{
		sgx_close_pse_session();
	}
    return ret;
}

uint32_t delete_sealed_conter(const uint8_t *sealed_secret, uint32_t sealed_secret_len)
{
	uint32_t ret = 0;
    int busy_retry_times = PSE_SESSION_MAX_RETRY_TIME;
    protected_secret_pack data_unsealed;
    if(sealed_secret_len != sgx_calc_sealed_data_size(0, sizeof(protected_secret_pack))) 
        return SGX_ERROR_INVALID_PARAMETER;
    do{
        ret = sgx_create_pse_session();
    }while (ret == SGX_ERROR_BUSY && busy_retry_times--);
    if (ret != SGX_SUCCESS)
        return ret;
	do
    {
		uint32_t unseal_length = sizeof(protected_secret_pack);
        ret = sgx_unseal_data((sgx_sealed_data_t*)sealed_secret, NULL, 0, (uint8_t*)&data_unsealed, &unseal_length);
        if(ret != SGX_SUCCESS)
            break;
        ret = sgx_destroy_monotonic_counter(&data_unsealed.mc);
        if(ret != SGX_SUCCESS)
        {
            switch(ret)
            {
            case SGX_ERROR_SERVICE_UNAVAILABLE:
                /* Architecture Enclave Service Manager is not installed or not
                working properly.*/
                break;
            case SGX_ERROR_SERVICE_TIMEOUT:
                /* retry the operation later*/
                break;
            case SGX_ERROR_BUSY:
                /* retry the operation later*/
                break;
            case SGX_ERROR_MC_NOT_FOUND:
                /* the the Monotonic Counter ID is invalid.*/
                break;
            case SGX_ERROR_MC_NO_ACCESS_RIGHT:
                /* the Monotonic Counter is not accessible by this enclave.
                This might happen under certain conditions.
                For example, the SGX Platform Service lost its data or
                the system is under attack. */
                break;
            default:
                /*other errors*/
                break;
            }
        }
    } while (0);
    /* remember to clear secret data after been used by memset_s */
    memset_s(&data_unsealed, sizeof(protected_secret_pack), 0, sizeof(protected_secret_pack));
    sgx_close_pse_session();
    return ret;
}

uint32_t get_sealed_secret_buffer_size()
{
	return sgx_calc_sealed_data_size(0, sizeof(protected_secret_pack));
}


////FUNCTIONS BELOW ARE ONLY FOR DEBUG.
//
//void ecall_seal_data(const uint8_t *secret, uint32_t secret_len,
//				   uint8_t *sealed_secret, uint32_t sealed_secret_len,
//				   int64_t secret_index,uint8_t *secret_MAC, uint32_t MAC_len,
//				   uint8_t time_policy, uint8_t replay_policy)
//{
//	sgx_status_t this_ret = (sgx_status_t)seal_data(secret,secret_len,sealed_secret,sealed_secret_len,secret_index,secret_MAC,(time_policy == 1),(replay_policy == 1));
//	if(this_ret != SGX_SUCCESS)
//	{
//		ocall_print_string("sealed data failed, error code = ");
//		//ocall_print_number_in_hex((int)this_ret);
//		ocall_print_string("\r\n");
//	}
//	return;
//}
//
//void ecall_unseal_data(uint8_t *sealed_secret, uint32_t sealed_secret_len,
//					 uint8_t *secret, uint32_t secret_len,
//					 int64_t check_secret_index, const uint8_t *check_secret_MAC, uint32_t check_MAC_len)
//{
//	sgx_status_t this_ret = (sgx_status_t)unseal_data(sealed_secret,sealed_secret_len,secret,secret_len,check_secret_index,check_secret_MAC);
//	if(this_ret != SGX_SUCCESS)
//	{
//		ocall_print_string("sealed data failed, error code = ");
//		//ocall_print_number_in_hex((int)this_ret);
//		ocall_print_string("\r\n");
//	}
//	return;
//}

//void ecall_get_secret_buffer_size(uint32_t *size)
//{
//	size[0] = SEALING_BUFFER_SIZE;
//	return;
//}
//
//void ecall_get_sealed_secret_buffer_size(uint32_t *size)
//{
//	size[0] = get_sealed_secret_buffer_size();
//	return ;
//}
//
//void ecall_get_secret_MAC_buffer_size(uint32_t *size)
//{
//	size[0] = SECRET_MAC_SIZE;
//}
//
//void ecall_get_random_64_bytes(uint8_t *buffer)
//{
//	sgx_read_rand(buffer,64);
//	return;
//}
