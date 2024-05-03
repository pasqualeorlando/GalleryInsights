#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include "exhibition_repository.h"
#include "../model.h"

exhibition_t* get_exhibitions_from_db(MYSQL* connection, int *len) {
    exhibition_t *exhibitions = NULL;
    MYSQL_RES *result = NULL;
    MYSQL_ROW row;
    int i = 0;

    if(mysql_query(connection, "SELECT * FROM exhibitions") == 0) {
        
        //store result from the query
        result = mysql_store_result(connection);

        if(result != NULL) {
            //get number of rows of the result
            *len = mysql_affected_rows(connection);

            //allocate array dynamically
            exhibitions = (exhibition_t*)calloc(*len, sizeof(exhibition_t));

            //for each row in the resultset copy its data into the array
            while((row = mysql_fetch_row(result))) {
                exhibitions[i].exhibition_id = atoi(row[0]);
                strcpy(exhibitions[i].name, row[1]);
                strcpy(exhibitions[i].description, row[2]);
                i++;
            }
        } else {
            *len = -1;
        }

        //free result
        mysql_free_result(result);
    } else {
        *len = -1;
    }

    return exhibitions;
}