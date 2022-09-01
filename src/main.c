#include <stdio.h>
#include <unistd.h>         //Used for UART
#include <fcntl.h>          //Used for UART
#include <termios.h>        //Used for UART
#include "../inc/uart.h"
#include "../inc/12c_bme.h"
#include "../inc/consts.h"


int main(int argc, const char * argv[]) {
    int uart;

    uart = start_uart();
    request_data(uart, REQUEST_INTERNAL_TEMPERATURE);
    printf("Temperatura interna: \n");
    get_data(uart);
    request_data(uart, REQUEST_REFERENCE_TEMPERATURE);
    printf("Temperatura de referencia: \n");
    get_data(uart);
    printf("Temperatura ambiente: %f\n", get_ambient_temperature());
    close_uart(uart);


   return 0;
}

// void airfryer(){
//     float TI, TR, TE;


// }
