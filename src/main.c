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
#include "../inc/log.h"

int uart;
float TI, TR, TE;


void init_device(){
    double Kp = 30.0;
    double Ki = 0.2;
    double Kd = 400.0;

    uart = start_uart();
    connect_bme();
    pid_configura_constantes(Kp, Ki, Kd);
    setup_gpio();
    lcd_init();
    create_file();
}

void finish_device(){
    deactivate_resistor();
    deactivate_fan();
    ClrLcd();
    bme280_driver_close();
    close_uart(uart);
    close_file();
    exit(0);
}

// void panel_routine(){
//     float TI, TR, TE;
//     double Kp = 30.0;
//     double Ki = 0.2;
//     double Kd = 400.0;

//     int intensity;



//     do{
//         request_data(uart, REQUEST_REFERENCE_TEMPERATURE);
//         TR = get_data(uart);

//         printf("Temperatura de referencia: %f\n", TR);


//         pid_atualiza_referencia(TR);

//         request_data(uart, REQUEST_INTERNAL_TEMPERATURE);
//         TI = get_data(uart);

//         printf("Temperatura interna: %f\n", TI);


//         intensity = pid_controle(TI);

//         pwm_control(intensity);

//         print_lcd(TI, TR);

//         sleep(1);

//     }while(abs(TR - TI) > 0.05);


//     TE = get_ambient_temperature();
//     if(TE == -1) TE = 24;
//     printf("Temperatura ambiente: %f\n", TE);


//     pid_atualiza_referencia(TE);
//     do{
//         request_data(uart, REQUEST_INTERNAL_TEMPERATURE);
//         TI = get_data(uart);

//         printf("Temperatura interna: %f\n", TI);


//         intensity = pid_controle(TI);

//         pwm_control(intensity);
//         print_lcd(TI, TE);

//         sleep(1);

//     }while(abs(TE - TI) > 0.5);

//     ClrLcd();
//     deactivate_resistor();
//     deactivate_fan();
//     bme280_driver_close();

// }

void selected_food_routine(int tref, int selected_time){
    int intensity;
    int current_time = selected_time * 60;
    int timerStatus = 1;

    pthread_t log_thread_id;
    pthread_create(&log_thread_id, NULL, (void*)control_log, NULL);

    TI = 0;
    TR = tref;
    
    printf("Temperatura de referencia: %f\n", TR);
    pid_atualiza_referencia(TR);

    while(timerStatus == 1){
        request_data(uart, REQUEST_INTERNAL_TEMPERATURE);
        TI = get_data(uart);

        printf("Temperatura interna: %f\n", TI);

        intensity = pid_controle(TI);

        pwm_control(intensity);

        TE = get_ambient_temperature();

        if(current_time < 0){
            timerStatus = 0;
        }
        else if(abs(TR - TI) < 0.5){
            print_ti_tr(TI, TR, current_time);
            current_time--;
        }else{
            print_status(1);
        }
        write_log(TI, TE, TR,intensity);

    }
   
    do{
        TE = get_ambient_temperature();
        if(TE == -1) TE = 24;
        printf("Temperatura ambiente: %f\n", TE);

        pid_atualiza_referencia(TE);

        request_data(uart, REQUEST_INTERNAL_TEMPERATURE);
        TI = get_data(uart);

        printf("Temperatura interna: %f\n", TI);

        intensity = pid_controle(TI);

        pwm_control(intensity);
        print_status(2);

        write_log(TI, TE, TR,intensity);

        sleep(1);
    }while(abs(TE - TI) > 0.5);

    ClrLcd();
    deactivate_resistor();
    deactivate_fan();
}

void food_menu(){
    int option;
    int choices [4][2] = {{35, 1}, {40, 2}, {60, 1}, {30, 3}};

    printf("======== Alimentos Pré-definidos ========\n");
    printf("Escolha sua opção:\n");
    printf("1 - Frango - 35°C - 1min\n");
    printf("2 - Batata - 40°C - 2min\n");
    printf("3 - Almôndega - 60°C - 1min\n");
    printf("4 - Pastel - 30°C - 3min\n");
    printf("5 - Voltar ao menu inicial\n");
    printf("===========================\n\n");

    scanf("%d", &option);
    while(option < 1 || option > 5){
        printf("Digite uma opção válida (1, 2 ou 3):\n");
        scanf("%d", &option); 
    }

    if(option == 5) return;
    selected_food_routine(choices[option - 1][0], choices[option - 1][1]);
}

void panel_control_menu(){
    int selected_time;

    printf("======== Painel ========\n\n");
    printf("Digite o tempo (em minutos):\n");
    printf("A temperatura é definida pelo painel de controle!\n");

    scanf("%d", &selected_time);

}


void print_menu(){
    int option;

    while(1){
        printf("======== AIR FRYER ========\n");
        printf("Escolha sua opção:\n");
        printf("1 - Painel de Controle\n");
        printf("2 - Escolher alimento pré-selecionado\n");
        printf("3 - Sair\n");
        printf("===========================\n\n");

        scanf("%d", &option);
        while(option < 1 || option > 3){
            printf("Digite uma opção válida (1, 2 ou 3):\n");
            scanf("%d", &option); 
        }

        if(option == 1){
            panel_control_menu();
        }
        else if(option == 2){
            food_menu();
        }
        else{
            return;
        }
    }

}

int main(int argc, const char * argv[]) {
    signal(SIGINT, finish_device);

    init_device();
    // panel_routine();
    print_menu();
    printf("deu certo\n");

    finish_device();

   return 0;
}



