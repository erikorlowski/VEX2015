################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Drive.c \
../src/DriveForTime.c \
../src/DriveTestMotor.c \
../src/OI.c \
../src/PantherMotor.c \
../src/TestMotor.c \
../src/Utilities.c \
../src/auto.c \
../src/init.c \
../src/opcontrol.c 

OBJS += \
./src/Drive.o \
./src/DriveForTime.o \
./src/DriveTestMotor.o \
./src/OI.o \
./src/PantherMotor.o \
./src/TestMotor.o \
./src/Utilities.o \
./src/auto.o \
./src/init.o \
./src/opcontrol.o 

C_DEPS += \
./src/Drive.d \
./src/DriveForTime.d \
./src/DriveTestMotor.d \
./src/OI.d \
./src/PantherMotor.d \
./src/TestMotor.d \
./src/Utilities.d \
./src/auto.d \
./src/init.d \
./src/opcontrol.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


