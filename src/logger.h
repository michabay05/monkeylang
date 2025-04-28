#ifndef _LOGGER_H_
#define _LOGGER_H_

typedef enum {
    LL_ERROR,
    LL_FATAL
} LogLevel;

void loggerf(LogLevel level, const char *filepath, int line_num, const char *fmt, ...);

#define errorf(fmt, ...) loggerf(LL_ERROR, __FILE__, __LINE__, fmt, __VA_ARGS__)
#define fatalf(fmt, ...) loggerf(LL_FATAL, __FILE__, __LINE__, fmt, __VA_ARGS__)

#endif // _LOGGER_H_

#ifdef LOGGER_IMPLEMENTATION
#define LOGGER_IMPLEMENTATION

void loggerf(LogLevel level, const char *filepath, int line_num, const char *fmt, ...)
{
    const char *label = NULL;
    switch (level) {
        case LL_ERROR:
            label = "ERROR";
            break;
        case LL_FATAL:
            label = "FATAL";
            break;
    }
    char buf[4*1024] = {0};
    snprintf(buf, 128, "[%s:%d] %s: %s", filepath, line_num, label, fmt);

    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, buf, args);
    va_end(args);
    fprintf(stderr, "\n");

    if (level == LL_FATAL)
        abort();
}

#endif // LOGGER_IMPLEMENTATION
