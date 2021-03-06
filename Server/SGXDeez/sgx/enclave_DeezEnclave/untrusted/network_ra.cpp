/**
*   Copyright(C) 2011-2015 Intel Corporation All Rights Reserved.
*
*   The source code, information  and  material ("Material") contained herein is
*   owned  by Intel Corporation or its suppliers or licensors, and title to such
*   Material remains  with Intel Corporation  or its suppliers or licensors. The
*   Material  contains proprietary information  of  Intel or  its  suppliers and
*   licensors. The  Material is protected by worldwide copyright laws and treaty
*   provisions. No  part  of  the  Material  may  be  used,  copied, reproduced,
*   modified, published, uploaded, posted, transmitted, distributed or disclosed
*   in any way  without Intel's  prior  express written  permission. No  license
*   under  any patent, copyright  or  other intellectual property rights  in the
*   Material  is  granted  to  or  conferred  upon  you,  either  expressly,  by
*   implication, inducement,  estoppel or  otherwise.  Any  license  under  such
*   intellectual  property  rights must  be express  and  approved  by  Intel in
*   writing.
*
*   *Third Party trademarks are the property of their respective owners.
*
*   Unless otherwise  agreed  by Intel  in writing, you may not remove  or alter
*   this  notice or  any other notice embedded  in Materials by Intel or Intel's
*   suppliers or licensors in any way.
*/


#include <stdint.h>
#ifdef _MSC_VER
#endif
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "Header/network_ra.h"
#include "Header/Socket.h"
#include "Header/service_provider.h"

// Used to send requests to the service provider sample.  It
// simulates network communication between the ISV app and the
// ISV service provider.  This would be modified in a real
// product to use the proper IP communication.
//
// @param server_url String name of the server URL
// @param p_req Pointer to the message to be sent.
// @param p_resp Pointer to a pointer of the response message.

// @return int

#define ATTESTATION_DEBUG

int ra_network_send_receive(Socket *S, int socket_fd,
    const ra_samp_request_header_t *p_req,
    ra_samp_response_header_t **p_resp)
{
	int ret = 0;
    ra_samp_response_header_t* p_resp_msg;
    int size;

    switch(p_req->type)
    {

        case TYPE_RA_MSG1:
        /*ret = sp_ra_proc_msg1_req((const sample_ra_msg1_t*)((uint8_t*)p_req
            + sizeof(ra_samp_request_header_t)),
            p_req->size,
            &p_resp_msg);*/

		//send msg1
//        	int size;
        	size = sizeof(sample_ra_msg1_t);

#if defined ATTESTATION_DEBUG
		printf( "sent size:%d\n", size);
#endif
		S->Send(socket_fd, (char *)(&size), sizeof(int));
		S->Send(socket_fd, (char *)(p_req->body), size);

#if defined ATTESTATION_DEBUG
		printf("msg1 sent! \n");
#endif		
//		 reconnect socket in order to adapt to web client
//		S->Close();
//
//		*S = Socket(0,7891,0);
//		printf("SERVER: restart to Listen!\n");
//		if(!S->Connect(0, 7891, 0))
//		{
//			printf("SERVER: Fail to Listen!\n");
//			exit(EXIT_FAILURE);
//		}
//		else{
//			printf("SERVER: Start to Listen!\n");
//		}
//		socket_fd = S->Accept();


//		end of reconnect socket

		// new code for web client
		socket_fd = S->Accept();
		// end of new code for we client

		//receive msg2
		char *msg2;
		int length;
		length = 0;
		int pos;
		pos = 0;
		int recvLength;
		while(true)
		{
			if (!length)
			{
				if(S->Recv(socket_fd,(char*)&length,4)!=4) 
				{
					printf("CLIENT: Recv Error! Error code: %d\n", errno);
					printf("Error description is : %s\n", strerror(errno));
					return 0;
				}
				msg2 = new char[length];
				printf("msg2 length is : %d\n", length);
			}
			else
			{
				while (pos < length)
				{
					recvLength = S->Recv(socket_fd, msg2+pos,length-pos);
					printf("recvLength is%d\n",recvLength);
					if (recvLength < 0)
					{
						printf("CLIENT: Recv Error! Error code: %d\n", errno);
						printf("Error description is : %s\n", strerror(errno));
						return 0;
					}
					pos += recvLength;
				}
				break;
			}
		}
		
		if (length >= 0)
		{
			*p_resp = (ra_samp_response_header_t*) msg2;
		}
#if defined ATTESTATION_DEBUG
		printf( "msg2 received!\n");
#endif
        if(0 != ret)
        {
            fprintf(stderr, "\nError, call sp_ra_proc_msg1_req fail [%s].",
                __FUNCTION__);
        }
      
        break;

        
		case TYPE_RA_MSG3:
        
		//send msg3
			// new code for web client
		socket_fd = S->Accept();
		// end of new code for web client

		printf("msg3 accept to client\n");
		size = 1452;

#if defined ATTESTATION_DEBUG
		printf("sent size: %d\n", size);
#endif

		S->Send(socket_fd, (char *)(&size), sizeof(int));
		S->Send(socket_fd, (char *)(p_req->body), size);
#if defined ATTESTATION_DEBUG
		printf("msg3 sent! \n");
#endif

		
        break;

        default:
        ret = -1;
        fprintf(stderr, "\nError, unknown ra message type. Type = %d [%s].",
            p_req->type, __FUNCTION__);
        break;
    }

    return ret;
}

