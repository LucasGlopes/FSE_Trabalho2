#ifndef UART_H_
#define UART_H_

int start_uart();
void request_data(int uart, unsigned char code);
void get_data(int uart);
void close_uart(int uart);
void send_data(int uart);

#endif