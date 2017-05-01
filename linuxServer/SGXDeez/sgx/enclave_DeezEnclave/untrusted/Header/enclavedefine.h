/*
 * enclavedefine.h
 *
 *  Created on: Aug 10, 2016
 *      Author: weijiawu
 */

#ifndef ENCLAVE_DEEZENCLAVE_UNTRUSTED_ENCLAVEDEFINE_H_
#define ENCLAVE_DEEZENCLAVE_UNTRUSTED_ENCLAVEDEFINE_H_

#include <unistd.h>
#include <pwd.h>
#include <libgen.h>
#include <stdlib.h>
#include <iostream>
#include <sgx_urts.h>

#include "DeezEnclave_u.h"

# define MAX_PATH FILENAME_MAX

typedef struct _sgx_errlist_t {
    sgx_status_t err;
    const char *msg;
    const char *sug; /* Suggestion */
} sgx_errlist_t;

void print_error_message(sgx_status_t ret);

int initialize_enclave(void);

void ocall_DeezEnclave_sample(const char *str);



#endif /* ENCLAVE_DEEZENCLAVE_UNTRUSTED_ENCLAVEDEFINE_H_ */
