#ifndef USER_SERVICE_H
#define USER_SERVICE_H

#include <mysql/mysql.h>
#include "../model.h"

/**
 * Check if the user sent right credentials and send appropriate message on socket
 * 
 * @param connection connection to the database
 * @param sd socket descriptor of the client
 * @param message_from_client string that contains email:password of the user
*/
void login(MYSQL* connection, int sd, char* message_from_client);

/**
 * Register the user by inserting its data into database. Checks if user already exists
 * 
 * @param connection connection to the database
 * @param sd socket descriptor of the client
 * @param message_from_client string that contains name:email:password of the user
*/
void register_user(MYSQL* connection, int sd, char* message_from_client);
#endif