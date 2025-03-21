################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../appli/button.c \
../appli/electrovanne.c \
../appli/foo.c \
../appli/hcsr04.c \
../appli/main.c \
../appli/mcp9701.c \
../appli/test_unitaire.c \
../appli/tft.c 

S_UPPER_SRCS += \
../appli/assembleur.S 

OBJS += \
./appli/assembleur.o \
./appli/button.o \
./appli/electrovanne.o \
./appli/foo.o \
./appli/hcsr04.o \
./appli/main.o \
./appli/mcp9701.o \
./appli/test_unitaire.o \
./appli/tft.o 

S_UPPER_DEPS += \
./appli/assembleur.d 

C_DEPS += \
./appli/button.d \
./appli/electrovanne.d \
./appli/foo.d \
./appli/hcsr04.d \
./appli/main.d \
./appli/mcp9701.d \
./appli/test_unitaire.d \
./appli/tft.d 


# Each subdirectory must supply rules for building sources it contributes
appli/%.o: ../appli/%.S appli/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m3 -g3 -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@" "$<"
appli/%.o: ../appli/%.c appli/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F1 -DNUCLEO_F103RB -DSTM32F103RBTx -DSTM32 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../lib/bsp -I../lib//middleware/MIDI/ -I../lib/bsp/VL53L0X -I../lib/bsp/MPU6050 -I../lib/bsp/iks01a1 -I../lib/bsp/iks01a1/Common -I../lib/bsp/iks01a1/hts221 -I../lib/bsp/iks01a1/lis3mdl -I../lib/bsp/iks01a1/lps22hb -I../lib/bsp/iks01a1/lps25hb -I../lib/bsp/iks01a1/lsm6ds0 -I../lib/bsp/iks01a1/lsm6ds3 -I../lib/bsp/tft_ili9341 -I../lib/bsp/tft_pcd8544 -I../lib/bsp/MCP23S17 -I../lib/hal/inc -I../lib/bsp/Common -I../lib/bsp/lcd2x16 -I../lib/bsp/MLX90614 -I../lib/bsp/MatrixKeyboard -I../lib/bsp/MatrixLed -I../lib/CMSIS/core -I../lib/CMSIS/device -I../lib/middleware/FatFs -I../lib/middleware/FatFs/src -I../lib/middleware/FatFs/src/drivers -I../appli -O0 -ffunction-sections -fdata-sections -Wall -Wextra -Wconversion -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"