// Used to free the response messages.  In the sample code, the
// response messages are allocated by the SP code.
//
//
// @param resp Pointer to the response buffer to be freed.

void ra_free_network_response_buffer(ra_samp_response_header_t *resp)
{
    if(resp!=NULL)
    {
        free(resp);
    }
}

int ra_network_send_MSG1(Socket *S, int socket_fd, const ra_samp_request_header_t *p_req)
{
	int ret = 0;

	int size;
	size = sizeof(sample_ra_msg1_t);

	printf( "sent size:%d\n", size);

	S->Send(socket_fd, (char *)(&size), sizeof(int));
	S->Send(socket_fd, (char *)(p_req->body), size);

	printf("msg1 sent! \n");
	return ret;
}

int ra_network_recv_MSG2(Socket *S, int socket_fd, ra_samp_response_header_t **p_resp)
{
	int ret = 0;

	char *msg2;
	int length = 0;
	int pos = 0;
	int recvLength;
	while(true)
	{
		if (!length)
		{
			if(S->Recv(socket_fd,(char*)&length,4)!=4)
			{
				printf("CLIENT: Recv Error! Error code: %d\n", errno);
				printf("Error description is : %s\n", strerror(errno));
				return 0;
			}
			msg2 = new char[length];
			printf("msg2 length is : %d\n", length);
		}
		else
		{
			while (pos < length)
			{
				recvLength = S->Recv(socket_fd, msg2+pos,length-pos);
				printf("recvLength is%d\n",recvLength);
				if (recvLength < 0)
				{
					printf("CLIENT: Recv Error! Error code: %d\n", errno);
					printf("Error description is : %s\n", strerror(errno));
					return 0;
				}
				pos += recvLength;
			}
			break;
		}
	}
	if (length >= 0)
	{
		*p_resp = (ra_samp_response_header_t*) msg2;
	}
	printf( "msg2 received!\n");
	return ret;
}



int ra_network_send_MSG3(Socket *S, int socket_fd, const ra_samp_request_header_t *p_req)
{
	int ret = 0;

	int size;
	size  = 1452;;

	printf( "sent size:%d\n", size);

	S->Send(socket_fd, (char *)(&size), sizeof(int));
	S->Send(socket_fd, (char *)(p_req->body), size);

	printf("msg3 sent! \n");
	return ret;
}
int ra_network_recv_MSG4(Socket *S, int socket_fd, ra_samp_response_header_t **p_resp)
{
	int ret = 0;

	char *msg4;
	int length = 0;
	int pos = 0;
	int recvLength;

	char msg[] = "data";
	S->Send(socket_fd, msg, strlen(msg)+1);


	while(true)
	{
		if (!length)
		{
			if(S->Recv(socket_fd,(char*)&length,4)!=4)
			{
				printf("CLIENT: Recv Error! Error code: %d\n", errno);
				printf("Error description is : %s\n", strerror(errno));
				return 0;
			}
			msg4 = new char[length];
			printf("msg4 length is : %d\n", length);
		}
		else
		{
			while (pos < length)
			{
				recvLength = S->Recv(socket_fd, msg4+pos,length-pos);
				printf("recvLength is%d\n",recvLength);
				if (recvLength < 0)
				{
					printf("CLIENT: Recv Error! Error code: %d\n", errno);
					printf("Error description is : %s\n", strerror(errno));
					return 0;
				}
				pos += recvLength;
			}
			break;
		}
	}
	if (length >= 0)
	{
		*p_resp = (ra_samp_response_header_t*) msg4;
	}
	printf( "msg4 received!\n");
	return ret;
}

