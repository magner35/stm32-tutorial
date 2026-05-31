.cpu cortex-m3
.syntax unified
.thumb

.thumb_func
.global reset_exception_handler

.section .text

reset_exception_handler:
add r0, r0, 1
b reset_exception_handler
