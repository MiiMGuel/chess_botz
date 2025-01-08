#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "logg.h"

static logg_level_t _logg_level = LOGG_TRACE;
static const char* _logg_level_tag[] = {
    "TRACE",
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR"
};

static void _logg_pformat(logg_level_t level, const char* msg, va_list args) {
    time_t now; time(&now);
    char* date = ctime(&now);
    date[strlen(date) - 1] = '\0';

    printf("[%s][%s]: ", _logg_level_tag[level], date);
    vprintf(msg, args);
    printf("\n");
}

static void _logg_fpformat(FILE* file, logg_level_t level, const char* msg, va_list args) {
    time_t now; time(&now);
    char* date = ctime(&now);
    date[strlen(date) - 1] = '\0';

    fprintf(file, "[%s][%s]: ", _logg_level_tag[level], date);
    vfprintf(file, msg, args);
    fprintf(file, "\n");
}

static void _logg_exformat(int ec, logg_level_t level, const char* msg, va_list args) {
    time_t now; time(&now);
    char* date = ctime(&now);
    date[strlen(date) - 1] = '\0';

    printf("[EC:%d][%s][%s]: ", ec, _logg_level_tag[level], date);
    vprintf(msg, args);
    printf("\n");
}

static void _logg_fexformat(FILE* file, int ec, logg_level_t level, const char* msg, va_list args) {
    time_t now; time(&now);
    char* date = ctime(&now);
    date[strlen(date) - 1] = '\0';

    fprintf(file, "[EC:%d][%s][%s]: ", ec, _logg_level_tag[level], date);
    vfprintf(file, msg, args);
    fprintf(file, "\n");
}

void logg_set_level(logg_level_t level) {
    _logg_level = level;
}

void logg_print(logg_level_t level, const char* msg) {
    if (level >= _logg_level && level >= LOGG_ERROR) {
        time_t now; time(&now);
        char* date = ctime(&now);
        date[strlen(date) - 1] = '\0';

        printf("[%s][%s]: %s\n", _logg_level_tag[level], date, msg);
    }
}

void logg_printf(logg_level_t level, const char* msg, ...) {
    if (level >= _logg_level && level >= LOGG_ERROR) {
        va_list args; va_start(args, msg);
        _logg_pformat(level, msg, args);
        va_end(args);
    }
}

void logg_fprint(FILE* file, logg_level_t level, const char* msg) {
    if (level >= _logg_level && level >= LOGG_ERROR) {
        time_t now; time(&now);
        char* date = ctime(&now);
        date[strlen(date) - 1] = '\0';

        fprintf(file, "[%s][%s]: %s\n", _logg_level_tag[level], date, msg);
    }
}

void logg_fprintf(FILE* file, logg_level_t level, const char* msg, ...) {
    if (level >= _logg_level && level >= LOGG_ERROR) {
        va_list args; va_start(args, msg);
        _logg_fpformat(file, level, msg, args);
        va_end(args);
    }
}

void logg_exit(int ec, logg_level_t level, const char* msg) {
    if (level >= _logg_level && level >= LOGG_ERROR) {
        time_t now; time(&now);
        char* date = ctime(&now);
        date[strlen(date) - 1] = '\0';

        printf("[EC:%d][%s][%s]: %s\n", ec, _logg_level_tag[level], date, msg);
    } exit(ec);
}

void logg_exitf(int ec, logg_level_t level, const char* msg, ...) {
    if (level >= _logg_level && level >= LOGG_ERROR) {
        va_list args; va_start(args, msg);
        _logg_exformat(ec, level, msg, args);
        va_end(args);
    } exit(ec);
}

void logg_fexit(FILE* file, int ec, logg_level_t level, const char* msg) {
    if (level >= _logg_level && level >= LOGG_ERROR) {
        time_t now; time(&now);
        char* date = ctime(&now);
        date[strlen(date) - 1] = '\0';

        fprintf(file, "[EC:%d][%s][%s]: %s\n", ec, _logg_level_tag[level], date, msg);
    } exit(ec);
}

void logg_fexitf(FILE* file, int ec, logg_level_t level, const char* msg, ...) {
    if (level >= _logg_level && level >= LOGG_ERROR) {
        va_list args; va_start(args, msg);
        _logg_fexformat(file, ec, level, msg, args);
        va_end(args);
    } exit(ec);
}