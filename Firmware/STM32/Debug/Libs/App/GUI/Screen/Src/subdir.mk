################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libs/App/GUI/Screen/Src/screen_1.c \
../Libs/App/GUI/Screen/Src/screen_2.c \
../Libs/App/GUI/Screen/Src/screen_3.c \
../Libs/App/GUI/Screen/Src/screen_4.c 

OBJS += \
./Libs/App/GUI/Screen/Src/screen_1.o \
./Libs/App/GUI/Screen/Src/screen_2.o \
./Libs/App/GUI/Screen/Src/screen_3.o \
./Libs/App/GUI/Screen/Src/screen_4.o 

C_DEPS += \
./Libs/App/GUI/Screen/Src/screen_1.d \
./Libs/App/GUI/Screen/Src/screen_2.d \
./Libs/App/GUI/Screen/Src/screen_3.d \
./Libs/App/GUI/Screen/Src/screen_4.d 


# Each subdirectory must supply rules for building sources it contributes
Libs/App/GUI/Screen/Src/%.o Libs/App/GUI/Screen/Src/%.su Libs/App/GUI/Screen/Src/%.cyclo: ../Libs/App/GUI/Screen/Src/%.c Libs/App/GUI/Screen/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"D:/BTL_ESD/Firmware/STM32/Libs/Driver/ILI9341" -I"D:/BTL_ESD/Firmware/STM32/Libs/Driver/XPT2046" -I"D:/BTL_ESD/Firmware/STM32/Libs/Middleware/Calibrate" -I"D:/BTL_ESD/Firmware/STM32/Libs/Middleware/Graphics" -I"D:/BTL_ESD/Firmware/STM32/Libs/App/GUI/Icon" -I"D:/BTL_ESD/Firmware/STM32/Libs/App/GUI/Screen" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Libs-2f-App-2f-GUI-2f-Screen-2f-Src

clean-Libs-2f-App-2f-GUI-2f-Screen-2f-Src:
	-$(RM) ./Libs/App/GUI/Screen/Src/screen_1.cyclo ./Libs/App/GUI/Screen/Src/screen_1.d ./Libs/App/GUI/Screen/Src/screen_1.o ./Libs/App/GUI/Screen/Src/screen_1.su ./Libs/App/GUI/Screen/Src/screen_2.cyclo ./Libs/App/GUI/Screen/Src/screen_2.d ./Libs/App/GUI/Screen/Src/screen_2.o ./Libs/App/GUI/Screen/Src/screen_2.su ./Libs/App/GUI/Screen/Src/screen_3.cyclo ./Libs/App/GUI/Screen/Src/screen_3.d ./Libs/App/GUI/Screen/Src/screen_3.o ./Libs/App/GUI/Screen/Src/screen_3.su ./Libs/App/GUI/Screen/Src/screen_4.cyclo ./Libs/App/GUI/Screen/Src/screen_4.d ./Libs/App/GUI/Screen/Src/screen_4.o ./Libs/App/GUI/Screen/Src/screen_4.su

.PHONY: clean-Libs-2f-App-2f-GUI-2f-Screen-2f-Src

