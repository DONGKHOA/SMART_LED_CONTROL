################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libs/Driver/ILI9341/ili9341.c 

OBJS += \
./Libs/Driver/ILI9341/ili9341.o 

C_DEPS += \
./Libs/Driver/ILI9341/ili9341.d 


# Each subdirectory must supply rules for building sources it contributes
Libs/Driver/ILI9341/%.o Libs/Driver/ILI9341/%.su Libs/Driver/ILI9341/%.cyclo: ../Libs/Driver/ILI9341/%.c Libs/Driver/ILI9341/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"D:/BTL_ESD/Firmware/STM32/Libs/Driver/ILI9341" -I"D:/BTL_ESD/Firmware/STM32/Libs/Driver/XPT2046" -I"D:/BTL_ESD/Firmware/STM32/Libs/Middleware/Calibrate" -I"D:/BTL_ESD/Firmware/STM32/Libs/Middleware/Graphics" -I"D:/BTL_ESD/Firmware/STM32/Libs/App/GUI/Icon" -I"D:/BTL_ESD/Firmware/STM32/Libs/App/GUI/Screen" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Libs-2f-Driver-2f-ILI9341

clean-Libs-2f-Driver-2f-ILI9341:
	-$(RM) ./Libs/Driver/ILI9341/ili9341.cyclo ./Libs/Driver/ILI9341/ili9341.d ./Libs/Driver/ILI9341/ili9341.o ./Libs/Driver/ILI9341/ili9341.su

.PHONY: clean-Libs-2f-Driver-2f-ILI9341

