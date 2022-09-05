#ifndef LOG_H_
#define LOG_H_

struct tm *get_time();
void create_file();
void write_log(float ti, float te, float tr, int intensity);
void close_file();

#endif 