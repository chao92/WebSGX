#ifndef ENCLAVE_DEEZENCLAVE_UNTRUSTED_SSL_SERVER_H_
#define ENCLAVE_DEEZENCLAVE_UNTRUSTED_SSL_SERVER_H_

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rsa.h>
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include "Header/Socket.h"

#define VERBOSE true // if show the communication info

#define CHK_NULL(x) if ((x)==NULL) exit (1)
#define CHK_ERR(err,s) if ((err)==-1) { perror(s); exit(1); }
#define CHK_SSL(err) if ((err)==-1) { ERR_print_errors_fp(stderr); exit(2); }

//get SSL_CTX and check the certificate and private key.
SSL_CTX * initilizeSSL(const char * certificate_file, const char * privateKey_file);

SSL* acceptSSL(int new_socketfd, SSL_CTX* ctx);

void disconnectSSL(SSL* ssl);

void freeCTX(SSL_CTX *ctx);

void* ReceiveData(void* s_c);

#endif
