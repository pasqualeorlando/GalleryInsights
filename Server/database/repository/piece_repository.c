#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include "piece_repository.h"
#include "../model.h"

piece_t* get_pieces_from_db(MYSQL* connection, int *len, int exhibition_id, enum_user_type_t user_type_id, enum_expertise_t expertise_id) {
    piece_t *pieces = NULL;
    MYSQL_RES *result = NULL;
    MYSQL_ROW row;
    int i = 0;

    char query[2000];

    expertise_id++;
    user_type_id++;

    sprintf(query, "SELECT DISTINCT p.piece_id, p.name AS piece_name, p.author, p.year, p.img_link, COALESCE(pd.description, pd_fallback.description, pd_default.description, 'Descrizione non disponibile') AS description "
                    "FROM pieces p "
                    "INNER JOIN exhibitions_pieces ep ON p.piece_id = ep.piece_id "
                    "LEFT JOIN ( "
                        "SELECT pd.piece_id, pd.description, pd.expertise_level_id, pd.user_type_id, "
                            "IF(pd.expertise_level_id = %d, 1, 0) AS expertise_match "
                        "FROM pieces_descriptions pd "
                        "WHERE pd.expertise_level_id = %d AND pd.user_type_id = %d "
                    ") pd ON p.piece_id = pd.piece_id "
                    "LEFT JOIN ( "
                        "SELECT pd_f.piece_id, pd_f.description, pd_f.expertise_level_id, pd_f.user_type_id "
                        "FROM pieces_descriptions pd_f "
                        "WHERE pd_f.expertise_level_id < %d AND pd_f.user_type_id = %d "
                        "AND NOT EXISTS ( "
                            "SELECT 1 FROM pieces_descriptions pd_check "
                            "WHERE pd_check.expertise_level_id = %d AND pd_check.user_type_id = %d AND pd_check.piece_id = pd_f.piece_id "
                        ") "
                        "ORDER BY pd_f.expertise_level_id DESC "
                        "LIMIT 1 "
                    ") pd_fallback ON p.piece_id = pd_fallback.piece_id "
                    "LEFT JOIN ( "
                        "SELECT piece_id, description "
                        "FROM pieces_descriptions "
                        "WHERE expertise_level_id = ( "
                            "SELECT id FROM enum_expertise WHERE level = 'STANDARD' "
                        ") AND user_type_id = ( "
                            "SELECT id FROM enum_user_type WHERE name = 'SINGLE' "
                        ") "
                    ") pd_default ON p.piece_id = pd_default.piece_id "
                    "WHERE ep.exhibition_id = %d "
                    "ORDER BY p.piece_id;", expertise_id, expertise_id, user_type_id, expertise_id, user_type_id, expertise_id, user_type_id, exhibition_id);

    if(mysql_query(connection, query) == 0) {
        
        //store result from the query
        result = mysql_store_result(connection);

        if(result != NULL) {
            //get number of rows of the result
            *len = mysql_affected_rows(connection);

            //allocate array dynamically
            pieces = (piece_t*)calloc(*len, sizeof(piece_t));

            //for each row in the resultset copy its data into the array
            while((row = mysql_fetch_row(result))) {
                pieces[i].piece_id = atoi(row[0]);
                strcpy(pieces[i].name, row[1]);
                strcpy(pieces[i].author, row[2]);
                strcpy(pieces[i].year, row[3]);
                strcpy(pieces[i].img_link, row[4]);
                strcpy(pieces[i].description, row[5]);
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

    return pieces;
}