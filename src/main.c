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


void airfyer_routine(int tref, int selected_time, int type_heating){
    int intensity;
    int current_time = selected_time * 60;
    int timerStatus = 1;

    TI = 0;
    TR = tref;
    
    pid_atualiza_referencia(TR);

    while(timerStatus == 1){
        if(type_heating == 1){
            request_data(uart, REQUEST_REFERENCE_TEMPERATURE);
            TR = get_data(uart);
            pid_atualiza_referencia(TR);
        }

        request_data(uart, REQUEST_INTERNAL_TEMPERATURE);
        TI = get_data(uart);

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

        pid_atualiza_referencia(TE);

        request_data(uart, REQUEST_INTERNAL_TEMPERATURE);
        TI = get_data(uart);


        intensity = pid_controle(TI);

        pwm_control(intensity);
        print_status(2);

        write_log(TI, TE, TR,intensity);

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
    airfyer_routine(choices[option - 1][0], choices[option - 1][1], 2);
}

void panel_control_menu(){
    int selected_time;

    printf("======== Painel ========\n\n");
    printf("Digite o tempo (em minutos) ou digite 0(zero) para voltar ao menu inicial:\n");
    printf("A temperatura é definida pelo painel de controle!\n");

    scanf("%d", &selected_time);
    while(selected_time < 0){
        printf("Digite um valor maior do que zero:\n");
        scanf("%d", &selected_time); 
    }
    if(selected_time == 0) return;
    airfyer_routine(0, selected_time, 1);

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
    print_menu();

    finish_device();

   return 0;
}



