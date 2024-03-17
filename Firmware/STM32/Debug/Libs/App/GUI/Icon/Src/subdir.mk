################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libs/App/GUI/Icon/Src/icon_check.c \
../Libs/App/GUI/Icon/Src/icon_delete.c \
../Libs/App/GUI/Icon/Src/icon_wifi.c 

OBJS += \
./Libs/App/GUI/Icon/Src/icon_check.o \
./Libs/App/GUI/Icon/Src/icon_delete.o \
./Libs/App/GUI/Icon/Src/icon_wifi.o 

C_DEPS += \
./Libs/App/GUI/Icon/Src/icon_check.d \
./Libs/App/GUI/Icon/Src/icon_delete.d \
./Libs/App/GUI/Icon/Src/icon_wifi.d 


# Each subdirectory must supply rules for building sources it contributes
Libs/App/GUI/Icon/Src/%.o Libs/App/GUI/Icon/Src/%.su Libs/App/GUI/Icon/Src/%.cyclo: ../Libs/App/GUI/Icon/Src/%.c Libs/App/GUI/Icon/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"D:/BTL_ESD/Firmware/STM32/Libs/Driver/ILI9341" -I"D:/BTL_ESD/Firmware/STM32/Libs/Driver/XPT2046" -I"D:/BTL_ESD/Firmware/STM32/Libs/Middleware/Calibrate" -I"D:/BTL_ESD/Firmware/STM32/Libs/Middleware/Graphics" -I"D:/BTL_ESD/Firmware/STM32/Libs/App/GUI/Icon" -I"D:/BTL_ESD/Firmware/STM32/Libs/App/GUI/Screen" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Libs-2f-App-2f-GUI-2f-Icon-2f-Src

clean-Libs-2f-App-2f-GUI-2f-Icon-2f-Src:
	-$(RM) ./Libs/App/GUI/Icon/Src/icon_check.cyclo ./Libs/App/GUI/Icon/Src/icon_check.d ./Libs/App/GUI/Icon/Src/icon_check.o ./Libs/App/GUI/Icon/Src/icon_check.su ./Libs/App/GUI/Icon/Src/icon_delete.cyclo ./Libs/App/GUI/Icon/Src/icon_delete.d ./Libs/App/GUI/Icon/Src/icon_delete.o ./Libs/App/GUI/Icon/Src/icon_delete.su ./Libs/App/GUI/Icon/Src/icon_wifi.cyclo ./Libs/App/GUI/Icon/Src/icon_wifi.d ./Libs/App/GUI/Icon/Src/icon_wifi.o ./Libs/App/GUI/Icon/Src/icon_wifi.su

.PHONY: clean-Libs-2f-App-2f-GUI-2f-Icon-2f-Src

