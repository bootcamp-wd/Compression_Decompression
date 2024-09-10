#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <time.h>

enum { LOG_TRACE, LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR, LOG_FATAL };

//Logging at different levels, including file name and line number
#define log_trace(...) log_log(LOG_TRACE, __FILE__, __LINE__, __VA;_ARGS__)
#define log_debug(...) log_log(LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define log_info(...)  log_log(LOG_INFO,  __FILE__, __LINE__, __VA_ARGS__)
#define log_warn(...)  log_log(LOG_WARN,  __FILE__, __LINE__, __VA_ARGS__)
#define log_error(...) log_log(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define log_fatal(...) log_log(LOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)

// Function to set the minimum log level. Logs below this level will be ignored.
void log_set_level(int level);

// Function to enable or disable console logging. If disabled, logs only to files.
void log_set_quiet(bool enable);

// Function to set a file pointer for logging to a file. Specify the minimum log level for this file.
void log_add_fp(FILE* file_p, int level);

// Core logging function called by the macros.
void log_log(int level, const char* file, int line, const char* fmt, ...);


#endif // LOG_H