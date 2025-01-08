#ifndef LOGG_H
#define LOGG_H

#include <stdio.h>
#include <stdarg.h>

#include "types.h"

typedef enum logg_level {
    LOGG_TRACE = 0,
    LOGG_DEBUG = 1,
    LOGG_INFO  = 2,
    LOGG_WARN  = 3,
    LOGG_ERROR = 4,
} logg_level_t;

void logg_set_level(logg_level_t level);
void logg_print(logg_level_t level, const char* msg);
void logg_printf(logg_level_t level, const char* msg, ...);
void logg_fprint(FILE* file, logg_level_t level, const char* msg);
void logg_fprintf(FILE* file, logg_level_t level, const char* msg, ...);
void logg_exit(i32 ec, logg_level_t level, const char* msg);
void logg_exitf(i32 ec, logg_level_t level, const char* msg, ...);
void logg_fexit(FILE* file, i32 ec, logg_level_t level, const char* msg);
void logg_fexitf(FILE* file, i32 ec, logg_level_t level, const char* msg, ...);

#endif // LOGG_H