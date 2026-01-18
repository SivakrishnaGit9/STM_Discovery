################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ThirdParty/RTOS/croutine.c \
../ThirdParty/RTOS/event_groups.c \
../ThirdParty/RTOS/list.c \
../ThirdParty/RTOS/queue.c \
../ThirdParty/RTOS/stream_buffer.c \
../ThirdParty/RTOS/tasks.c \
../ThirdParty/RTOS/timers.c 

OBJS += \
./ThirdParty/RTOS/croutine.o \
./ThirdParty/RTOS/event_groups.o \
./ThirdParty/RTOS/list.o \
./ThirdParty/RTOS/queue.o \
./ThirdParty/RTOS/stream_buffer.o \
./ThirdParty/RTOS/tasks.o \
./ThirdParty/RTOS/timers.o 

C_DEPS += \
./ThirdParty/RTOS/croutine.d \
./ThirdParty/RTOS/event_groups.d \
./ThirdParty/RTOS/list.d \
./ThirdParty/RTOS/queue.d \
./ThirdParty/RTOS/stream_buffer.d \
./ThirdParty/RTOS/tasks.d \
./ThirdParty/RTOS/timers.d 


# Each subdirectory must supply rules for building sources it contributes
ThirdParty/RTOS/%.o ThirdParty/RTOS/%.su ThirdParty/RTOS/%.cyclo: ../ThirdParty/RTOS/%.c ThirdParty/RTOS/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I"E:/embedded projects/EmbeddedSystem/STM_Discovery/STMicro_FreeRTOS/STMicro_FreeRTOS/ThirdParty/RTOS" -I"E:/embedded projects/EmbeddedSystem/STM_Discovery/STMicro_FreeRTOS/STMicro_FreeRTOS/ThirdParty/RTOS/include" -I"E:/embedded projects/EmbeddedSystem/STM_Discovery/STMicro_FreeRTOS/STMicro_FreeRTOS/ThirdParty/RTOS/portable/GCC/ARM_CM4F" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-ThirdParty-2f-RTOS

clean-ThirdParty-2f-RTOS:
	-$(RM) ./ThirdParty/RTOS/croutine.cyclo ./ThirdParty/RTOS/croutine.d ./ThirdParty/RTOS/croutine.o ./ThirdParty/RTOS/croutine.su ./ThirdParty/RTOS/event_groups.cyclo ./ThirdParty/RTOS/event_groups.d ./ThirdParty/RTOS/event_groups.o ./ThirdParty/RTOS/event_groups.su ./ThirdParty/RTOS/list.cyclo ./ThirdParty/RTOS/list.d ./ThirdParty/RTOS/list.o ./ThirdParty/RTOS/list.su ./ThirdParty/RTOS/queue.cyclo ./ThirdParty/RTOS/queue.d ./ThirdParty/RTOS/queue.o ./ThirdParty/RTOS/queue.su ./ThirdParty/RTOS/stream_buffer.cyclo ./ThirdParty/RTOS/stream_buffer.d ./ThirdParty/RTOS/stream_buffer.o ./ThirdParty/RTOS/stream_buffer.su ./ThirdParty/RTOS/tasks.cyclo ./ThirdParty/RTOS/tasks.d ./ThirdParty/RTOS/tasks.o ./ThirdParty/RTOS/tasks.su ./ThirdParty/RTOS/timers.cyclo ./ThirdParty/RTOS/timers.d ./ThirdParty/RTOS/timers.o ./ThirdParty/RTOS/timers.su

.PHONY: clean-ThirdParty-2f-RTOS

