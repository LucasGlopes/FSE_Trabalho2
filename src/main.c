#include <stdio.h>
#include <unistd.h>         //Used for UART
#include <fcntl.h>          //Used for UART
#include <termios.h>        //Used for UART
#include "../inc/uart.h"

int main(int argc, const char * argv[]) {
    int uart;

    uart = start_uart();
    request_data(uart);
    get_data(uart);
    close_uart(uart);


   return 0;
}
