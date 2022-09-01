#include <stdio.h>
#include <unistd.h>         //Used for UART
#include <fcntl.h>          //Used for UART
#include <termios.h>        //Used for UART
#include <string.h>

#include "../inc/consts.h"
#include "../inc/crc16.h"

int start_uart(){
    int uart0_filestream = -1;

    uart0_filestream = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY);      //Open in non blocking read/write mode
    if (uart0_filestream == -1)
    {
        printf("Erro - Não foi possível iniciar a UART.\n");
    }
    else
    {
        printf("UART inicializada!\n");
    }   

    struct termios options;
    tcgetattr(uart0_filestream, &options);
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;     //<Set baud rate
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(uart0_filestream, TCIFLUSH);
    tcsetattr(uart0_filestream, TCSANOW, &options);

    return uart0_filestream;
}

void request_data(int uart, unsigned char code){
    unsigned char data[7] = {SERVER_ADDRESS, REQUEST_CODE, code, 0, 1, 0, 2};
    short crc = calcula_CRC(data, 7);

    unsigned char message[9];
    memcpy(message, &data, 7);
    memcpy(&message[7], &crc, 2);

        
    if (uart != -1)
    {
        int count = write(uart, &message[0], 9);
        if (count < 0)
        {
            printf("UART TX error\n");
        }
    }

    sleep(1);

}

float get_data(int uart){
    float response = -1;
    if (uart!= -1)
    {
        // Read up to 255 characters from the port if they are there
        unsigned char rx_buffer[15];
        int rx_length = read(uart, (void*)rx_buffer, 15);      //Filestream, buffer to store in, number of bytes to read (max)
        if (rx_length < 0)
        {
            printf("Erro na leitura.\n"); //An error occured (will occur if there are no bytes)
        }
        else if (rx_length == 0)
        {
            printf("Nenhum dado disponível.\n"); //No data waiting
        }
        else
        {
            rx_buffer[rx_length] = '\0';
            memcpy(&response, &rx_buffer[3], sizeof(float));
            // printf("Temperatura: %f\n", response);
        }
    }
    
    return response;
}

void send_data(int uart){
    unsigned char header[7] = {SERVER_ADDRESS, SEND_CODE, SEND_REFERENCE_SIGNAL, 0, 1, 0, 2};

    unsigned char value[4];
    float temperature = 40.5;
    
    memcpy(value, &temperature, sizeof(float));

    unsigned char data[11];
    memcpy(data, &header, 7);
    memcpy(&data[7], &value, 4);

    short crc = calcula_CRC(data, 11);

    unsigned char message[13];
    memcpy(message, &data, 11);
    memcpy(&message[11], &crc, 2);

        
    if (uart != -1)
    {
        int count = write(uart, &message[0], 13);
        if (count < 0)
        {
            printf("UART TX error\n");
        }
    }

    sleep(1);
}

void close_uart(int uart){
    close(uart);
}