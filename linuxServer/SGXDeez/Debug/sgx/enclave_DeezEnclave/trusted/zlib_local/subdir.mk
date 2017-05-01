################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../sgx/enclave_DeezEnclave/trusted/zlib_local/adler32_local.cpp \
../sgx/enclave_DeezEnclave/trusted/zlib_local/crc32_local.cpp \
../sgx/enclave_DeezEnclave/trusted/zlib_local/inffast_local.cpp \
../sgx/enclave_DeezEnclave/trusted/zlib_local/inflate_local.cpp \
../sgx/enclave_DeezEnclave/trusted/zlib_local/inftrees_local.cpp \
../sgx/enclave_DeezEnclave/trusted/zlib_local/uncompr_local.cpp \
../sgx/enclave_DeezEnclave/trusted/zlib_local/zutil_local.cpp 

O_SRCS += \
../sgx/enclave_DeezEnclave/trusted/zlib_local/adler32_local.o \
../sgx/enclave_DeezEnclave/trusted/zlib_local/crc32_local.o \
../sgx/enclave_DeezEnclave/trusted/zlib_local/inffast_local.o \
../sgx/enclave_DeezEnclave/trusted/zlib_local/inflate_local.o \
../sgx/enclave_DeezEnclave/trusted/zlib_local/inftrees_local.o \
../sgx/enclave_DeezEnclave/trusted/zlib_local/uncompr_local.o \
../sgx/enclave_DeezEnclave/trusted/zlib_local/zutil_local.o 

OBJS += \
./sgx/enclave_DeezEnclave/trusted/zlib_local/adler32_local.o \
./sgx/enclave_DeezEnclave/trusted/zlib_local/crc32_local.o \
./sgx/enclave_DeezEnclave/trusted/zlib_local/inffast_local.o \
./sgx/enclave_DeezEnclave/trusted/zlib_local/inflate_local.o \
./sgx/enclave_DeezEnclave/trusted/zlib_local/inftrees_local.o \
./sgx/enclave_DeezEnclave/trusted/zlib_local/uncompr_local.o \
./sgx/enclave_DeezEnclave/trusted/zlib_local/zutil_local.o 

CPP_DEPS += \
./sgx/enclave_DeezEnclave/trusted/zlib_local/adler32_local.d \
./sgx/enclave_DeezEnclave/trusted/zlib_local/crc32_local.d \
./sgx/enclave_DeezEnclave/trusted/zlib_local/inffast_local.d \
./sgx/enclave_DeezEnclave/trusted/zlib_local/inflate_local.d \
./sgx/enclave_DeezEnclave/trusted/zlib_local/inftrees_local.d \
./sgx/enclave_DeezEnclave/trusted/zlib_local/uncompr_local.d \
./sgx/enclave_DeezEnclave/trusted/zlib_local/zutil_local.d 


# Each subdirectory must supply rules for building sources it contributes
sgx/enclave_DeezEnclave/trusted/zlib_local/%.o: ../sgx/enclave_DeezEnclave/trusted/zlib_local/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I/opt/intel/sgxsdk/include/ -I"/home/cjiang/workspace/SGXDeez/sgx/enclave_DeezEnclave/untrusted" -I"/home/cjiang/workspace/SGXDeez/sgx/enclave_DeezEnclave/untrusted/Header" -I"/home/cjiang/workspace/SGXDeez/sgx/enclave_DeezEnclave/untrusted/openssl-0.9.8k/include/openssl" -I"/home/cjiang/workspace/SGXDeez/sgx/enclave_DeezEnclave/untrusted/openssl-0.9.8k/include" -I/opt/intel/sgxsdk/include/tlibc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


