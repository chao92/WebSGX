#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <pwd.h>
#include <libgen.h>
#include <stdlib.h>
#include <iostream>
#include <sgx_urts.h>

#include "DeezEnclave_u.h"
#include "Header/enclavedefine.h"
#include "Header/log.h"
#include "Header/sample.h"
#include "Header/config.h"
#include "Server.h"

//using namespace std;

/* Global EID shared by multiple threads */
sgx_enclave_id_t global_eid = 0;


void CommandAnalsis(Config& client_configuration, int argc, char* argv[]);

/* Application entry */
int SGX_CDECL main(int argc, char *argv[])
{

    RecordResult();

    Config client_configuration;

    CommandAnalsis(client_configuration, argc, argv);

    Server sgx_server(&client_configuration);

    sgx_server.InitializeCommunication();



    return 0;

}

void CommandAnalsis(Config& client_configuration, int argc, char* argv[])
{
    for(int i = 1; i < argc; i++)
    {
    	if(argv[i][0] == '-')
    	{
    		if(argv[i][1] == 'c')
    		{
    			if(client_configuration.Parse(argv[i+1]))
    			{
    				printf("Read client information from file successfully.");
    				i++;
    			}
    			else
    			{
    				printf("Can't not read client information from file.");
    				exit(EXIT_SUCCESS);
    			}
    		}

    		else if(argv[i][1] == 'n')
    		{
    			client_configuration.Write(Config::KeyName[0], std::string(argv[i+1]));//"ClientNum"
    			i++;
    		}

    		else if (argv[i][1] == 'p')
			{
    			client_configuration.Write(Config::KeyName[2], std::string(argv[i+1]));//"Port"
				i++;
			}

    		else if (argv[i][1] == 'a')
			{
    			client_configuration.Write(Config::KeyName[8], std::string(argv[i+1]));//"Algorithm"
				i++;
			}

    		else if (argv[i][1] == 'h')
			{
				printf("usage:\n");
				printf("Distributed_Secure_GWAS -c <config file path>\n");
				printf("Distributed_Secure_GWAS -n <number of clients> -p <port> -a <algorithm>\n");
				printf("Distributed_Secure_GWAS -h\n");
				printf("Distributed_Secure_GWAS -v\n");
				exit(EXIT_SUCCESS);
			}
			else if (argv[i][1] == 'v')
			{
				printf("Version: v1.0\nRelease data: Feb 10th 2016\n");
				exit(EXIT_SUCCESS);
			}
			else
			{
				printf("Unknown option!\n");
				exit(EXIT_SUCCESS);
			}
		}
		else
		{
			printf("Unknown option!\n");
			exit(EXIT_SUCCESS);
		}
    }
}
