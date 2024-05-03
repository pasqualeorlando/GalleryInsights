#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "log.h"

char* get_color_from_log_level(logger_level_t const level) {
	//Colors respectively for [OFF, DEBUG, INFO, WARNING, ERROR] levels
	char *colors[] = {COLOR_RESET, WHT, CYN, YEL, RED};
	
	return colors[level];
}

char* get_string_from_log_level(logger_level_t const level) {
	char *strings[] = {"OFF", "DEBUG", "INFO", "WARNING", "ERROR"};
	
	return strings[level];
}

FILE* get_file(void) {
	char filename[20] = {0};	//name of the log file to open
	FILE* to_return = NULL;

	//get the current datetime
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	char convertedYear[5] = {0}, convertedMonth[3] = {0}, convertedDay[3] = {0};
	
	//convert year, month and day string in integer and create the filename
	if(sprintf(convertedYear, "%d", tm.tm_year + 1900) > 0 && sprintf(convertedMonth, "%02d", tm.tm_mon + 1) > 0 && sprintf(convertedDay, "%02d", tm.tm_mday) > 0) {
		strcat(filename, "./logs/");
		strcat(filename, convertedYear);
		strcat(filename, convertedMonth);
		strcat(filename, convertedDay);
		strcat(filename, ".log");

		//open the file in append. creates it if it doesn't exist 
		to_return = fopen(filename, "a+");
		return to_return;
	} else {
		return NULL;
	}
	
}

int logger_init(logger_t * const logger, logger_level_t const level, bool const write_to_file) {
	//set the logger level
    logger->level = level;

	//if the user wants to log on file then get the file, else log on stderr
    if(write_to_file) {
        logger->fp = get_file();
        if (logger->fp == NULL) {
            return -1;
        }
    } else {
        logger->fp = stderr;
    }
    return 0;
}

void logger_log(logger_t const * const logger, logger_level_t const level, char const * const file, size_t const line, char const * const format, ...) {
	//get the current datetime
    time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	
	//if the level we want to log is less than the minimum level then exit
	if (level < logger->level)
        return;
    
	//if the file is stderr then we can print log colors
    if(logger->fp == stderr)
    	fprintf(logger->fp, "%s", get_color_from_log_level(level));

	//print date, file, line and string level. Format is YYYY/MM/DD HH:mm:ss file.c: line [LEVEL] 
    fprintf(logger->fp, "%d/%02d/%02d %02d:%02d:%02d %s: %lu: [%s] ", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, file, line, get_string_from_log_level(level));

	//get the variadic args and print them
    va_list argp;
    va_start(argp, format);
    vfprintf(logger->fp, format, argp);
    va_end(argp);

	//if log color has been set, then reset it and print \n
    if(logger->fp == stderr)
    	fprintf(logger->fp, "%s\n", COLOR_RESET);
    else
    	fprintf(logger->fp, "\n");
    	
    fflush(logger->fp);
}

void check_and_create_log_dir() {
	struct stat st = {0};

	//if logs directory doesn't exist create it with 755 permission
	if(stat("./logs", &st) == -1) {
		mkdir("./logs", 0755);
	}
}