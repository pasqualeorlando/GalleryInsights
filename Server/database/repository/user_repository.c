#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <mysql/mysql.h>
#include "user_repository.h"
#include "../../utils/utils.h"
#include "../model.h"

user_t* check_credentials(MYSQL* connection, char* email, char* password) {
    MYSQL_RES *result = NULL;
    MYSQL_ROW row;
    user_t* to_return = NULL;
    char query[600];

    sprintf(query, "SELECT * FROM users WHERE email='%s' AND password='%s'", email, password);

    if(mysql_query(connection, query) == 0) {
        
        //store result from the query
        result = mysql_store_result(connection);

        if(result != NULL) {

            //if there's 1 row then credentials are correct
            if(mysql_affected_rows(connection) == 1) {
                to_return = (user_t*)malloc(sizeof(user_t));

                row = mysql_fetch_row(result);

                to_return->user_id = atoi(row[0]);
                strcpy(to_return->name, row[1]);
                strcpy(to_return->email, row[2]);
                strcpy(to_return->password, row[3]);
            }
        }

        //free result
        mysql_free_result(result);
    }

    return to_return;
}

bool check_email_already_registered(MYSQL* connection, char* email) {
    MYSQL_RES *result = NULL;
    bool to_return;
    char query[300];

    sprintf(query, "SELECT * FROM users WHERE email='%s'", email);

    if(mysql_query(connection, query) == 0) {
        
        //store result from the query
        result = mysql_store_result(connection);

        if(result != NULL) {

            //if there's 1 row then email is registered
            to_return = mysql_affected_rows(connection) == 1;

        } else {
            to_return = false;;
        }

        //free result
        mysql_free_result(result);
    } else {
        to_return = false;
    }

    return to_return;
}

bool register_user_to_db(MYSQL* connection, char* name, char* email, char* password) {
    char query[BUFSIZE];

    sprintf(query, "INSERT INTO users(name, email, password) VALUES ('%s', '%s', '%s');", name, email, password);

    if(mysql_query(connection, query) == 0) {

        //if there's 1 row then user is correctly registered
        return mysql_affected_rows(connection) == 1;
    } else {
        return false;
    }
}