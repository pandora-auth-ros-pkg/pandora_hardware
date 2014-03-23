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
../src/ \
../src/ASF/ \
../src/ASF/common/ \
../src/ASF/common/boards/ \
../src/ASF/common/boards/user_board/ \
../src/ASF/common/services/ \
../src/ASF/common/services/clock/ \
../src/ASF/common/services/clock/xmega/ \
../src/ASF/common/services/ioport/ \
../src/ASF/common/services/ioport/xmega/ \
../src/ASF/common/utils/ \
../src/ASF/common/utils/interrupt/ \
../src/ASF/common/utils/make/ \
../src/ASF/xmega/ \
../src/ASF/xmega/drivers/ \
../src/ASF/xmega/drivers/cpu/ \
../src/ASF/xmega/drivers/pmic/ \
../src/ASF/xmega/drivers/usart/ \
../src/ASF/xmega/utils/ \
../src/ASF/xmega/utils/assembler/ \
../src/ASF/xmega/utils/bit_handling/ \
../src/ASF/xmega/utils/preprocessor/ \
../src/config/ \
../src/include/


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../src/usart_driver.c \
../src/ASF/common/boards/user_board/init.c \
../src/ASF/common/services/clock/xmega/sysclk.c \
../src/ASF/common/services/ioport/xmega/ioport_compat.c \
../src/ASF/xmega/drivers/usart/usart.c \
../src/main.c \
../src/system.c \
../src/USART.c


PREPROCESSING_SRCS +=  \
../src/ASF/xmega/drivers/cpu/ccp.s


ASM_SRCS += 


OBJS +=  \
src/usart_driver.o \
src/ASF/common/boards/user_board/init.o \
src/ASF/common/services/clock/xmega/sysclk.o \
src/ASF/common/services/ioport/xmega/ioport_compat.o \
src/ASF/xmega/drivers/cpu/ccp.o \
src/ASF/xmega/drivers/usart/usart.o \
src/main.o \
src/system.o \
src/USART.o

OBJS_AS_ARGS +=  \
src/usart_driver.o \
src/ASF/common/boards/user_board/init.o \
src/ASF/common/services/clock/xmega/sysclk.o \
src/ASF/common/services/ioport/xmega/ioport_compat.o \
src/ASF/xmega/drivers/cpu/ccp.o \
src/ASF/xmega/drivers/usart/usart.o \
src/main.o \
src/system.o \
src/USART.o

C_DEPS +=  \
src/usart_driver.d \
src/ASF/common/boards/user_board/init.d \
src/ASF/common/services/clock/xmega/sysclk.d \
src/ASF/common/services/ioport/xmega/ioport_compat.d \
src/ASF/xmega/drivers/cpu/ccp.d \
src/ASF/xmega/drivers/usart/usart.d \
src/main.d \
src/system.d \
src/USART.d

C_DEPS_AS_ARGS +=  \
src/usart_driver.d \
src/ASF/common/boards/user_board/init.d \
src/ASF/common/services/clock/xmega/sysclk.d \
src/ASF/common/services/ioport/xmega/ioport_compat.d \
src/ASF/xmega/drivers/cpu/ccp.d \
src/ASF/xmega/drivers/usart/usart.d \
src/main.d \
src/system.d \
src/USART.d

OUTPUT_FILE_PATH +=xMega_refactored.elf

OUTPUT_FILE_PATH_AS_ARGS +=xMega_refactored.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

# AVR32/GNU C Compiler

















src/%.o: ../src/%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.8.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1051\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -DDEBUG -DBOARD=USER_BOARD  -I"../common/applications/user_application/user_board/config" -I"../src/ASF/xmega/utils" -I"../src/config" -I"../src/ASF/common/boards" -I"../src/ASF/xmega/utils/preprocessor" -I"../src/ASF/common/utils" -I"../src" -I"../src/ASF/common/boards/user_board" -I"../src/ASF/xmega/drivers/cpu" -I"../src/ASF/xmega/drivers/pmic" -I"../src/ASF/common/services/clock" -I"../src/ASF/common/services/ioport" -I"../src/ASF/xmega/drivers/usart"  -O1 -fdata-sections -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -mrelax -g3 -Wall -mmcu=atxmega128a1 -c -std=gnu99 -fno-strict-aliasing -Wstrict-prototypes -Wmissing-prototypes -Werror-implicit-function-declaration -Wpointer-arith -mrelax -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

src/ASF/common/boards/user_board/%.o: ../src/ASF/common/boards/user_board/%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.8.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1051\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -DDEBUG -DBOARD=USER_BOARD  -I"../common/applications/user_application/user_board/config" -I"../src/ASF/xmega/utils" -I"../src/config" -I"../src/ASF/common/boards" -I"../src/ASF/xmega/utils/preprocessor" -I"../src/ASF/common/utils" -I"../src" -I"../src/ASF/common/boards/user_board" -I"../src/ASF/xmega/drivers/cpu" -I"../src/ASF/xmega/drivers/pmic" -I"../src/ASF/common/services/clock" -I"../src/ASF/common/services/ioport" -I"../src/ASF/xmega/drivers/usart"  -O1 -fdata-sections -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -mrelax -g3 -Wall -mmcu=atxmega128a1 -c -std=gnu99 -fno-strict-aliasing -Wstrict-prototypes -Wmissing-prototypes -Werror-implicit-function-declaration -Wpointer-arith -mrelax -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

