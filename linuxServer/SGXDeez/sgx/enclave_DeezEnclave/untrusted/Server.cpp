#include <Server.h>
#include <errno.h>
#include <sgx_error.h>
//#include <unistd.h>
#include "Header/common.h"
#include "Header/ssl_server.h"
#include "Header/attestation.h"
#include "Header/network_ra.h"
#include "Header/remote_attestation_result.h"
#include "Header/DataProcess.h"
#include "FileIO.h"
#include "Reference.h"
#include <time.h>
//#include <p>
//#define PROFILE_DEEZ
#define PROFILE_DEEZ1
//#include <chrono>
//#include <ctime>

bool optTest = false;
bool optSort = false;
bool optForce = false;
bool optStdout = false;
bool optStats = false;
bool optNoQual = false;
bool optReadLossy = false;
bool optInvalidChr = false;
size_t optBlock = 1000; //10000;
char optQuality = 0;
char optLossy = 0;
int optThreads = 4;
int optFlag = 0;
int optLogLevel = 2;
bool optBzip = false;
size_t optSortMemory = GB;
string optRef = "";
vector<string> optInput;
string optRange = "";
string optOutput = "";
vector<FILE*> samFiles;
static File* sealedFile = NULL;
static File* inFile = NULL;

#ifndef SAFE_FREE
#define SAFE_FREE(ptr)       \
    {                        \
        if (NULL != (ptr)) { \
            free(ptr);       \
            (ptr) = NULL;    \
        }                    \
    }
#endif

/* OCall functions to dump dump*/
void ocall_print_string(const char* str)
{
    /* Proxy/Bridge will check the length and null-terminate
	* the input string to prevent buffer overflow.
	*/
    printf("%s", str);
}

Socket serverSocket;
string folder_name;
Reference* reference;

void ocall_reference_getCharAt(int pos, char* out)
{
    *out = reference->getCharAt(pos);
}

void ocall_print2file1(const char* s, int fileID)
{
    string fn = optOutput;

    if (fileID > 0)
        fn += S("_%d", fileID);
    if (samFiles.size() < (fileID + 1)) {
        samFiles.push_back(fopen(fn.c_str(), "wb"));
        if (samFiles[fileID] == NULL) {
            printf("Cannot open the file %s", fn.c_str());
            abort();
            //			throw DZException("Cannot open the file %s", fn.c_str());
        }
    }

    fputs(s, samFiles[fileID]);
}

void ocall_reference_scanChromosome(const char* s, char** out)
{
    string tmp = reference->scanChromosome(string(s));
    char* buffer = (char*)calloc(1, tmp.size() + 1);
    memcpy(buffer, tmp.data(), tmp.size());
    buffer[tmp.size()] = '\0';
    *out = buffer;
}

void ocall_reference_copy_trim(size_t start, size_t end, char** out)
{
    string tmp = reference->copy(start, end);
    char* buffer = (char*)calloc(1, tmp.size() + 1);
    memcpy(buffer, tmp.data(), tmp.size());
    buffer[tmp.size()] = '\0';
    *out = buffer;
    reference->trim(start);
}

int ocall_fetchBuffer(uint8_t* buffer, size_t size, size_t start)
{
    char* temp = new char[size];

    if (!sealedFile) {
        printf("error loading file!!!\n");
        return -1;
    }
    else {
        fseek((FILE*)sealedFile->handle(), start, SEEK_SET);
        fread(buffer, 1, size, (FILE*)sealedFile->handle());
        return 0;
    }
}

void decompressSealedDZ(string sealed_deez_file, size_t inFileSz, string optRef, int DZ_order, char* sealedDataMAC, sgx_enclave_id_t eid)
{
    cout << "Decompressing dz with sealed dz file..." << endl;
    sealedFile = File::Open(sealed_deez_file.c_str(), "rb");
    size_t inSealedFileSz = sealedFile->size();
    cout << "sealed FileSz: " << inSealedFileSz << endl;

    reference = new Reference(optRef);

    ecallHere(eid, inFileSz, DZ_order, sealedDataMAC, SECRET_MAC_SIZE);
    //inFile->close();
    sealedFile->close();
    delete reference;
    reference = NULL;
}

