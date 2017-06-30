################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../sgx/enclave_DeezEnclave/trusted/Fields/EditOperationDecompressor.cpp \
../sgx/enclave_DeezEnclave/trusted/Fields/PairedEndDecompressor.cpp \
../sgx/enclave_DeezEnclave/trusted/Fields/QualityScoreDecompressor.cpp \
../sgx/enclave_DeezEnclave/trusted/Fields/ReadNameDecompressor.cpp \
../sgx/enclave_DeezEnclave/trusted/Fields/SAMComment.cpp \
../sgx/enclave_DeezEnclave/trusted/Fields/SequenceDecompressor.cpp 

O_SRCS += \
../sgx/enclave_DeezEnclave/trusted/Fields/EditOperationDecompressor.o \
../sgx/enclave_DeezEnclave/trusted/Fields/PairedEndDecompressor.o \
../sgx/enclave_DeezEnclave/trusted/Fields/QualityScoreDecompressor.o \
../sgx/enclave_DeezEnclave/trusted/Fields/ReadNameDecompressor.o \
../sgx/enclave_DeezEnclave/trusted/Fields/SAMComment.o \
../sgx/enclave_DeezEnclave/trusted/Fields/SequenceDecompressor.o 

OBJS += \
./sgx/enclave_DeezEnclave/trusted/Fields/EditOperationDecompressor.o \
./sgx/enclave_DeezEnclave/trusted/Fields/PairedEndDecompressor.o \
./sgx/enclave_DeezEnclave/trusted/Fields/QualityScoreDecompressor.o \
./sgx/enclave_DeezEnclave/trusted/Fields/ReadNameDecompressor.o \
./sgx/enclave_DeezEnclave/trusted/Fields/SAMComment.o \
./sgx/enclave_DeezEnclave/trusted/Fields/SequenceDecompressor.o 

CPP_DEPS += \
./sgx/enclave_DeezEnclave/trusted/Fields/EditOperationDecompressor.d \
./sgx/enclave_DeezEnclave/trusted/Fields/PairedEndDecompressor.d \
./sgx/enclave_DeezEnclave/trusted/Fields/QualityScoreDecompressor.d \
./sgx/enclave_DeezEnclave/trusted/Fields/ReadNameDecompressor.d \
./sgx/enclave_DeezEnclave/trusted/Fields/SAMComment.d \
./sgx/enclave_DeezEnclave/trusted/Fields/SequenceDecompressor.d 


# Each subdirectory must supply rules for building sources it contributes
sgx/enclave_DeezEnclave/trusted/Fields/%.o: ../sgx/enclave_DeezEnclave/trusted/Fields/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I/opt/intel/sgxsdk/include/ -I"/home/cjiang/workspace/SGXDeez/sgx/enclave_DeezEnclave/untrusted" -I"/home/cjiang/workspace/SGXDeez/sgx/enclave_DeezEnclave/untrusted/Header" -I"/home/cjiang/workspace/SGXDeez/sgx/enclave_DeezEnclave/untrusted/openssl-0.9.8k/include/openssl" -I"/home/cjiang/workspace/SGXDeez/sgx/enclave_DeezEnclave/untrusted/openssl-0.9.8k/include" -I/opt/intel/sgxsdk/include/tlibc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


