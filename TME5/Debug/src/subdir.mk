################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Color.cpp \
../src/Vec3D.cpp \
../src/main.cpp 

O_SRCS += \
../src/Color.o \
../src/Vec3D.o \
../src/main.o 

OBJS += \
./src/Color.o \
./src/Vec3D.o \
./src/main.o 

CPP_DEPS += \
./src/Color.d \
./src/Vec3D.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


