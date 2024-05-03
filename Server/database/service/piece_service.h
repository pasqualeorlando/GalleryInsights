#ifndef PIECE_SERVICE_H
#define PIECE_SERVICE_H

#include <mysql/mysql.h>
#include "../model.h"

#define MSG_PIECE_LENGTH 68000

/**
 * Send pieces messages to socket descriptor. This function DESTROYS message_from_client parameter
 * 
 * @param connection connection to the database
 * @param sd socket descriptor of the client
 * @param message_from_client string that contains exhibition id from which obtain pieces and usertype:expertise to customize pieces descriptions
*/
void get_pieces(MYSQL* connection, int sd, char* message_from_client);
#endif