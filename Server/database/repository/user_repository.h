#ifndef USER_REPOSITORY_H
#define USER_REPOSITORY_H

#include <mysql/mysql.h>
#include <stdbool.h>
#include "../model.h"

/**
 * Send the query to check if credentials are correct
 * 
 * @param connection connection to the database
 * @param email email of the user
 * @param password password of the user
 * 
 * @return pointer to logged user if credentials are correct; NULL otherwise
*/
user_t* check_credentials(MYSQL* connection, char* email, char* password);

/**
 * Check if a specific email is registered
 * 
 * @param connection connection to the database
 * @param email email to check for
 * 
 * @return TRUE if email is already registered; FALSE otherwise
*/
bool check_email_already_registered(MYSQL* connection, char* email);

/**
 * Insert user into database
 * 
 * @param connection connection to the database
 * @param name name of the user
 * @param email email of the user
 * @param password password of the user
 * 
 * @return TRUE if the user is correctly registered; FALSE otherwise
*/
bool register_user_to_db(MYSQL* connection, char* name, char* email, char* password);
#endif