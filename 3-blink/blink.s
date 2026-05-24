.cpu cortex-m3
.syntax unified
.thumb

.global reset_exception_handler

.section code

reset_exception_handler:

// enable I/O port B clock
ldr r0, =0x40021000 + 0x18 // RCC_APB2ENR
ldr r1, [r0]
orr r1, 1 << 3 // IOPBEN
str r1, [r0]

// configure PB2 as push-pull output with 2 MHz speed
ldr r0, =0x40010C00 + 0x00 // GPIOB_CRL
ldr r1, [r0]
bic r1, 0x0f << 8
orr r1, 0b10 << 8 // CNF2:0
str r1, [r0]

blink_loop:

// wait loop
ldr r0, =1000000
delay_loop:
subs r0, 1
bne delay_loop

// toggle PB2
ldr r0, =0x40010C00 + 0x0c // GPIOB_ODR
ldr r1, [r0]
eor r1, 1 << 2 // ODR2
str r1, [r0]

b blink_loop