int ra_network_recv_SAM(Socket *S, int socket_fd, uint8_t **p_resp, int* data_size)
{
	int ret = 0;

	uint8_t *samfile;
	int length = 0;
	int pos = 0;
	int recvLength;

	char msg[] = "data";
	S->Send(socket_fd, msg, strlen(msg)+1);


	while(true)
	{
		if (!length)
		{
			if(S->Recv(socket_fd,(char*)&length,4)!=4)
			{
				printf("CLIENT: Recv Error! Error code: %d\n", errno);
				printf("Error description is : %s\n", strerror(errno));
				return 0;
			}
			*data_size = length;
			samfile = new uint8_t[length];
			printf("sam file length is : %d\n", length);
		}
		else
		{
			while (pos < length)
			{
				recvLength = S->Recv(socket_fd, samfile+pos,length-pos);
				printf("recvLength is%d\n",recvLength);
				if (recvLength < 0)
				{
					printf("CLIENT: Recv Error! Error code: %d\n", errno);
					printf("Error description is : %s\n", strerror(errno));
					return 0;
				}
				pos += recvLength;
			}
			break;
		}
	}
	if (length >= 0)
	{
		*p_resp = (uint8_t*) samfile;
	}
	printf( "sam file received!\n");
	return ret;
}

int ra_network_recv_REF(Socket *S, int socket_fd, uint8_t **p_resp, int* data_size)
{
	int ret = 0;

	uint8_t *reffile;
	int length = 0;
	int pos = 0;
	int recvLength;

	char msg[] = "data";
	S->Send(socket_fd, msg, strlen(msg)+1);


	while(true)
	{
		if (!length)
		{
			if(S->Recv(socket_fd,(char*)&length,4)!=4)
			{
				printf("CLIENT: Recv Error! Error code: %d\n", errno);
				printf("Error description is : %s\n", strerror(errno));
				return 0;
			}
			*data_size = length;
			reffile = new uint8_t[length];
			printf("ref file length is : %d\n", length);
		}
		else
		{
			while (pos < length)
			{
				recvLength = S->Recv(socket_fd, reffile+pos,length-pos);
				printf("recvLength is%d\n",recvLength);
				if (recvLength < 0)
				{
					printf("CLIENT: Recv Error! Error code: %d\n", errno);
					printf("Error description is : %s\n", strerror(errno));
					return 0;
				}
				pos += recvLength;
			}
			break;
		}
	}
	if (length >= 0)
	{
		*p_resp = (uint8_t*) reffile;
	}
	printf( "ref file received!\n");
	return ret;
}

int ra_network_recv_INDEX(Socket *S, int socket_fd, uint8_t **p_resp, int* data_size)
{
	int ret = 0;

	uint8_t *indexfile;
	int length = 0;
	int pos = 0;
	int recvLength;

	char msg[] = "data";
	S->Send(socket_fd, msg, strlen(msg)+1);


	while(true)
	{
		if (!length)
		{
			if(S->Recv(socket_fd,(char*)&length,4)!=4)
			{
				printf("CLIENT: Recv Error! Error code: %d\n", errno);
				printf("Error description is : %s\n", strerror(errno));
				return 0;
			}
			*data_size = length;
			indexfile = new uint8_t[length];
			printf("------index file length is : %d\n", length);
		}
		else
		{
			while (pos < length)
			{
				printf("pos < length and pos is %d, length is %d\n",pos,length);
				recvLength = S->Recv(socket_fd, indexfile+pos,length-pos);
				printf("recvLength is%d\n",recvLength);
				if (recvLength < 0)
				{
					printf("CLIENT: Recv Error! Error code: %d\n", errno);
					printf("Error description is : %s\n", strerror(errno));
					return 0;
				}
				pos += recvLength;
			}
			break;
		}
	}
	if (length >= 0)
	{
		*p_resp = (uint8_t*) indexfile;
	}
	printf( "index file received!\n");
	return ret;
}