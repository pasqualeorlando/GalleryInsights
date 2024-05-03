#ifndef EXHIBITION_REPOSITORY_H
#define EXHIBITION_REPOSITORY_H

#include <mysql/mysql.h>
#include "../model.h"

/**
 * Send the query to get exhibitions from the database
 * 
 * @param connection connection to the database
 * @param *len length of the array returned
 * 
 * @return array of exhibition_t
*/
exhibition_t* get_exhibitions_from_db(MYSQL* connection, int *len);

#endif