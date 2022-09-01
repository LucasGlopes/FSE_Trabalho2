#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>

#define RESISTOR_PIN 4
#define FAN_PIN 5


void setup_gpio(){
    wiringPiSetup();
    pinMode(RESISTOR_PIN, OUTPUT);
    pinMode(FAN_PIN, OUTPUT);
}

void activate_resistor(int intensity){
    softPwmCreate(RESISTOR_PIN, 0, 100);
    softPwmWrite(RESISTOR_PIN, intensity);
}

void activate_fan(int intensity){
    softPwmCreate(FAN_PIN, 0, 100);
    softPwmWrite(FAN_PIN, intensity);
}

void deactivate_resistor(){
    softPwmCreate(RESISTOR_PIN, 0, 100);
    softPwmWrite(RESISTOR_PIN, 0);
}

void deactivate_fan(){
    softPwmCreate(FAN_PIN, 0, 100);
    softPwmWrite(FAN_PIN, 0);
}

void pwm_control(int intensity){
    if(intensity >= 0){
        activate_resistor(intensity);
        deactivate_fan();
    }
    else if(intensity >= -40){
        activate_fan(40);
        deactivate_resistor();
    }else{
        activate_fan(intensity * (-1));
        deactivate_resistor();
    }
}