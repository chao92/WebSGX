################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../sgx/enclave_DeezEnclave/trusted/Streams/ArithmeticCoder.cpp \
../sgx/enclave_DeezEnclave/trusted/Streams/Stream.cpp 

O_SRCS += \
../sgx/enclave_DeezEnclave/trusted/Streams/ArithmeticCoder.o \
../sgx/enclave_DeezEnclave/trusted/Streams/Stream.o 

OBJS += \
./sgx/enclave_DeezEnclave/trusted/Streams/ArithmeticCoder.o \
./sgx/enclave_DeezEnclave/trusted/Streams/Stream.o 

CPP_DEPS += \
./sgx/enclave_DeezEnclave/trusted/Streams/ArithmeticCoder.d \
./sgx/enclave_DeezEnclave/trusted/Streams/Stream.d 


# Each subdirectory must supply rules for building sources it contributes
sgx/enclave_DeezEnclave/trusted/Streams/%.o: ../sgx/enclave_DeezEnclave/trusted/Streams/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I/opt/intel/sgxsdk/include/ -I"/home/cjiang/workspace/SGXDeez/sgx/enclave_DeezEnclave/untrusted" -I"/home/cjiang/workspace/SGXDeez/sgx/enclave_DeezEnclave/untrusted/Header" -I"/home/cjiang/workspace/SGXDeez/sgx/enclave_DeezEnclave/untrusted/openssl-0.9.8k/include/openssl" -I"/home/cjiang/workspace/SGXDeez/sgx/enclave_DeezEnclave/untrusted/openssl-0.9.8k/include" -I/opt/intel/sgxsdk/include/tlibc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


