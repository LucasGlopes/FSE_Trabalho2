#ifndef PWM_H_
#define PWM_H_

void setup_gpio();
void activate_resistor(int intensity);
void activate_fan(int intensity);
void deactivate_resistor();
void deactivate_fan();
void pwm_control(int intensity);

#endif