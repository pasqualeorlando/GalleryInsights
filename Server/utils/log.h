#ifndef LOG_H
#define LOG_H

//Regular text
#define BLK "\e[0;30m"
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define YEL "\e[0;33m"
#define BLU "\e[0;34m"
#define MAG "\e[0;35m"
#define CYN "\e[0;36m"
#define WHT "\e[0;37m"

//Regular bold text
#define BBLK "\e[1;30m"
#define BRED "\e[1;31m"
#define BGRN "\e[1;32m"
#define BYEL "\e[1;33m"
#define BBLU "\e[1;34m"
#define BMAG "\e[1;35m"
#define BCYN "\e[1;36m"
#define BWHT "\e[1;37m"

//Regular underline text
#define UBLK "\e[4;30m"
#define URED "\e[4;31m"
#define UGRN "\e[4;32m"
#define UYEL "\e[4;33m"
#define UBLU "\e[4;34m"
#define UMAG "\e[4;35m"
#define UCYN "\e[4;36m"
#define UWHT "\e[4;37m"

//Regular background
#define BLKB "\e[40m"
#define REDB "\e[41m"
#define GRNB "\e[42m"
#define YELB "\e[43m"
#define BLUB "\e[44m"
#define MAGB "\e[45m"
#define CYNB "\e[46m"
#define WHTB "\e[47m"

//High intensty background 
#define BLKHB "\e[0;100m"
#define REDHB "\e[0;101m"
#define GRNHB "\e[0;102m"
#define YELHB "\e[0;103m"
#define BLUHB "\e[0;104m"
#define MAGHB "\e[0;105m"
#define CYNHB "\e[0;106m"
#define WHTHB "\e[0;107m"

//High intensty text
#define HBLK "\e[0;90m"
#define HRED "\e[0;91m"
#define HGRN "\e[0;92m"
#define HYEL "\e[0;93m"
#define HBLU "\e[0;94m"
#define HMAG "\e[0;95m"
#define HCYN "\e[0;96m"
#define HWHT "\e[0;97m"

//Bold high intensity text
#define BHBLK "\e[1;90m"
#define BHRED "\e[1;91m"
#define BHGRN "\e[1;92m"
#define BHYEL "\e[1;93m"
#define BHBLU "\e[1;94m"
#define BHMAG "\e[1;95m"
#define BHCYN "\e[1;96m"
#define BHWHT "\e[1;97m"

//Reset
#define COLOR_RESET "\e[0m"

#include <stdbool.h>

/**
 * Logger levels
*/
typedef enum {
    OFF,
    DEBUG,
    INFO,
    WARNING,
    ERROR
} logger_level_t;

/**
 * Type representing a logger
 * @param level minimum level to log
 * @param fp file pointer to write on. If logger is initialized with "write_to_file" false, its value is stderr
*/
typedef struct {
    logger_level_t level;
    FILE *fp;
} logger_t;

/**
 * Get the color of a specific log level
 * 
 * @param level level from which obtain the right color
 * @return unix color string
 */
char* get_color_from_log_level(logger_level_t const level);

/**
 * Get the string representing a level
 * 
 * @param level level from which obtain the string
 * @return string representing the level
 */
char* get_string_from_log_level(logger_level_t const level);

/**
 * Gets the log file to write on, with name indicating today's date
 * 
 * @return file pointer
 */
FILE* get_file(void);

/**
 * Initialize the logger with the minimum level and the file to write to
 * 
 * @param logger logger to initialize
 * @param level minimum level to log
 * @param write_to_file if the user wants to write logs on file. If false, file pointer is stderr
 * @return 0 for success or -1 for error (-1 only if it can't open the file)
 */
int logger_init(logger_t * const logger, logger_level_t const level, bool const write_to_file);

/**
 * Writes the C string pointed by format to the file specified in the logger
 * 
 * @param logger logger to be used
 * @param level log level of the message
 * @param file .c file where the function is called
 * @param line the exact line where the function is called
 * @param format C string that contains the text to be written to stdout. It can optionally contain embedded format specifiers that are replaced by the values specified in subsequent additional arguments and formatted as requested
 */
void logger_log(logger_t const * const logger, logger_level_t const level, char const * const file, size_t const line, char const * const format, ...);

/**
 * Checks if the logs directory is present. If not creates it
 * 
 */
void check_and_create_log_dir();

#endif