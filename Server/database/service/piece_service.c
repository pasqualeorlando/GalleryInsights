#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <mysql/mysql.h>
#include "piece_service.h"
#include "../repository/piece_repository.h"
#include "../model.h"
#include "../../utils/utils.h"
#include "../../utils/log.h"

extern logger_t logger;

void get_pieces(MYSQL* connection, int sd, char* message_from_client) {
    //data for sending messages
    int lines = 0;
    piece_t *pieces = NULL;

    enum_expertise_t exp;
    enum_user_type_t usr_type;

    //data to extract to execute the query
    int exhibition_id;
    char* user_type = NULL;
    char* expertise = NULL;

    char message[MSG_PIECE_LENGTH + 1]; //defined in piece_service.h

    //send start message
    if(!send(sd, "START\r", 6 * sizeof(char), 0))
        return;

    //extract exhibition id from message
    exhibition_id = strtol(message_from_client, &message_from_client, 10);
    message_from_client++; //because strtol doesn't delete space between exhibition_id and usertype:expertise string

    //extract user type and expertise from string
    user_type = strtok(message_from_client, ":");
    expertise = strtok(NULL, ":");

    //check if client sent empty exhibition id or empty user_type and expertise
    if(user_type == NULL || strlen(user_type) == 0 || expertise == NULL || strlen(expertise) == 0 || exhibition_id == 0) {
        send(sd, "usage: get_pieces <exhibition id> <user_type>:<expertise>\r", 57 * sizeof(char), 0);
        return;
    }

    //get user_type_id and expertise_level_id from strings
    for(int i = 0; i < user_type_string_array_len; i++)
        if(strcmp(user_type, user_type_string[i]) == 0) {
            usr_type = i;
            break;
        }
    
    for(int i = 0; i < expertise_string_array_len; i++)
        if(strcmp(expertise, expertise_string[i]) == 0) {
            exp = i;
            break;
        }

    //get pieces from database
    pieces = get_pieces_from_db(connection, &lines, exhibition_id, usr_type, exp);

    //send pieces to client
    for(int i = 0; i < lines; i++) {
        memset(message, 0, MSG_PIECE_LENGTH + 1);
        sprintf(message, "%d:%s:%s:%s:%s:%s\r", pieces[i].piece_id, pieces[i].name, pieces[i].author, pieces[i].year, pieces[i].img_link, pieces[i].description);
        
        send(sd, message, sizeof(message), 0);
    }

    logger_log(&logger, INFO, __FILE__, __LINE__, "Sent pieces to user");

    //send end message
    send(sd, "END\r", 4 * sizeof(char), 0);
    free(pieces);
}