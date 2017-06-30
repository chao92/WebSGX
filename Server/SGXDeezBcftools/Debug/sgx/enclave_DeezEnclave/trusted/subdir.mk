################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../sgx/enclave_DeezEnclave/trusted/DeezEnclave_t.c 

CPP_SRCS += \
../sgx/enclave_DeezEnclave/trusted/DataSealing.cpp \
../sgx/enclave_DeezEnclave/trusted/Decompress.cpp \
../sgx/enclave_DeezEnclave/trusted/DeezEnclave.cpp \
../sgx/enclave_DeezEnclave/trusted/EnclaveCommon.cpp \
../sgx/enclave_DeezEnclave/trusted/MemoryIO.cpp \
../sgx/enclave_DeezEnclave/trusted/Stats.cpp 

O_SRCS += \
../sgx/enclave_DeezEnclave/trusted/DataSealing.o \
../sgx/enclave_DeezEnclave/trusted/Decompress.o \
../sgx/enclave_DeezEnclave/trusted/DeezEnclave.o \
../sgx/enclave_DeezEnclave/trusted/DeezEnclave_t.o \
../sgx/enclave_DeezEnclave/trusted/EnclaveCommon.o \
../sgx/enclave_DeezEnclave/trusted/MemoryIO.o \
../sgx/enclave_DeezEnclave/trusted/Stats.o 

OBJS += \
./sgx/enclave_DeezEnclave/trusted/DataSealing.o \
./sgx/enclave_DeezEnclave/trusted/Decompress.o \
./sgx/enclave_DeezEnclave/trusted/DeezEnclave.o \
./sgx/enclave_DeezEnclave/trusted/DeezEnclave_t.o \
./sgx/enclave_DeezEnclave/trusted/EnclaveCommon.o \
./sgx/enclave_DeezEnclave/trusted/MemoryIO.o \
./sgx/enclave_DeezEnclave/trusted/Stats.o 

C_DEPS += \
./sgx/enclave_DeezEnclave/trusted/DeezEnclave_t.d 

CPP_DEPS += \
./sgx/enclave_DeezEnclave/trusted/DataSealing.d \
./sgx/enclave_DeezEnclave/trusted/Decompress.d \
./sgx/enclave_DeezEnclave/trusted/DeezEnclave.d \
./sgx/enclave_DeezEnclave/trusted/EnclaveCommon.d \
./sgx/enclave_DeezEnclave/trusted/MemoryIO.d \
./sgx/enclave_DeezEnclave/trusted/Stats.d 


# Each subdirectory must supply rules for building sources it contributes
sgx/enclave_DeezEnclave/trusted/%.o: ../sgx/enclave_DeezEnclave/trusted/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I/opt/intel/sgxsdk/include/ -I"/home/cjiang/workspace/SGXDeez/sgx/enclave_DeezEnclave/untrusted" -I"/home/cjiang/workspace/SGXDeez/sgx/enclave_DeezEnclave/untrusted/Header" -I"/home/cjiang/workspace/SGXDeez/sgx/enclave_DeezEnclave/untrusted/openssl-0.9.8k/include/openssl" -I"/home/cjiang/workspace/SGXDeez/sgx/enclave_DeezEnclave/untrusted/openssl-0.9.8k/include" -I/opt/intel/sgxsdk/include/tlibc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

sgx/enclave_DeezEnclave/trusted/%.o: ../sgx/enclave_DeezEnclave/trusted/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I/opt/intel/sgxsdk/include/ -I"/home/cjiang/workspace/SGXDeez/sgx/enclave_DeezEnclave/untrusted" -I"/home/cjiang/workspace/SGXDeez/sgx/enclave_DeezEnclave/untrusted/Header" -I"/home/cjiang/workspace/SGXDeez/sgx/enclave_DeezEnclave/untrusted/openssl-0.9.8k/include/openssl" -I"/home/cjiang/workspace/SGXDeez/sgx/enclave_DeezEnclave/untrusted/openssl-0.9.8k/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


