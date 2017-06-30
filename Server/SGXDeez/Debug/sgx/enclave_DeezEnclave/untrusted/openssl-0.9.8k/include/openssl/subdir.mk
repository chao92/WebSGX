################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../sgx/enclave_DeezEnclave/untrusted/openssl-0.9.8k/include/openssl/applink.c 

OBJS += \
./sgx/enclave_DeezEnclave/untrusted/openssl-0.9.8k/include/openssl/applink.o 

C_DEPS += \
./sgx/enclave_DeezEnclave/untrusted/openssl-0.9.8k/include/openssl/applink.d 


# Each subdirectory must supply rules for building sources it contributes
sgx/enclave_DeezEnclave/untrusted/openssl-0.9.8k/include/openssl/%.o: ../sgx/enclave_DeezEnclave/untrusted/openssl-0.9.8k/include/openssl/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I/opt/intel/sgxsdk/include/ -I"/home/cjiang/workspace/SGXDeez/sgx/enclave_DeezEnclave/untrusted" -I"/home/cjiang/workspace/SGXDeez/sgx/enclave_DeezEnclave/untrusted/Header" -I"/home/cjiang/workspace/SGXDeez/sgx/enclave_DeezEnclave/untrusted/openssl-0.9.8k/include/openssl" -I"/home/cjiang/workspace/SGXDeez/sgx/enclave_DeezEnclave/untrusted/openssl-0.9.8k/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