src/ASF/common/services/clock/xmega/%.o: ../src/ASF/common/services/clock/xmega/%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.8.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1051\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -DDEBUG -DBOARD=USER_BOARD  -I"../common/applications/user_application/user_board/config" -I"../src/ASF/xmega/utils" -I"../src/config" -I"../src/ASF/common/boards" -I"../src/ASF/xmega/utils/preprocessor" -I"../src/ASF/common/utils" -I"../src" -I"../src/ASF/common/boards/user_board" -I"../src/ASF/xmega/drivers/cpu" -I"../src/ASF/xmega/drivers/pmic" -I"../src/ASF/common/services/clock" -I"../src/ASF/common/services/ioport" -I"../src/ASF/xmega/drivers/usart"  -O1 -fdata-sections -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -mrelax -g3 -Wall -mmcu=atxmega128a1 -c -std=gnu99 -fno-strict-aliasing -Wstrict-prototypes -Wmissing-prototypes -Werror-implicit-function-declaration -Wpointer-arith -mrelax -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

src/ASF/common/services/ioport/xmega/%.o: ../src/ASF/common/services/ioport/xmega/%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.8.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1051\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -DDEBUG -DBOARD=USER_BOARD  -I"../common/applications/user_application/user_board/config" -I"../src/ASF/xmega/utils" -I"../src/config" -I"../src/ASF/common/boards" -I"../src/ASF/xmega/utils/preprocessor" -I"../src/ASF/common/utils" -I"../src" -I"../src/ASF/common/boards/user_board" -I"../src/ASF/xmega/drivers/cpu" -I"../src/ASF/xmega/drivers/pmic" -I"../src/ASF/common/services/clock" -I"../src/ASF/common/services/ioport" -I"../src/ASF/xmega/drivers/usart"  -O1 -fdata-sections -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -mrelax -g3 -Wall -mmcu=atxmega128a1 -c -std=gnu99 -fno-strict-aliasing -Wstrict-prototypes -Wmissing-prototypes -Werror-implicit-function-declaration -Wpointer-arith -mrelax -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

src/ASF/xmega/drivers/usart/%.o: ../src/ASF/xmega/drivers/usart/%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.8.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1051\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -DDEBUG -DBOARD=USER_BOARD  -I"../common/applications/user_application/user_board/config" -I"../src/ASF/xmega/utils" -I"../src/config" -I"../src/ASF/common/boards" -I"../src/ASF/xmega/utils/preprocessor" -I"../src/ASF/common/utils" -I"../src" -I"../src/ASF/common/boards/user_board" -I"../src/ASF/xmega/drivers/cpu" -I"../src/ASF/xmega/drivers/pmic" -I"../src/ASF/common/services/clock" -I"../src/ASF/common/services/ioport" -I"../src/ASF/xmega/drivers/usart"  -O1 -fdata-sections -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -mrelax -g3 -Wall -mmcu=atxmega128a1 -c -std=gnu99 -fno-strict-aliasing -Wstrict-prototypes -Wmissing-prototypes -Werror-implicit-function-declaration -Wpointer-arith -mrelax -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler
src/ASF/xmega/drivers/cpu/ccp.o: ../src/ASF/xmega/drivers/cpu/ccp.s
	@echo Building file: $<
	@echo Invoking: AVR32/GNU Assembler : 4.8.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1051\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE) -Wa,-gdwarf2 -x assembler-with-cpp -c -mmcu=atxmega128a1 -mrelax -DBOARD=USER_BOARD -I "../common/applications/user_application/user_board/config" -I "../src/ASF/xmega/utils" -I "../src/config" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils" -I "../src" -I "../src/ASF/common/boards/user_board" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/pmic" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/services/ioport" -I "../src/ASF/xmega/drivers/usart"  -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -mrelax -Wa,-g   -o "$@" "$<" 
	@echo Finished building: $<
	


src/ASF/xmega/drivers/cpu/%.o: ../src/ASF/xmega/drivers/cpu/%.s
	@echo Building file: $<
	@echo Invoking: AVR32/GNU Assembler : 4.8.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1051\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE) -Wa,-gdwarf2 -x assembler-with-cpp -c -mmcu=atxmega128a1 -mrelax -DBOARD=USER_BOARD -I "../common/applications/user_application/user_board/config" -I "../src/ASF/xmega/utils" -I "../src/config" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils" -I "../src" -I "../src/ASF/common/boards/user_board" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/pmic" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/services/ioport" -I "../src/ASF/xmega/drivers/usart"  -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -mrelax -Wa,-g   -o "$@" "$<" 
	@echo Finished building: $<
	




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
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1051\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="xMega_refactored.map" -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections -mrelax -mmcu=atxmega128a1 -Wl,--relax  
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1051\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  "xMega_refactored.elf" "xMega_refactored.hex"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1051\avr8-gnu-toolchain\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "xMega_refactored.elf" "xMega_refactored.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1051\avr8-gnu-toolchain\bin\avr-objdump.exe" -h -S "xMega_refactored.elf" > "xMega_refactored.lss"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1051\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures "xMega_refactored.elf" "xMega_refactored.srec"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1051\avr8-gnu-toolchain\bin\avr-size.exe" "xMega_refactored.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "xMega_refactored.elf" "xMega_refactored.a" "xMega_refactored.hex" "xMega_refactored.lss" "xMega_refactored.eep" "xMega_refactored.map" "xMega_refactored.srec" "xMega_refactored.usersignatures"
	