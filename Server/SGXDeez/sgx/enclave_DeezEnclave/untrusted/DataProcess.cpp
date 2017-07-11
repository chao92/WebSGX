#include "Header/DataProcess.h"
//#include "sgx_key_exchange.h"
#include "DeezEnclave_u.h"

MESSAGE_LENGTH_TYPE getFileSize(sgx_ra_context_t context, char* fileBuffer,
								int numberofParties, int client_i, sgx_enclave_id_t eid)
{
#ifdef DEBUG_WWJ
	cout<<"The encryptedSize is :"<<endl;
	for (int i = 0; i < 24; i++)
	{
		putchar(fileBuffer[i]);
	}
	cout<<endl;
#endif

	MESSAGE_LENGTH_TYPE decryptedFileSize = 0;
	char decryptedFileSizeArray[sizeof(MESSAGE_LENGTH_TYPE)];

	useEnclaveGetFileSize(eid, context, fileBuffer, sizeof(MESSAGE_LENGTH_TYPE)+ENCRYPTED_ADD_SIZE, 
		decryptedFileSizeArray, sizeof(MESSAGE_LENGTH_TYPE), numberofParties, client_i);

	memmove(&decryptedFileSize, decryptedFileSizeArray, sizeof(MESSAGE_LENGTH_TYPE));

	return decryptedFileSize;
}

MESSAGE_LENGTH_TYPE getEncryptedFileSize(MESSAGE_LENGTH_TYPE fileSize)
{
	MESSAGE_LENGTH_TYPE blocks = fileSize / SEALING_BUFFER_SIZE;
	MESSAGE_LENGTH_TYPE residual = fileSize % SEALING_BUFFER_SIZE;
	MESSAGE_LENGTH_TYPE encryptedFileSize = blocks * (SEALING_BUFFER_SIZE + ENCRYPTED_ADD_SIZE) + (residual + ENCRYPTED_ADD_SIZE);
	if (residual == 0)
	{
		encryptedFileSize = encryptedFileSize - ENCRYPTED_ADD_SIZE;
	}
	return encryptedFileSize;
}

void sealDZFile(sgx_ra_context_t context, char* data, MESSAGE_LENGTH_TYPE inputFileSize, string sealed_dz, int client_num, int i, char* sealedDataMAC, sgx_enclave_id_t eid)
{
	MESSAGE_LENGTH_TYPE pos = 0;
	MESSAGE_LENGTH_TYPE size_after_seal = 0;
	int temp_size;
	__int64 index = 0;
	char* encryptedData = new char[SEALING_BUFFER_SIZE+ENCRYPTED_ADD_SIZE];
	char* sealed_data = new char[SEALED_BUFFER_SIZE];

	//MESSAGE_LENGTH_TYPE inputFileSize = getFileSize(context, data, client_num, i, eid);
	MESSAGE_LENGTH_TYPE encryptedFileSize = getEncryptedFileSize(inputFileSize);
	data = data + sizeof(MESSAGE_LENGTH_TYPE) + ENCRYPTED_ADD_SIZE;

	printf("slicing and sealing dz file...\n");
	File* outFile = File::Open(sealed_dz.c_str(),"wb");
	if(outFile ==NULL)
	{
		std::cout << "Cannot open the file %s" << sealed_dz;
	}

	while (pos != encryptedFileSize) 
	{
		temp_size = (SEALING_BUFFER_SIZE+ENCRYPTED_ADD_SIZE) < (encryptedFileSize-pos)? 
			(SEALING_BUFFER_SIZE+ENCRYPTED_ADD_SIZE):(encryptedFileSize - pos);
		memmove(encryptedData, data, temp_size);

		ecall_decryped_seal_buffer(eid, context, encryptedData, SEALING_BUFFER_SIZE+ENCRYPTED_ADD_SIZE,
			sealed_data, SEALED_BUFFER_SIZE, temp_size, index, sealedDataMAC, SECRET_MAC_SIZE);
		outFile->write(sealed_data, SEALED_BUFFER_SIZE);
		data = data + temp_size;
		pos += temp_size;
		index++;
	}

	outFile->close();
	delete[] encryptedData;
	delete[] sealed_data;
}

