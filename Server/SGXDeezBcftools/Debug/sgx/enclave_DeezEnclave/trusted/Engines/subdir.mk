################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../sgx/enclave_DeezEnclave/trusted/Engines/Engine.cpp 

O_SRCS += \
../sgx/enclave_DeezEnclave/trusted/Engines/Engine.o 

OBJS += \
./sgx/enclave_DeezEnclave/trusted/Engines/Engine.o 

CPP_DEPS += \
./sgx/enclave_DeezEnclave/trusted/Engines/Engine.d 


# Each subdirectory must supply rules for building sources it contributes
sgx/enclave_DeezEnclave/trusted/Engines/%.o: ../sgx/enclave_DeezEnclave/trusted/Engines/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I/opt/intel/sgxsdk/include/ -I"/home/cjiang/workspace/SGXDeez/sgx/enclave_DeezEnclave/untrusted" -I"/home/cjiang/workspace/SGXDeez/sgx/enclave_DeezEnclave/untrusted/Header" -I"/home/cjiang/workspace/SGXDeez/sgx/enclave_DeezEnclave/untrusted/openssl-0.9.8k/include/openssl" -I"/home/cjiang/workspace/SGXDeez/sgx/enclave_DeezEnclave/untrusted/openssl-0.9.8k/include" -I/opt/intel/sgxsdk/include/tlibc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


