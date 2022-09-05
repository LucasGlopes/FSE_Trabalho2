#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

FILE *log;

struct tm *get_time(){
    time_t now;

    time(&now);

    struct tm *current_time = localtime(&now);
 
    return current_time;
}

void create_file(){
    log = fopen("log.csv", "w+");
    fprintf(log,"Data, Hora, Temp. Interna (°C), Temp. Externa (°C), Temp. de Referência (°C), Atuador (%)\n");
}

void write_log(float ti, float te, float tr, int intensity){
    int hours, minutes, seconds, day, month, year;

    struct tm *current_time = get_time();
    hours = current_time->tm_hour;         
    minutes = current_time->tm_min;        
    seconds = current_time->tm_sec;        
 
    day = current_time->tm_mday;            
    month = current_time->tm_mon + 1;      
    year = current_time->tm_year + 1900;   

    fprintf(log,"%02d/%02d/%d, %02d:%02d:%02d, %.2f, %.2f, %.2f, %d\n", day, month, year, hours, minutes, seconds, ti, te, tr, intensity);

}

void close_file(){
    fclose(log);
}
