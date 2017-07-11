#include"ssl_server.h"

#include <iostream>
#include "common.h"
#include "Server.h"

//get SSL_CTX and check the certificate and private key.
SSL_CTX * initilizeSSL(const char * certificate_file,const char * privateKey_file){
	SSL_load_error_strings();
	SSL_library_init();
	OpenSSL_add_all_algorithms();

	SSL_CTX * ctx = SSL_CTX_new(SSLv23_server_method());

	if (!ctx) {
		ERR_print_errors_fp(stderr);
		exit(2);
	}

	if (SSL_CTX_use_certificate_file(ctx, certificate_file, SSL_FILETYPE_PEM) <= 0) {
		ERR_print_errors_fp(stderr);
		exit(3);
	}

	if (SSL_CTX_use_PrivateKey_file(ctx, privateKey_file, SSL_FILETYPE_PEM) <= 0) {
		ERR_print_errors_fp(stderr);
		exit(4);
	}
	/* check the private key */
	if (!SSL_CTX_check_private_key(ctx)) {
		fprintf(stderr,"Private key does not match the certificate public key\n");
		exit(5);
	}

	return ctx;
}

SSL_CTX * initilizeSSL1(char * certificate_file, char * privateKey_file){
	SSL_library_init();

	OpenSSL_add_all_algorithms();

	SSL_load_error_strings();

	SSL_CTX * ctx = SSL_CTX_new(SSLv23_server_method());
	if (ctx == NULL) {
		ERR_print_errors_fp(stdout);
		exit(1);
	}

	if (SSL_CTX_use_certificate_file(ctx, certificate_file, SSL_FILETYPE_PEM) <= 0) {
		ERR_print_errors_fp(stdout);
		exit(1);
	}

	if (SSL_CTX_use_PrivateKey_file(ctx, privateKey_file, SSL_FILETYPE_PEM) <= 0) {
		ERR_print_errors_fp(stdout);
		exit(1);
	}
	/* check the private key */
	if (!SSL_CTX_check_private_key(ctx)) {
		ERR_print_errors_fp(stdout);
		exit(1);
	}

	return ctx;
}

SSL* acceptSSL(int new_socketfd, SSL_CTX* ctx){
	  SSL* ssl = SSL_new (ctx);                           CHK_NULL(ssl);
	  SSL_set_fd (ssl, new_socketfd);
	  int err = SSL_accept (ssl);                        CHK_SSL(err);

	  return ssl;
	/* construct a new SSL based on ctx*/
	//SSL* ssl = SSL_new(ctx);
	/* add the client socket to SSL */
	//SSL_set_fd(ssl, new_socketfd);

	//if (SSL_accept(ssl) == -1) {
	//	return NULL;
	//}else{
	//	return ssl;
	//}
	
}

void disconnectSSL(SSL* ssl){
	SSL_shutdown(ssl);
	SSL_free(ssl);
}

void freeCTX(SSL_CTX *ctx){
	SSL_CTX_free(ctx);
}

void* ReceiveData(void* receive_data_parameter)
{
	//getchar();
#if defined FUNCTION_LEVEL_PROFILE
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
#endif
	S_C socket_client = *(((receive_data_parameter_struct*)receive_data_parameter)->s_c);
	//Socket *server_socket = ((receive_data_parameter_struct*)receive_data_parameter)->socket;

	// new code for web client sync.
	Socket *data_exchange_socket = ((receive_data_parameter_struct*)receive_data_parameter)->socket;
	// end of new code for web client sync.

	int socket_fd = socket_client.socket_fd;

	// new code for webclient
	//socket_fd = server_socket->Accept();
	//cout<<"receive msg4 data with socket_df"<<socket_fd<<endl;
	// end of code for webclient


	// new code for webclient sync.
	socket_fd = data_exchange_socket->Accept();
	cout<<"receive msg4 data with socket_df"<<socket_fd<<endl;
	// end of for webclient sync.


	int client_id = socket_client.client_id;
	char username[128];
	strcpy(username, socket_client.username);//before is strcpy_s, changed by wwj

	std::cout <<"SERVER: Request data from client: [" << username <<"] " <<"confirmed!" <<std::endl;

	char msg[] = "data";
	data_exchange_socket->Send(socket_fd, msg, strlen(msg)+1);

	std::cout <<"SERVER: Start Receiving Data From client: [" << username <<"]" <<std::endl;
	//receive msg4
	char *msg4;
	MESSAGE_LENGTH_TYPE length;
	length = 0;
	MESSAGE_LENGTH_TYPE pos;
	pos = 0;
	MESSAGE_LENGTH_TYPE recvLength;

	while(true)
	{
		if (!length)
		{
//			printf("MESSAGE_LENGTH_TYPE is %d\n",sizeof(MESSAGE_LENGTH_TYPE);

			if(data_exchange_socket->Recv(socket_fd, (char*)&length,sizeof(MESSAGE_LENGTH_TYPE))!=sizeof(MESSAGE_LENGTH_TYPE))
			{
				printf("SERVER: Recv Error! Error code: %d\n", errno);
				printf("Error description is : %s\n", strerror(errno));
				return 0;
			}

			msg4 = new char[length];

			printf("Server received the length of msg4 is %lld\n", length);
		}
		else
		{
			while (pos < length)
			{
				recvLength = data_exchange_socket->Recv(socket_fd, msg4+pos,length-pos);

				if (recvLength < 0)
				{
					printf("SERVER: Recv Error! Error code: %d\n", errno);
					printf("Error description is : %s\n", strerror(errno));
					return 0;
				}
				//else
				//{
				//	reply(&this->server_socket, true);
				//}

				pos += recvLength;
//				printf("\rThe receiving process: %.2f%%",pos/(double)length*100);
			}
			std::cout<<std::endl;
			break;
		}
	}

	std::cout << "SERVER: MSG4 received form client: [" << username <<"]!" <<std::endl;
#if defined FUNCTION_LEVEL_PROFILE
	//duration.receive_msg4[client_id] = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - start);
	cout << "The running time for transfer data is: "
		<< chrono::duration<double, milli>(std::chrono::high_resolution_clock::now() - start).count() << " ms\n";
#endif
	return msg4;
}

