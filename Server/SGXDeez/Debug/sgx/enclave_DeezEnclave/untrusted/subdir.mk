################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../sgx/enclave_DeezEnclave/untrusted/DeezEnclave_u.c 

CPP_SRCS += \
../sgx/enclave_DeezEnclave/untrusted/DataProcess.cpp \
../sgx/enclave_DeezEnclave/untrusted/FileIO.cpp \
../sgx/enclave_DeezEnclave/untrusted/OcallFunction.cpp \
../sgx/enclave_DeezEnclave/untrusted/Reference.cpp \
../sgx/enclave_DeezEnclave/untrusted/Server.cpp \
../sgx/enclave_DeezEnclave/untrusted/Socket.cpp \
../sgx/enclave_DeezEnclave/untrusted/attestation.cpp \
../sgx/enclave_DeezEnclave/untrusted/common.cpp \
../sgx/enclave_DeezEnclave/untrusted/config.cpp \
../sgx/enclave_DeezEnclave/untrusted/enclavedefine.cpp \
../sgx/enclave_DeezEnclave/untrusted/log.cpp \
../sgx/enclave_DeezEnclave/untrusted/network_ra.cpp \
../sgx/enclave_DeezEnclave/untrusted/sample.cpp \
../sgx/enclave_DeezEnclave/untrusted/ssl_server.cpp 

O_SRCS += \
../sgx/enclave_DeezEnclave/untrusted/ClientInformation.o \
../sgx/enclave_DeezEnclave/untrusted/Config.o \
../sgx/enclave_DeezEnclave/untrusted/DataProcess.o \
../sgx/enclave_DeezEnclave/untrusted/DeezEnclave_u.o \
../sgx/enclave_DeezEnclave/untrusted/FileIO.o \
../sgx/enclave_DeezEnclave/untrusted/OcallFunction.o \
../sgx/enclave_DeezEnclave/untrusted/Reference.o \
../sgx/enclave_DeezEnclave/untrusted/Server.o \
../sgx/enclave_DeezEnclave/untrusted/Socket.o \
../sgx/enclave_DeezEnclave/untrusted/attestation.o \
../sgx/enclave_DeezEnclave/untrusted/common.o \
../sgx/enclave_DeezEnclave/untrusted/config.o \
../sgx/enclave_DeezEnclave/untrusted/enclavedefine.o \
../sgx/enclave_DeezEnclave/untrusted/log.o \
../sgx/enclave_DeezEnclave/untrusted/network_ra.o \
../sgx/enclave_DeezEnclave/untrusted/sample.o \
../sgx/enclave_DeezEnclave/untrusted/ssl_server.o 

OBJS += \
./sgx/enclave_DeezEnclave/untrusted/DataProcess.o \
./sgx/enclave_DeezEnclave/untrusted/DeezEnclave_u.o \
./sgx/enclave_DeezEnclave/untrusted/FileIO.o \
./sgx/enclave_DeezEnclave/untrusted/OcallFunction.o \
./sgx/enclave_DeezEnclave/untrusted/Reference.o \
./sgx/enclave_DeezEnclave/untrusted/Server.o \
./sgx/enclave_DeezEnclave/untrusted/Socket.o \
./sgx/enclave_DeezEnclave/untrusted/attestation.o \
./sgx/enclave_DeezEnclave/untrusted/common.o \
./sgx/enclave_DeezEnclave/untrusted/config.o \
./sgx/enclave_DeezEnclave/untrusted/enclavedefine.o \
./sgx/enclave_DeezEnclave/untrusted/log.o \
./sgx/enclave_DeezEnclave/untrusted/network_ra.o \
./sgx/enclave_DeezEnclave/untrusted/sample.o \
./sgx/enclave_DeezEnclave/untrusted/ssl_server.o 

C_DEPS += \
./sgx/enclave_DeezEnclave/untrusted/DeezEnclave_u.d 

CPP_DEPS += \
./sgx/enclave_DeezEnclave/untrusted/DataProcess.d \
./sgx/enclave_DeezEnclave/untrusted/FileIO.d \
./sgx/enclave_DeezEnclave/untrusted/OcallFunction.d \
./sgx/enclave_DeezEnclave/untrusted/Reference.d \
./sgx/enclave_DeezEnclave/untrusted/Server.d \
./sgx/enclave_DeezEnclave/untrusted/Socket.d \
./sgx/enclave_DeezEnclave/untrusted/attestation.d \
./sgx/enclave_DeezEnclave/untrusted/common.d \
./sgx/enclave_DeezEnclave/untrusted/config.d \
./sgx/enclave_DeezEnclave/untrusted/enclavedefine.d \
./sgx/enclave_DeezEnclave/untrusted/log.d \
./sgx/enclave_DeezEnclave/untrusted/network_ra.d \
./sgx/enclave_DeezEnclave/untrusted/sample.d \
./sgx/enclave_DeezEnclave/untrusted/ssl_server.d 


# Each subdirectory must supply rules for building sources it contributes
sgx/enclave_DeezEnclave/untrusted/%.o: ../sgx/enclave_DeezEnclave/untrusted/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I/opt/intel/sgxsdk/include/ -I"/home/cjiang/workspace/SGXDeez/sgx/enclave_DeezEnclave/untrusted" -I"/home/cjiang/workspace/SGXDeez/sgx/enclave_DeezEnclave/untrusted/Header" -I"/home/cjiang/workspace/SGXDeez/sgx/enclave_DeezEnclave/untrusted/openssl-0.9.8k/include/openssl" -I"/home/cjiang/workspace/SGXDeez/sgx/enclave_DeezEnclave/untrusted/openssl-0.9.8k/include" -I/opt/intel/sgxsdk/include/tlibc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

sgx/enclave_DeezEnclave/untrusted/%.o: ../sgx/enclave_DeezEnclave/untrusted/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I/opt/intel/sgxsdk/include/ -I"/home/cjiang/workspace/SGXDeez/sgx/enclave_DeezEnclave/untrusted" -I"/home/cjiang/workspace/SGXDeez/sgx/enclave_DeezEnclave/untrusted/Header" -I"/home/cjiang/workspace/SGXDeez/sgx/enclave_DeezEnclave/untrusted/openssl-0.9.8k/include/openssl" -I"/home/cjiang/workspace/SGXDeez/sgx/enclave_DeezEnclave/untrusted/openssl-0.9.8k/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


