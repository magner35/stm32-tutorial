CPPFLAGS := -I$(CMSIS_STM32F1)/Device/ST/STM32F1xx/Include
CPPFLAGS += -I$(CMSIS_STM32F1)/Include
CPPFLAGS += -DSTM32F103xB
CFLAGS := -mcpu=cortex-m3 -g -O0 -std=c17 -Wall -Wextra -Wpedantic
LDSCRIPT := linker.ld
LDFLAGS := -Wl,-T,$(LDSCRIPT)

usart.bin: usart.elf
	arm-none-eabi-objcopy -O binary usart.elf usart.bin

flash: usart.bin
	st-flash --connect-under-reset write usart.bin 0x08000000

usart.elf: linker.ld reset_exception_handler.o usart.o
	arm-none-eabi-ld -T $(LDSCRIPT) -o usart.elf reset_exception_handler.o usart.o

reset_exception_handler.o: reset_exception_handler.s
	arm-none-eabi-as -o reset_exception_handler.o reset_exception_handler.s

usart.o: usart.c
	arm-none-eabi-gcc $(CPPFLAGS) $(CFLAGS) -c -o usart.o usart.c

clean:
	rm -f usart.bin usart.elf startup.o usart.o
