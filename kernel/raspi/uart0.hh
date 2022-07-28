#pragma once
#include <stdint.h>

// Initializes UART0.
void uart_init();

// putc over UART0
void uart_putc(int_fast8_t c);

// getc over UART0
int uart_getc();

// puts over UART0, but without the newline, because that one's a misdesign in C
void uart_puts(const char *s);
