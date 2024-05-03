#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <mysql/mysql.h>
#include "user_service.h"
#include "../repository/user_repository.h"
#include "../model.h"
#include "../../utils/utils.h"
#include "../../utils/log.h"

extern logger_t logger;

void login(MYSQL* connection, int sd, char* message_from_client) {
    //data to extract to execute the query
    char* email = NULL;
    char* password = NULL;

    //extract user email and password from string
    email = strtok(message_from_client, ":");
    password = strtok(NULL, ":");

    //check for empty fields
    if(email == NULL || strlen(email) == 0 || password == NULL || strlen(password) == 0) {
        send(sd, "usage: login <email>:<password>\r", 31 * sizeof(char), 0);
        return;
    }

    //check for credentials
    user_t* user = check_credentials(connection, email, password);

    //send message to client based on correct_credentials
    if(user != NULL) {
        //user can log
        char message[BUFSIZE];
        sprintf(message, "%d:%s:%s:%s\r", user->user_id, user->name, user->email, user->password);
        send(sd, message, strlen(message)+1, 0);
        logger_log(&logger, INFO, __FILE__, __LINE__, "User %s logged", email);
    } else {
        send(sd, "bad credentials\r",  16 * sizeof(char), 0);
        logger_log(&logger, WARNING, __FILE__, __LINE__, "User tried login with bad credentials");
    }

    free(user);
}

void register_user(MYSQL* connection, int sd, char* message_from_client) {
    //data to extract to execute the query
    char* name = NULL;
    char* email = NULL;
    char* password = NULL;

    //extract user name, email and password from string
    name = strtok(message_from_client, ":");
    email = strtok(NULL, ":");
    password = strtok(NULL, ":");

    //check for empty fields
    if(name == NULL || strlen(name) == 0 || email == NULL || strlen(email) == 0 || password == NULL || strlen(password) == 0) {
        send(sd, "usage: register <name>:<email>:<password>\r", 42 * sizeof(char), 0);
        return;
    }

    //check if user already exists
    if(check_email_already_registered(connection, email)) {
        send(sd, "email already registered\r", 25 * sizeof(char), 0);
        logger_log(&logger, WARNING, __FILE__, __LINE__, "User tried to register with an already registered email");
    } else {
        bool registered = register_user_to_db(connection, name, email, password);
        
        if(registered) {
            send(sd, "user registered\r", 16 * sizeof(char), 0);
            logger_log(&logger, INFO, __FILE__, __LINE__, "User %s registered", email);
        } else {
            send(sd, "cannot register user\r", 21 * sizeof(char), 0);
            logger_log(&logger, ERROR, __FILE__, __LINE__, "Cannot register user with email %s", email);
        }
    }
}