#include "log.h"

// ANSI color codes for log levels
#define LOG_COLOR_TRACE "\x1b[94m"  // Light Blue
#define LOG_COLOR_DEBUG "\x1b[36m"  // Cyan
#define LOG_COLOR_INFO  "\x1b[32m"  // Green
#define LOG_COLOR_WARN  "\x1b[33m"  // Yellow
#define LOG_COLOR_ERROR "\x1b[31m"  // Red
#define LOG_COLOR_FATAL "\x1b[35m"  // Magenta
#define LOG_COLOR_RESET "\x1b[0m"   // Reset color to default

// Struct to hold information about a logging target (e.g., file).
typedef struct {
    FILE* fp;  // File pointer for logging to a file
    int level; // Minimum log level for this file
} LogTarget;

// Struct to hold the global logger state
static struct {
    int level;           // Minimum log level for all logs
    bool quiet;          // If true, suppress console output
    LogTarget file_target; // File logging target
} L = { LOG_TRACE, false, {NULL, LOG_INFO} };

typedef struct {
    va_list ap;           // Variable argument list for formatting the message
    const char* fmt;      // Format string for the log message
    const char* file;     // File name where the log originated
    int line;             // Line number in the file where the log originated
    int level;            // Log level (e.g., TRACE, DEBUG, INFO)
} log_Event;

// Array of strings representing the log levels
static const char* level_strings[] = {
    "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"
};

// Array of color codes for log levels
static const char* level_colors[] = {
    LOG_COLOR_TRACE, LOG_COLOR_DEBUG, LOG_COLOR_INFO,
    LOG_COLOR_WARN, LOG_COLOR_ERROR, LOG_COLOR_FATAL
}; // **[ADDED]** Array of colors for each log level

// Helper function to log a message to a specific target (e.g., file or console)
static void log_to_target(FILE* target, log_Event* ev) {
    char time_buf[20];
    time_t t = time(NULL);
    strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", localtime(&t));

    // If logging to the console, add color. If logging to a file, omit color.
    if (target == stderr) {
        fprintf(target, "%s%s %-5s %s:%d: ", level_colors[ev->level], time_buf, level_strings[ev->level], ev->file, ev->line);
        // **[CHANGED]**: Add color code before the log level and reset it after.
    }
    else {
        fprintf(target, "%s %-5s %s:%d: ", time_buf, level_strings[ev->level], ev->file, ev->line);
    }

    vfprintf(target, ev->fmt, ev->ap); // Write the formatted log message
    fprintf(target, LOG_COLOR_RESET "\n"); // **[ADDED]**: Reset the color after the message
    fflush(target);                    // Ensure the message is immediately written to the target
}

// Set the global minimum log level. Logs below this level will not be processed.
void log_set_level(int level) {
    L.level = level;
}

// Enable or disable console output. If disabled, logs only go to files.
void log_set_quiet(bool enable) {
    L.quiet = enable;
}

// Set a file pointer for logging to a file. Specify the minimum log level for this file.
void log_add_fp(FILE* fp, int level) {
    L.file_target.fp = fp;
    L.file_target.level = level;
}

// Core logging function, called by the log level macros. Handles logging to console and files.
void log_log(int level, const char* file, int line, const char* fmt, ...) {
    if (level < L.level) return; // Skip logs below the current global log level

    log_Event ev = { .fmt = fmt, .file = file, .line = line, .level = level };

    va_start(ev.ap, fmt);
    // If console output is not suppressed and the log level is high enough, log to console
    if (!L.quiet) {
        log_to_target(stderr, &ev);
    }
    // If a file target is set and the log level is high enough, log to the file
    if (L.file_target.fp && level >= L.file_target.level) {
        log_to_target(L.file_target.fp, &ev);
    }
    va_end(ev.ap);
}
#include "log.h"

// ANSI color codes for log levels
#define LOG_COLOR_TRACE "\x1b[94m"  // Light Blue
#define LOG_COLOR_DEBUG "\x1b[36m"  // Cyan
#define LOG_COLOR_INFO  "\x1b[32m"  // Green
#define LOG_COLOR_WARN  "\x1b[33m"  // Yellow
#define LOG_COLOR_ERROR "\x1b[31m"  // Red
#define LOG_COLOR_FATAL "\x1b[35m"  // Magenta
#define LOG_COLOR_RESET "\x1b[0m"   // Reset color to default

// Struct to hold information about a logging target (e.g., file).
typedef struct {
    FILE* fp;  // File pointer for logging to a file
    int level; // Minimum log level for this file
} LogTarget;

// Struct to hold the global logger state
static struct {
    int level;           // Minimum log level for all logs
    bool quiet;          // If true, suppress console output
    LogTarget file_target; // File logging target
} L = { LOG_TRACE, false, {NULL, LOG_INFO} };

typedef struct {
    va_list ap;           // Variable argument list for formatting the message
    const char* fmt;      // Format string for the log message
    const char* file;     // File name where the log originated
    int line;             // Line number in the file where the log originated
    int level;            // Log level (e.g., TRACE, DEBUG, INFO)
} log_Event;

// Array of strings representing the log levels
static const char* level_strings[] = {
    "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"
};

// Array of color codes for log levels
static const char* level_colors[] = {
    LOG_COLOR_TRACE, LOG_COLOR_DEBUG, LOG_COLOR_INFO,
    LOG_COLOR_WARN, LOG_COLOR_ERROR, LOG_COLOR_FATAL
}; // **[ADDED]** Array of colors for each log level

// Helper function to log a message to a specific target (e.g., file or console)
static void log_to_target(FILE* target, log_Event* ev) {
    char time_buf[20];
    time_t t = time(NULL);
    strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", localtime(&t));

    // If logging to the console, add color. If logging to a file, omit color.
    if (target == stderr) {
        fprintf(target, "%s%s %-5s %s:%d: ", level_colors[ev->level], time_buf, level_strings[ev->level], ev->file, ev->line);
        // **[CHANGED]**: Add color code before the log level and reset it after.
    }
    else {
        fprintf(target, "%s %-5s %s:%d: ", time_buf, level_strings[ev->level], ev->file, ev->line);
    }

    vfprintf(target, ev->fmt, ev->ap); // Write the formatted log message
    fprintf(target, LOG_COLOR_RESET "\n"); // **[ADDED]**: Reset the color after the message
    fflush(target);                    // Ensure the message is immediately written to the target
}

// Set the global minimum log level. Logs below this level will not be processed.
void log_set_level(int level) {
    L.level = level;
}

// Enable or disable console output. If disabled, logs only go to files.
void log_set_quiet(bool enable) {
    L.quiet = enable;
}

// Set a file pointer for logging to a file. Specify the minimum log level for this file.
void log_add_fp(FILE* fp, int level) {
    L.file_target.fp = fp;
    L.file_target.level = level;
}

// Core logging function, called by the log level macros. Handles logging to console and files.
void log_log(int level, const char* file, int line, const char* fmt, ...) {
    if (level < L.level) return; // Skip logs below the current global log level

    log_Event ev = { .fmt = fmt, .file = file, .line = line, .level = level };

    va_start(ev.ap, fmt);
    // If console output is not suppressed and the log level is high enough, log to console
    if (!L.quiet) {
        log_to_target(stderr, &ev);
    }
    // If a file target is set and the log level is high enough, log to the file
    if (L.file_target.fp && level >= L.file_target.level) {
        log_to_target(L.file_target.fp, &ev);
    }
    va_end(ev.ap);
}
