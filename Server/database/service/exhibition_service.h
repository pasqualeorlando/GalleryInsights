#ifndef EXHIBITION_SERVICE_H
#define EXHIBITION_SERVICE_H

#include <mysql/mysql.h>
#include "../model.h"

#define MSG_EXHIBITION_LENGTH 68000

/**
 * Send exhibitions messages to socket descriptor
 * 
 * @param connection connection to the database
 * @param sd socket descriptor of the client
*/
void get_exhibitions(MYSQL* connection, int sd);
#endif