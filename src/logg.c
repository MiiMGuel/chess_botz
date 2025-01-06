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
    time_t now;
    time(&now);
    char* date = ctime(&now);
    date[strlen(date) - 1] = '\0';

    printf("[%s][%s]: ", date, _logg_level_tag[level]);
    vprintf(msg, args);
    printf("\n");
}

static void _logg_fpformat(FILE* file, logg_level_t level, const char* msg, va_list args) {
    time_t now;
    time(&now);
    char* date = ctime(&now);
    date[strlen(date) - 1] = '\0';

    fprintf(file, "[%s][%s]: ", date, _logg_level_tag[level]);
    vfprintf(file, msg, args);
    fprintf(file, "\n");
}

void logg_set_level(logg_level_t level) {
    _logg_level = level;
}

void logg_print(logg_level_t level, const char* msg) {
    if (level >= _logg_level) {
        time_t now;
        time(&now);
        char* date = ctime(&now);
        date[strlen(date) - 1] = '\0';

        printf("[%s][%s]: %s\n", date, _logg_level_tag[level], msg);
    }
}

void logg_printf(logg_level_t level, const char* msg, ...) {
    if (level >= _logg_level) {
        va_list args;
        va_start(args, msg);
        _logg_pformat(level, msg, args);
        va_end(args);
    }
}

void logg_fprint(FILE* file, logg_level_t level, const char* msg) {
    if (level >= _logg_level) {
        time_t now;
        time(&now);
        char* date = ctime(&now);
        date[strlen(date) - 1] = '\0';

        fprintf(file, "[%s][%s]: %s\n", date, _logg_level_tag[level], msg);
    }
}

void logg_fprintf(FILE* file, logg_level_t level, const char* msg, ...) {
    if (level >= _logg_level) {
        va_list args;
        va_start(args, msg);
        _logg_fpformat(file, level, msg, args);
        va_end(args);
    }
}

void logg_exit(int ec, logg_level_t level, const char* msg) {
    logg_print(level, msg);
    exit(ec);
}

void logg_exitf(int ec, logg_level_t level, const char* msg, ...) {
    if (level >= _logg_level) {
        va_list args;
        va_start(args, msg);
        _logg_pformat(level, msg, args);
        va_end(args);
    } exit(ec);
}

void logg_fexit(FILE* file, int ec, logg_level_t level, const char* msg) {
    logg_printf(level, msg);
    exit(ec);
}

void logg_fexitf(FILE* file, int ec, logg_level_t level, const char* msg, ...) {
    if (level >= _logg_level) {
        va_list args;
        va_start(args, msg);
        _logg_fpformat(file, level, msg, args);
        va_end(args);
    } exit(ec);
}