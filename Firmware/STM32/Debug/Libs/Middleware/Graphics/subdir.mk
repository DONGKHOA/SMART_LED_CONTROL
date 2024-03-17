################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libs/Middleware/Graphics/graphics.c 

OBJS += \
./Libs/Middleware/Graphics/graphics.o 

C_DEPS += \
./Libs/Middleware/Graphics/graphics.d 


# Each subdirectory must supply rules for building sources it contributes
Libs/Middleware/Graphics/%.o Libs/Middleware/Graphics/%.su Libs/Middleware/Graphics/%.cyclo: ../Libs/Middleware/Graphics/%.c Libs/Middleware/Graphics/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"D:/BTL_ESD/Firmware/STM32/Libs/Driver/ILI9341" -I"D:/BTL_ESD/Firmware/STM32/Libs/Driver/XPT2046" -I"D:/BTL_ESD/Firmware/STM32/Libs/Middleware/Calibrate" -I"D:/BTL_ESD/Firmware/STM32/Libs/Middleware/Graphics" -I"D:/BTL_ESD/Firmware/STM32/Libs/App/GUI/Icon" -I"D:/BTL_ESD/Firmware/STM32/Libs/App/GUI/Screen" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Libs-2f-Middleware-2f-Graphics

clean-Libs-2f-Middleware-2f-Graphics:
	-$(RM) ./Libs/Middleware/Graphics/graphics.cyclo ./Libs/Middleware/Graphics/graphics.d ./Libs/Middleware/Graphics/graphics.o ./Libs/Middleware/Graphics/graphics.su

.PHONY: clean-Libs-2f-Middleware-2f-Graphics

