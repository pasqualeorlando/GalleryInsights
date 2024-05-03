#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <mysql/mysql.h>
#include "exhibition_service.h"
#include "../repository/exhibition_repository.h"
#include "../model.h"
#include "../../utils/utils.h"
#include "../../utils/log.h"

extern logger_t logger;

void get_exhibitions(MYSQL* connection, int sd) {
    int lines;
    exhibition_t *exhibitions = NULL;

    char message[MSG_EXHIBITION_LENGTH + 1]; //MSG_EXHIBITION_LENGTH declared in exhibition_service.h

    //send start message
    if(!send(sd, "START\r", 6 * sizeof(char), 0))
        return;

    //get exhibitions from database
    exhibitions = get_exhibitions_from_db(connection, &lines);

    //send exhibitions to client
    for(int i = 0; i < lines; i++) {
        memset(message, 0, BUFSIZE + 1);
        sprintf(message, "%d:%s:%s\r", exhibitions[i].exhibition_id, exhibitions[i].name, exhibitions[i].description);
        
        send(sd, message, strlen(message)+1, 0);
    }

    logger_log(&logger, INFO, __FILE__, __LINE__, "Sent exhibitions to user");

    //send end message
    send(sd, "END\r", 4 * sizeof(char), 0);
    free(exhibitions);
}