void sendBackMessage(Socket* serverSocket, int socketID, double dist, sgx_ra_context_t context, sgx_enclave_id_t eid)
{
    char* response = new char[sizeof(double)];
    char* encryptedResponse = new char[sizeof(double) + ENCRYPTED_ADD_SIZE];

    memmove(response, &dist, sizeof(double));
    encryptData(eid, context, (uint8_t*)response, sizeof(double),
        (uint8_t*)encryptedResponse, sizeof(double) + ENCRYPTED_ADD_SIZE);

    serverSocket->Send(socketID, encryptedResponse, sizeof(double) + ENCRYPTED_ADD_SIZE);

    delete[] response;
    delete[] encryptedResponse;
}

Server::Server(Config* new_client_information)
    : token({ 0 })
    , updated(0)
{
    //    char buf[80];
    //    getcwd(buf, sizeof(buf));
    //    printf("current working directory : %s\n", buf);
    // TODO Auto-generated constructor stub
    this->client_information = new_client_information;
    context = new sgx_ra_context_t[StringToInt(this->client_information->Read(Config::KeyName[0]))];

    //	this->token = {0};
    //	this->updated = 0;

    ret = sgx_create_enclave(ENCLAVE_FILE,
        SGX_DEBUG_FLAG,
        &(this->token),
        &(this->updated),
        &(this->eid), NULL);

    if (SGX_SUCCESS != ret) {
        printf("\nError %#x, call sgx_create_enclave fail [%s].", ret, __FUNCTION__);
        exit(EXIT_FAILURE);
    }
    else {
        printf("\nCall sgx_create_enclave success.\n");
    }
}

Server::~Server()
{
    // TODO Auto-generated destructor stub
    delete context;
}

void Server::TimeStart()
{
    gettimeofday(&this->start, NULL);
}

void Server::StartListening(void)
{
    if (!server_socket.Connect(0, this->client_information->GetPort(), 0)) {
        printf("SERVER: Fail to Listen!\n");
        exit(EXIT_FAILURE);
    }

    else {
        printf("SERVER: Start Listening!\n");
    }
}

void Server::StartDataExchangeListening(void)
{
    if (!data_exchange_socket.Connect(0, this->client_information->GetPort() + 1, 0)) {
        printf("SERVER Data exchange port : Fail to Listen!\n");
        exit(EXIT_FAILURE);
    }
    else {
        printf("SERVER Data exchange port: Start Listening!\n");
    }
}

