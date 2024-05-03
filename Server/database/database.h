#ifndef DB_H
#define DB_H

#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include "../utils/log.h"

#define DB_NAME "GalleryInsights"

/**
 * Data structure to simplify tables creation
 * 
 * @param name name of the table
 * @param definition definition of the table
*/
typedef struct struct_tables_t {
    char name[50];
    char definition[2048];
} tables_t;

/**
 * Check if the database (identified with DB_NAME constant) exists
 * 
 * @param connection connection to MYSQL server to use
 * @return TRUE if exists; FALSE otherwise
 */
bool check_database_exists(MYSQL* connection);

/**
 * Check if all the tables exist
 * 
 * @param connection connection to MYSQL server to use
 * @return TRUE if exist; FALSE otherwise
 */
bool check_tables_exist(MYSQL* connection);

/**
 * Check if a single table exists in the database
 * 
 * @param connection connection to MYSQL server to use
 * @param table_name string representing the table name
 * @return TRUE if exists; FALSE otherwise
 */
bool check_table_exists(MYSQL* connection, const char* table_name);

/**
 * Create the entire database, including tables
 * 
 * @param connection connection to MYSQL server to use
 * @return TRUE if database is created successfully; FALSE otherwise
 */
bool create_entire_database(MYSQL* connection);

/**
 * Connect to the database. Check if database/tables exist and call other functions
 * 
 * @param flag_insert_mock TRUE if the user wants to insert sample data into the database; FALSE otherwise
 * @return a structure representing the handler for one database connection
 */
MYSQL* connect_to_database(bool flag_insert_mock);

/**
 * Calculate how many tables there are in the database
 * 
 * @return number of tables
*/
int get_number_of_tables(void);

/**
 * Initialize database by executing some insertions
 * 
 * @param connection connection to database
 * @return TRUE if queries are executed correctly; FALSE otherwise
*/
bool init_database(MYSQL* connection);

/**
 * Insert sample data into the database
 * 
 * @param connection connection to database
 * @return TRUE if data is inserted correctly; FALSE otherwise
*/
bool insert_mock(MYSQL* connection);

#endif