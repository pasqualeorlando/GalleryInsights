#ifndef EXHIBITION_REPOSITORY_H
#define EXHIBITION_REPOSITORY_H

#include <mysql/mysql.h>
#include "../model.h"

/**
 * Send the query to get pieces from the database. The query search for pieaces descriptions based on user type and expertise.
 * If the description is not found, the query retrieves the corresponding description with lower level of expertise. If not found then
 * retrieves standard description for single user.
 * 
 * @param connection connection to the database
 * @param *len length of the array returned
 * @param exhibition_id id of the exhibitions from which obtain pieces
 * @param user_type type of user to customize pieces descriptions
 * @param expertise expertise of user to customize pieces descriptions
 * 
 * @return array of piece_t
*/
piece_t* get_pieces_from_db(MYSQL* connection, int *len, int exhibition_id, enum_user_type_t user_type_id, enum_expertise_t expertise_id);

#endif