//bool Server::InitializeCommunication(void)
//{
//	bool *authResult = new bool[this->client_information->GetAccountCount()];
//	for (int i=0; i<this->client_information->GetAccountCount(); i++)
//	{
//		authResult[i] = false;
//	}
//
//	pthread_t *t_client = (pthread_t *)malloc(this->client_information->GetClientNumber()*sizeof(pthread_t));
//
//	ra_samp_response_header_t **pp_att_result_msg_full = new ra_samp_response_header_t*[this->client_information->GetClientNumber()];
//
//	S_C *socket_client = new S_C[(this->client_information)->GetClientNumber()];
//	this->server_socket.setSSLenable(this->client_information->GetSSL());
//	SSL_CTX *ctx;
//
//	if(this->client_information->GetSSL())
//	{
//		ctx = initilizeSSL(this->Certificate_File.c_str(), this->Private_Key_File.c_str());
//	}
//	this->StartListening();
//
//	// new code for webclient start listening data_exchange_socket on port 7892
//			//
//			this->StartDataExchangeListening();
//			// new code for webclient
//
//	// if(!server_socket.Connect(0, this->client_information->GetPort(), 0))
//	// {
//	// 	printf("SERVER: Fail to Listen!\n");
//	// 	exit(EXIT_FAILURE);
//	// }
//
//	// else
//	// {
//	// 	printf("SERVER: Start Listening!\n");
//	// }
////	this->ReListening();
//
//	for (int i=0; i<this->client_information->GetClientNumber(); i++)
//	{
//
//		cout<<"client number is "<<this->client_information->GetClientNumber()<<endl;
//		socket_client[i].socket_fd = server_socket.Accept();
//		cout<<"the user  "<<i<<" socket_fd is "<<socket_client[i].socket_fd<<endl;
//
//		//SSL setting
//		char buf_SSL[5];
//		int bytes = server_socket.RecvInitInfo (socket_client[i].socket_fd, buf_SSL, 5);
//		if ((bytes >= 0) && (!strcmp(buf_SSL, "SSL?")))
//		{
//			if (this->client_information->GetSSL())
//			{
//				strcpy(buf_SSL,"SSL!");
//				server_socket.SendInitInfo(socket_client[i].socket_fd, buf_SSL, 5);
//			}
//			else
//			{
//				strcpy(buf_SSL,"nSSL");
//				server_socket.SendInitInfo(socket_client[i].socket_fd, buf_SSL, 5);
//			}
//		}
//		printf("SERVER: SSL settings sent!\n");
//
//		if (this->client_information->GetSSL())
//		{
//			SSL *ssl = acceptSSL(socket_client[i].socket_fd, ctx);
//			server_socket.setSSLpair(ssl, socket_client[i].socket_fd);
//		}
//
//		//authentication
//		printf("SERVER: Start Authentication!\n");
//		char msg[] = "auth";
//		server_socket.Send(socket_client[i].socket_fd, msg, strlen(msg)+1);
//
//		int length = 0;
//		int pos = 0;
//		char *buf;
//		int recvLength;
//		while(true)
//		{
//			if (!length)
//			{
//				if(server_socket.Recv(socket_client[i].socket_fd, (char*)&length,4)!=4)
//				{
//					printf("SERVER: Recv Error! Error code: %d\n", errno);
//					printf("Error description is : %s\n", strerror(errno));
//					exit(EXIT_FAILURE);
//				}
//				printf("get username and password length is : %d\n", length);
//				buf = new char[length];
//			}
//			else
//			{
//				printf("program running in getting username and password");
//				while (pos < length)
//				{
//					printf("receiving start with pos is %d with length is%d\n", pos, length-pos);
//					recvLength = server_socket.Recv(socket_client[i].socket_fd, buf+pos,length-pos);
//					if (recvLength < 0)
//					{
//						printf("SERVER: Recv Error! Error code: %d\n", errno);
//						printf("Error description is : %s\n", strerror(errno));
//						exit(EXIT_FAILURE);
//					}
//					pos += recvLength;
//					printf("received length is %d updated pos is%d\n", recvLength, pos);
//				}
//				break;
//			}
//		}
//		if (length >= 0)
//		{
//			//split username and password
//			int split_pos = 0;
//			for (int j=0; j<length; j++)
//			{
//				printf("%c ",buf[j]);
//				if (buf[j] == 0)
//				{
//					split_pos = j+1;
//					printf("splite position is \%d\n", split_pos);
//					break;
//				}
//			}
//			//compare and authorize
//			int authSuccess = 0;
//			for (int j=0; j<this->client_information->GetAccountCount(); j++)
//			{
//				if (!strcmp(buf,this->client_information->GetUsername(j).c_str()))
//				{
//					if (!strcmp(buf+split_pos,this->client_information->GetPassword(j).c_str()))
//					{
//						if (authResult[j])
//							break;
//
//						authResult[j] = true;
//						authSuccess = 1;
//						socket_client[i].client_id = j;
//						strcpy(socket_client[i].username, this->client_information->GetUsername(j).c_str());
//						break;
//					}
//				}
//			}
//			if (!authSuccess)
//			{
//				printf("No such username password combination.\n");
//				continue;
//			}
//		}
//
//		printf("SERVER: Authentication Pass!\n");
//
////		int temp_data = 0;
////		if(this->client_information->GetAlgorithm() == 0)
////		{
////			temp_data = this->client_information->GetSegmentLength();
////			server_socket.Send( socket_client[i].socket_fd, (char*)&temp_data, sizeof(int));
////			temp_data = this->client_information->GetCompression();
////			cout<<"compress flag is"<<temp_data<<"sizeof(int)"<<sizeof(int)<<endl;
////			server_socket.Send( socket_client[i].socket_fd, (char*)&temp_data, sizeof(int));
////		}
////		temp_data = this->client_information->GetRequestSummary();
////		cout<<"request summery data is "<<temp_data<<endl;
////		server_socket.Send( socket_client[i].socket_fd, (char*)&temp_data, sizeof(int));
//
//		//attestation
//		cout <<"SERVER: Start Attestation to client: [" << this->client_information->GetUsername(socket_client[i].client_id) <<"]" <<endl;
//
//		ret = attestation(this->eid, &(this->context[i]), this->status, &server_socket, &socket_client[i].socket_fd, &socket_client[i].client_id);
//
//
//
//	#if defined FUNCTION_LEVEL_PROFILE
//		duration.attestation[socket_client[i].client_id] = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - start);
//	#endif
//
//		if (ret == 1)
//		{
//			cout << "SERVER: Attestation Report to client: [" << this->client_information->GetUsername(socket_client[i].client_id) <<"]" <<endl;
//		}
//
////		receive_data_parameter_struct receive_data_parameter;
////		receive_data_parameter.s_c = &(socket_client[i]);
////		receive_data_parameter.socket = &(server_socket);
//
//	// new code for web client sync.
//				receive_data_parameter_struct receive_data_parameter;
//				receive_data_parameter.s_c = &(socket_client[i]);
//				receive_data_parameter.socket = &(data_exchange_socket);
//				// end of new code for web client sync.
//
//		pthread_create(&t_client[i], NULL, &ReceiveData, &(receive_data_parameter));
//		cout<<"the next user start is "<<i<<"and this->client_information->GetClientNumber() is "<<this->client_information->GetClientNumber()<<endl;
//	}
//
//	//wait
//	for (int i=0; i<this->client_information->GetClientNumber(); i++)
//	{
//		cout<<"pthread waiting ---------"<<endl;
//		pthread_join (t_client[i], (void **)&(pp_att_result_msg_full[i]));
//
//		//pthread_join (t_client[i], (void **)&genome_data[i]);
//	}
//
//
//	char **data;
//	int data_size;
//	char *p_gcm_mac;
//
//	//check clients' responses, re-align data and macs
//	for ( int i = 0; i < this->client_information->GetClientNumber(); i ++)
//	{
//		sample_ra_att_result_msg_t * p_att_result_msg_body =
//			(sample_ra_att_result_msg_t *)((uint8_t*)pp_att_result_msg_full[i]
//		+ sizeof(ra_samp_response_header_t));
//
//		if(TYPE_RA_ATT_RESULT != pp_att_result_msg_full[i]->type)
//		{
//			ret = -1;
//			printf("\nError. Sent MSG3 successfully, but the message "
//				"received was NOT of type att_msg_result. Type = "
//				"%d. [%s].", pp_att_result_msg_full[i]->type,
//				__FUNCTION__);
//			exit (-1);
//		}
//
//#if defined DUMP_LOG
//		PRINT_BYTE_ARRAY(OUTPUT, pp_att_result_msg_full[i]->body,
//			pp_att_result_msg_full[i]->size);
//#endif
//
//		// Check the MAC using MK on the attestation result message.
//		// The format of the attestation result message is ISV specific.
//		// This is a simple form for demonstration. In a real product,
//		// the ISV may want to communicate more information.
//
//#if defined FUNCTION_LEVEL_PROFILE
//		start = std::chrono::high_resolution_clock::now();
//#endif
//		ret = verify_att_result_mac(this->eid,
//			&status,
//			context[i],
//			(uint8_t*)&p_att_result_msg_body->platform_info_blob,
//			sizeof(ias_platform_info_blob_t),
//			(uint8_t*)&p_att_result_msg_body->mac,
//			sizeof(sgx_mac_t));
//
//#if defined FUNCTION_LEVEL_PROFILE
//		duration.verify_attstation[socket_client[i].client_id] = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - start);
//#endif
//
//		if((SGX_SUCCESS != ret) ||
//			(SGX_SUCCESS != status))
//		{
//			ret = -1;
//			printf("\nError: INTEGRITY FAILED - attestation result "
//				"message MK based cmac failed in [%s].",
//				__FUNCTION__);
//			exit (-1);
//		}
//
//		bool attestation_passed = true;
//		// Check the attestation result for pass or fail.
//		// @TODO:  Check the status.  This is ISV defined.
//		if(0 != pp_att_result_msg_full[i]->status[0]
//		|| 0 != pp_att_result_msg_full[i]->status[1])
//		{
//			printf("\nError, attestation result message MK based cmac "
//				"failed in [%s].", __FUNCTION__);
//			attestation_passed = false;
//		}
//
//		if (attestation_passed)
//		{
//			cout << endl << "SERVER: MSG4 from client: [" << this->client_information->GetUsername(socket_client[i].client_id)
//					<<"] " <<"confirmed." <<endl;
//		}
//
//		//align data at erver's platform
//		if(attestation_passed)
//		{
//			if (!i)
//			{
//				data_size = p_att_result_msg_body->secret.payload_size;
//				cout<<"the recived secret size should be 8 "<<data_size<<endl;
//				data = new char*[this->client_information->GetClientNumber()];
//				//p_gcm_mac = new char[mac_size];
//			}
//			data[i] = new char[p_att_result_msg_body->secret.payload_size];
//			memcpy(data[i], (char *)p_att_result_msg_body->secret.payload,p_att_result_msg_body->secret.payload_size);
//			//memcpy(p_gcm_mac+i*16, p_att_result_msg_body->secret.payload_tag, 16);
//			ret = put_secret_data(eid,&status,this->context[i],
//					p_att_result_msg_body->secret.payload,
//					p_att_result_msg_body->secret.payload_size,
//					p_att_result_msg_body->secret.payload_tag);
//			if((SGX_SUCCESS != ret)  || (SGX_SUCCESS != status))
//			{
//				fprintf(stdout, "\nError, attestation result message secret "
//								                                "using SK based AESGCM failed in [%s]. ret = "
//								                                "0x%0x. status = 0x%0x", __FUNCTION__, ret,
//								                                 status);
//			}
//			fprintf(stdout, "\nSecret successfully received from server.");
//			fprintf(stdout, "\nRemote attestation success!");
//
////			data[i] = new char[p_att_result_msg_body->secret.payload_size];
////			memcpy(data[i], (char *)p_att_result_msg_body->secret.payload,p_att_result_msg_body->secret.payload_size);
////			//memcpy(p_gcm_mac+i*16, p_att_result_msg_body->secret.payload_tag, 16);
////
////			ret = put_secret_data(eid,&status,this->context[0],
////					p_att_result_msg_body->secret.payload,
////					p_att_result_msg_body->secret.payload_size,
////					p_att_result_msg_body->secret.payload_tag);
////				            if((SGX_SUCCESS != ret)  || (SGX_SUCCESS != status))
////				            {
////				                fprintf(stdout, "\nError, attestation result message secret "
////				                                "using SK based AESGCM failed in [%s]. ret = "
////				                                "0x%0x. status = 0x%0x", __FUNCTION__, ret,
////				                                 status);
////				            }
////				            fprintf(stdout, "\nSecret successfully received from server.");
////				            fprintf(stdout, "\nRemote attestation success!");
//		}
//
//		cout << "SERVER: Secret successfully received from client: ["
//				<< this->client_information->GetUsername(socket_client[i].client_id) <<"]" <<endl;
//	}
//
//#if defined FUNCTION_LEVEL_PROFILE
//	start = std::chrono::high_resolution_clock::now();
//#endif
//
//}

