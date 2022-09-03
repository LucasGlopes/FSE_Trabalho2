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
#include "../inc/i2c_lcd.h"

int uart;


void finish_device(){
    deactivate_resistor();
    deactivate_fan();
    ClrLcd();
    bme280_driver_close();
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
    lcd_init();
    struct bme280_dev dev;
    dev = connect_bme();

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

        // print_lcd(TI, TR, get_ambient_temperature(&dev));
        // print_lcd(TI, TR, stream_sensor_data_forced_mode(&dev));
        print_lcd(TI, TR);


        sleep(1);

    }while(abs(TR - TI) > 0.05);


    TE = get_ambient_temperature();
    if(TE == -1) TE = 24;
    printf("Temperatura ambiente: %f\n", TE);


    pid_atualiza_referencia(TE);
    do{
        request_data(uart, REQUEST_INTERNAL_TEMPERATURE);
        TI = get_data(uart);

        printf("Temperatura interna: %f\n", TI);


        intensity = pid_controle(TI);

        pwm_control(intensity);
        print_lcd(TI, TE);

        sleep(1);

    }while(abs(TE - TI) > 0.5);

    ClrLcd();
    deactivate_resistor();
    deactivate_fan();
    bme280_driver_close();

}

int main(int argc, const char * argv[]) {
    signal(SIGINT, finish_device);


    uart = start_uart();
    // request_data(uart, REQUEST_INTERNAL_TEMPERATURE);
    // float ti =  get_data(uart);
    // printf("Temperatura interna: %f\n", get_data(uart));
    // // get_data(uart);
    // request_data(uart, REQUEST_REFERENCE_TEMPERATURE);
    // float tr = get_data(uart);
    // printf("Temperatura de referencia: %f\n", get_data(uart));
    // // get_data(uart);
    // printf("Temperatura ambiente: %f\n", get_ambient_temperature());

    airfryer();
    // print_lcd(TI, tr, get_ambient_temperature());
    close_uart(uart);


   return 0;
}

// void print_menu(){
//     printf("======== AIR FRYER ========\n\n");
//     printf("Escolha sua opção:\n");
//     printf("1 -")
// }

