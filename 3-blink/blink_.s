// target board STM32F103VET6 weactstudio

.cpu cortex-m3
.syntax unified
.thumb

.global reset_exception_handler

.section code

reset_exception_handler:

//begin
add r0, r1
add r0, r0, r1

and r0, r1
ands r0, r0, r1
ANDS r0, r0, r1

adds r0, #1
adds.n r0, #1
adds.w r0, #1


// enable I/O port E clock
ldr r0, =0x40021000 + 0x18 // RCC_APB2ENR
ldr r1, [r0]
orr r1, 1 << 6 // IOPEEN
str r1, [r0]

// configure PE6 as push-pull output with 2 MHz speed
ldr r0, =0x40011800 + 0x00 // GPIOE_CRL
ldr r1, [r0]
bic r1, 0x0f << 24
orr r1, 0b10 << 24 // CNF2:0
str r1, [r0]

blink_loop:

// wait loop
ldr r0, =5000000
delay_loop:
subs r0, 1
bne delay_loop

// toggle PB2
ldr r0, =0x40011800 + 0x0c // GPIOE_ODR
ldr r1, [r0]
eor r1, 1 << 6 // ODR6
str r1, [r0]

b blink_loop