bool Server::StartCommunication(void)
{
    bool* authResult = new bool[this->client_information->GetAccountCount()];
    for (int i = 0; i < this->client_information->GetAccountCount(); i++) {
        authResult[i] = false;
    }

    FILE* OUTPUT = stdout;

    // point to msg4 for verify
    ra_samp_response_header_t** pp_att_result_msg_full = new ra_samp_response_header_t*[this->client_information->GetClientNumber()];

    // point to msg1 for memory management
    ra_samp_request_header_t** pp_ra_msg1_full = new ra_samp_request_header_t*[this->client_information->GetClientNumber()];

    // socket_df, client_id, username for each client
    S_C* socket_client = new S_C[(this->client_information)->GetClientNumber()];
    int current_client_id;

    // point to msg3 for send out
    ra_samp_request_header_t** pp_ra_msg3_full = new ra_samp_request_header_t*[this->client_information->GetClientNumber()];

    // Start listening, server ready to accept attestation request
    this->server_socket.setSSLenable(this->client_information->GetSSL());
    this->StartListening();

    // each client will connect to server 4 times sequentially, the totoal accept time is 4*client_number
    for (int i = 0, client_index = 0; i < 4 * (this->client_information)->GetClientNumber(); ++i) {
        // each time, after accept, send msg type, then client username.
        // processing following message based on above information
        // 4 msg type
        // 0-"MSG1" authenticate the useranme and password and send msg1 to the client
        // 1-"MSG2" accept msg2 from client,
        // 2-"MSG3"	send msg3 out
        // 3-"MSG4" rec msg4, finish attestation

        int current_socket_fd = server_socket.Accept();
        int MSG_type;

        if (server_socket.RecvInitInfo(current_socket_fd, (char*)&MSG_type, 4) != 4) {
            printf("SERVER: Recv MSG Type Error!");
        }
        else {

            switch (MSG_type) {

            case 0: {
                //SSL setting
                char buf_SSL[5];
                printf("SERVER: SSL settings Recv!\n");
                int bytes = server_socket.RecvInitInfo(current_socket_fd, buf_SSL, 5);
                if ((bytes >= 0) && (!strcmp(buf_SSL, "SSL?"))) {
                    if (this->client_information->GetSSL()) {
                        strcpy(buf_SSL, "SSL!");
                        server_socket.SendInitInfo(current_socket_fd, buf_SSL, 5);
                    }
                    else {
                        strcpy(buf_SSL, "nSSL");
                        server_socket.SendInitInfo(current_socket_fd, buf_SSL, 5);
                    }
                }
                printf("SERVER: SSL settings sent!\n");

                //authentication
                printf("SERVER: Start Authentication!\n");
                char msg[] = "auth";
                server_socket.Send(current_socket_fd, msg, strlen(msg) + 1);

                int length = 0;
                int pos = 0;
                char* buf;
                int recvLength;
                while (true) {
                    if (!length) {
                        if (server_socket.Recv(current_socket_fd, (char*)&length, 4) != 4) {
                            printf("SERVER: Recv Error! Error code: %d\n", errno);
                            printf("Error description is : %s\n", strerror(errno));
                            exit(EXIT_FAILURE);
                        }
                        printf("get username and password length is : %d\n", length);
                        buf = new char[length];
                    }
                    else {
                        printf("program running in getting username and password");
                        while (pos < length) {
                            printf("receiving start with pos is %d with length is%d\n", pos, length - pos);
                            recvLength = server_socket.Recv(current_socket_fd, buf + pos, length - pos);
                            if (recvLength < 0) {
                                printf("SERVER: Recv Error! Error code: %d\n", errno);
                                printf("Error description is : %s\n", strerror(errno));
                                exit(EXIT_FAILURE);
                            }
                            pos += recvLength;
                            printf("received length is %d updated pos is%d\n", recvLength, pos);
                        }
                        break;
                    }
                }
                if (length >= 0) {
                    //split username and password
                    int split_pos = 0;
                    for (int j = 0; j < length; j++) {
                        printf("%c ", buf[j]);
                        if (buf[j] == 0) {
                            split_pos = j + 1;
                            printf("split position is %d\n", split_pos);
                            break;
                        }
                    }
                    //compare and authorize
                    int authSuccess = 0;
                    for (int j = 0; j < this->client_information->GetAccountCount(); j++) {
                        if (!strcmp(buf, this->client_information->GetUsername(j).c_str())) {
                            if (!strcmp(buf + split_pos, this->client_information->GetPassword(j).c_str())) {
                                if (authResult[j])
                                    break;
                                authResult[j] = true;
                                authSuccess = 1;
                                socket_client[client_index].client_id = j;
                                strcpy(socket_client[client_index].username, this->client_information->GetUsername(j).c_str());
                                break;
                            }
                        }
                    }
                    if (!authSuccess) {
                        printf("No such username password combination.\n");
                        continue;
                    }
                }
                printf("SERVER: Authentication Pass!\n");

                //Attestation send out msg1 to client
                printf("SERVER: Start Attestation send out msg1!\n");
                socket_client[client_index].socket_fd = current_socket_fd;
                // 0 success, otherwise, fail
                int current_client_index = client_index;
                int ret = attestation_sendMSG1(this->eid, &(this->context[current_client_index]), this->status, &server_socket, socket_client[current_client_index].socket_fd, socket_client[current_client_index].client_id, &(pp_ra_msg1_full[current_client_index]));
                client_index++;
                if (ret != 0) {
                    if (INT_MAX != *context) {
                        int ret_save = ret;
                        ret = enclave_ra_close(this->eid, &this->status, this->context[current_client_index]);
                        if (SGX_SUCCESS != ret || status) {
                            ret = -1;
                            fprintf(OUTPUT, "\nError, call enclave_ra_close fail [%s].",
                                __FUNCTION__);
                        }
                        else {
                            // enclave_ra_close was successful, let's restore the value that
                            // led us to this point in the code.
                            ret = ret_save;
                        }
                        fprintf(OUTPUT, "\nCall enclave_ra_close success.");
                    }
                    sgx_destroy_enclave(this->eid);
                    SAFE_FREE(pp_ra_msg1_full[current_client_index]);
                }

                break;
            }

            case 1: {
                if (server_socket.Recv(current_socket_fd, (char*)&current_client_id, 4) != 4) {
                    printf("SERVER: Recv Error! Error code: %d\n", errno);
                    printf("Error description is : %s\n", strerror(errno));
                    exit(EXIT_FAILURE);
                }

                // determing which client this socket is connnected
                for (int i_client = 0; i_client < (this->client_information)->GetClientNumber(); ++i_client) {
                    /* code */
                    if (socket_client[i_client].client_id == current_client_id) {
                        /* code */
                        current_client_id = i_client;
                        break;
                    }
                }
                printf("SERVER: MSG2 client_id Recv!\n");
                socket_client[current_client_id].socket_fd = current_socket_fd;

                // enclave proc msg2 and construct msg3, store msg3 for each client
                // 0: success, otherwise, fails
                int ret = attestation_recvMSG2(this->eid, &(this->context[current_client_id]), this->status, &server_socket, socket_client[current_client_id].socket_fd, socket_client[current_client_id].client_id, &(pp_ra_msg3_full[current_client_id]));

                if (ret != 0) {
                    if (INT_MAX != *context) {
                        int ret_save = ret;
                        ret = enclave_ra_close(this->eid, &this->status, this->context[current_client_id]);
                        if (SGX_SUCCESS != ret || status) {
                            ret = -1;
                            fprintf(OUTPUT, "\nError, call enclave_ra_close fail [%s].",
                                __FUNCTION__);
                        }
                        else {
                            // enclave_ra_close was successful, let's restore the value that
                            // led us to this point in the code.
                            ret = ret_save;
                        }
                        fprintf(OUTPUT, "\nCall enclave_ra_close success.");
                    }
                    sgx_destroy_enclave(this->eid);
                    SAFE_FREE(pp_ra_msg1_full[current_client_id]);
                    SAFE_FREE(pp_ra_msg3_full[current_client_id]);
                }
                break;
            }

            case 2: {
                if (server_socket.Recv(current_socket_fd, (char*)&current_client_id, 4) != 4) {
                    printf("SERVER: Recv Error! Error code: %d\n", errno);
                    printf("Error description is : %s\n", strerror(errno));
                    exit(EXIT_FAILURE);
                }

                // determing which client this socket is connnected
                for (int i_client = 0; i_client < (this->client_information)->GetClientNumber(); ++i_client) {
                    /* code */
                    if (socket_client[i_client].client_id == current_client_id) {
                        /* code */
                        current_client_id = i_client;
                        break;
                    }
                }
                socket_client[current_client_id].socket_fd = current_socket_fd;
                int ret = attestation_sendMSG3(this->eid, &(this->context[current_client_id]), this->status, &server_socket, socket_client[current_client_id].socket_fd, socket_client[current_client_id].client_id, &(pp_ra_msg3_full[current_client_id]));

                if (ret != 0) {
                                    if (INT_MAX != *context) {
                                        int ret_save = ret;
                                        ret = enclave_ra_close(this->eid, &this->status, this->context[current_client_id]);
                                        if (SGX_SUCCESS != ret || status) {
                                            ret = -1;
                                            fprintf(OUTPUT, "\nError, call enclave_ra_close fail [%s].",
                                                __FUNCTION__);
                                        }
                                        else {
                                            // enclave_ra_close was successful, let's restore the value that
                                            // led us to this point in the code.
                                            ret = ret_save;
                                        }
                                        fprintf(OUTPUT, "\nCall enclave_ra_close success.");
                                    }
                                    sgx_destroy_enclave(this->eid);
                                    SAFE_FREE(pp_ra_msg1_full[current_client_id]);
                                    SAFE_FREE(pp_ra_msg3_full[current_client_id]);
                                }
                                break;

            }
            case 3: {
                if (server_socket.Recv(current_socket_fd, (char*)&current_client_id, 4) != 4) {
                    printf("SERVER: Recv Error! Error code: %d\n", errno);
                    printf("Error description is : %s\n", strerror(errno));
                    exit(EXIT_FAILURE);
                }

                // determing which client this socket is connnected
                for (int i_client = 0; i_client < (this->client_information)->GetClientNumber(); ++i_client) {
                    /* code */
                    if (socket_client[i_client].client_id == current_client_id) {
                        /* code */
                        current_client_id = i_client;
                        break;
                    }
                }
                printf("SERVER: MSG4 client_id Recv!\n");
                socket_client[current_client_id].socket_fd = current_socket_fd;
                printf("recv msg4, from client id is %d and current_client_id is %d", socket_client[current_client_id].client_id, current_client_id);

                int ret = attestation_recvMSG4(this->eid, &(this->context[current_client_id]), this->status, &server_socket, socket_client[current_client_id].socket_fd, socket_client[current_client_id].client_id, &(pp_att_result_msg_full[current_client_id]), current_client_id);
                if(ret != 0){
                	  if (INT_MAX != *context) {
                	                                        int ret_save = ret;
                	                                        ret = enclave_ra_close(this->eid, &this->status, this->context[current_client_id]);
                	                                        if (SGX_SUCCESS != ret || status) {
                	                                            ret = -1;
                	                                            fprintf(OUTPUT, "\nError, call enclave_ra_close fail [%s].",
                	                                                __FUNCTION__);
                	                                        }
                	                                        else {
                	                                            // enclave_ra_close was successful, let's restore the value that
                	                                            // led us to this point in the code.
                	                                            ret = ret_save;
                	                                        }
                	                                        fprintf(OUTPUT, "\nCall enclave_ra_close success.");
                	                                    }
                	                                    sgx_destroy_enclave(this->eid);
                	                                    SAFE_FREE(pp_ra_msg1_full[current_client_id]);
                	                                    SAFE_FREE(pp_ra_msg3_full[current_client_id]);
                	                                    SAFE_FREE(pp_att_result_msg_full[current_client_id]);


                }
                break;
            }

            default: {
                printf("unknown message type!\n");
                break;
            }
            }

        } // end if_else for recv msg_type

    } // end loop for 4*client_number accept
}

long long Server::TimeEnd()
{
    gettimeofday(&this->end, NULL);
    long long time_length = (this->end.tv_sec - this->start.tv_sec) * 1000 + (this->end.tv_usec - this->end.tv_usec) / 1000;
    return time_length;
}

const string Server::Certificate_File = "sgx/enclave_DeezEnclave/enclave_server.cert.pem";
const string Server::Private_Key_File = "sgx/enclave_DeezEnclave/enclave_server.key.pem";
