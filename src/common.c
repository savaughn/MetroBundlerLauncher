#include "common.h"

void LOG(const char *restrict fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    printf("[%d:%d:%d] ", tm.tm_hour, tm.tm_min, tm.tm_sec);
    vprintf(fmt, args);
    va_end(args);
}