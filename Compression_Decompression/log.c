//
//#include "log.h"
//
//typedef struct {
//    FILE* file_p;  
//    int level; // Minimum log level
//} LogTarget;
//
//static struct {
//    int level;           // Minimum log level for all logs
//    bool quiet;          // If true, suppress console output
//    LogTarget file_target; // File logging target
//} L = { LOG_INFO, false, {NULL, LOG_INFO} };
//
//// Log levels
//static const char* level_strings[] = {
//    "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"
//};
//
//// Log a message to a specific target (file or console)
//static void log_to_target(FILE* target, log_Event* ev) {
//    char time_buf[20];
//    time_t t = time(NULL);
//    strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", localtime(&t));
//    // Write the log message to the target with timestamp, log level, file, and line number
//    fprintf(target, "%s %-5s %s:%d: ", time_buf, level_strings[ev->level], ev->file, ev->line);
//    vfprintf(target, ev->fmt, ev->ap); // Write the formatted log message
//    fprintf(target, "\n");             // End the log message with a newline
//    fflush(target);                    // Ensure the message is immediately written to the target
//}
//
//// Set the global minimum log level.
//void log_set_level(int level) {
//    L.level = level;
//}
//
//// Enable or disable console output. If disabled, logs only go to files.
//void log_set_quiet(bool enable) {
//    L.quiet = enable;
//}
//
//// Set a file pointer for logging to a file. Specify the minimum log level for this file.
//void log_add_fp(FILE* fp, int level) {
//    L.file_target.file_p = fp;
//    L.file_target.level = level;
//}
//
//// Core logging function, called by the log level macros. Handles logging to console and files.
//void log_log(int level, const char* file, int line, const char* fmt, ...) {
//    if (level < L.level) return; // Skip logs below the current global log level
//
//    log_Event ev = { .fmt = fmt, .file = file, .line = line, .level = level };
//
//    va_start(ev.ap, fmt);
//    // If console output is not suppressed and the log level is high enough, log to console
//    if (!L.quiet) {
//        log_to_target(stderr, &ev);
//    }
//    // If a file target is set and the log level is high enough, log to the file
//    if (L.file_target.file_p && level >= L.file_target.level) {
//        log_to_target(L.file_target.file_p, &ev);
//    }
//    va_end(ev.ap);
//}