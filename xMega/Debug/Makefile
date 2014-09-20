################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS :=  \
../include \
../drivers \
../src/


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../drivers/adc_driver.c \
../drivers/clksys_driver.c \
../drivers/TC_driver.c \
../drivers/twi_master_driver.c \
../drivers/usart_driver.c \
../drivers/wdt_driver.c \
../src/battery.c \
../src/encoder.c \
../src/fsm.c \
../src/pcInterface.c \
../src/sensors.c \
../src/xMega.c \
../src/system.c


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
drivers/adc_driver.o \
drivers/clksys_driver.o \
drivers/TC_driver.o \
drivers/twi_master_driver.o \
drivers/usart_driver.o \
drivers/wdt_driver.o \
src/battery.o \
src/encoder.o \
src/fsm.o \
src/pcInterface.o \
src/sensors.o \
src/xMega.o \
src/system.o

OBJS_AS_ARGS +=  \
drivers/adc_driver.o \
drivers/clksys_driver.o \
drivers/TC_driver.o \
drivers/twi_master_driver.o \
drivers/usart_driver.o \
drivers/wdt_driver.o \
src/battery.o \
src/encoder.o \
src/fsm.o \
src/pcInterface.o \
src/sensors.o \
src/xMega.o \
src/system.o

C_DEPS +=  \
drivers/adc_driver.d \
drivers/clksys_driver.d \
drivers/TC_driver.d \
drivers/twi_master_driver.d \
drivers/usart_driver.d \
drivers/wdt_driver.d \
src/battery.d \
src/encoder.d \
src/fsm.d \
src/pcInterface.d \
src/sensors.d \
src/xMega.d \
src/system.d

C_DEPS_AS_ARGS +=  \
drivers/adc_driver.d \
drivers/clksys_driver.d \
drivers/TC_driver.d \
drivers/twi_master_driver.d \
drivers/usart_driver.d \
drivers/wdt_driver.d \
src/battery.d \
src/encoder.d \
src/fsm.d \
src/pcInterface.d \
src/sensors.d \
src/xMega.d \
src/system.d

OUTPUT_FILE_PATH +=xMega.elf

OUTPUT_FILE_PATH_AS_ARGS +=xMega.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

# AVR32/GNU C Compiler



























drivers/%.o: ../drivers/%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.8.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DIOPORT_XMEGA_COMPAT  -I"../src" -I"../drivers" -I"../include"  -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g2 -Wall -mmcu=atxmega128a1 -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

src/%.o: ../src/%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.8.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DIOPORT_XMEGA_COMPAT  -I"../src" -I"../drivers" -I"../include"  -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g2 -Wall -mmcu=atxmega128a1 -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP)
	@echo Building target: $@
	@echo Invoking: AVR/GNU Linker : 4.8.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="xMega.map" -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections -mmcu=atxmega128a1  
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  "xMega.elf" "xMega.hex"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "xMega.elf" "xMega.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\bin\avr-objdump.exe" -h -S "xMega.elf" > "xMega.lss"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures "xMega.elf" "xMega.srec"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\bin\avr-size.exe" "xMega.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "xMega.elf" "xMega.a" "xMega.hex" "xMega.lss" "xMega.eep" "xMega.map" "xMega.srec" "xMega.usersignatures"
	