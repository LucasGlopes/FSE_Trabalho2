#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>         //Used for UART
#include <fcntl.h>          //Used for UART
#include <termios.h>        //Used for UART
#include "../inc/uart.h"
#include "../inc/12c_bme.h"
#include "../inc/consts.h"
#include "../inc/pid.h"
#include "../inc/pwm.h"

int uart;


void parou(){
    deactivate_resistor();
    deactivate_fan();
    close_uart(uart);
    exit(0);
}

void airfryer(){
    float TI, TR, TE;
    double Kp = 30.0;
    double Ki = 0.2;
    double Kd = 400.0;

    int intensity;

    pid_configura_constantes(Kp, Ki, Kd);

    setup_gpio();

    do{
        request_data(uart, REQUEST_REFERENCE_TEMPERATURE);
        TR = get_data(uart);

        printf("Temperatura de referencia: %f\n", TR);


        pid_atualiza_referencia(TR);

        request_data(uart, REQUEST_INTERNAL_TEMPERATURE);
        TI = get_data(uart);

        printf("Temperatura interna: %f\n", TI);


        intensity = pid_controle(TI);

        pwm_control(intensity);
        sleep(1);

    }while(abs(TR - TI) > 0.5);

    printf("DEU CERTO\n");

    do{
        TE = get_ambient_temperature();


        printf("Temperatura ambiente: %f\n", TE);


        pid_atualiza_referencia(TE);

        request_data(uart, REQUEST_INTERNAL_TEMPERATURE);
        TI = get_data(uart);

        printf("Temperatura interna: %f\n", TI);


        intensity = pid_controle(TI);

        pwm_control(intensity);
        sleep(1);

    }while(abs(TE - TI) > 0.5);

    deactivate_resistor();
    deactivate_fan();

}

int main(int argc, const char * argv[]) {
    signal(SIGINT, parou);


    uart = start_uart();
    // request_data(uart, REQUEST_INTERNAL_TEMPERATURE);
    // printf("Temperatura interna: %f\n", get_data(uart));
    // // get_data(uart);
    // request_data(uart, REQUEST_REFERENCE_TEMPERATURE);
    // printf("Temperatura de referencia: %f\n", get_data(uart));
    // // get_data(uart);
    // printf("Temperatura ambiente: %f\n", get_ambient_temperature());
    airfryer();
    close_uart(uart);


   return 0;
}


