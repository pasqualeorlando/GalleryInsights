#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <signal.h>
#include <mysql/mysql.h>

#include "utils/log.h"
#include "utils/utils.h"
#include "database/database.h"
#include "database/service/exhibition_service.h"
#include "database/service/piece_service.h"
#include "database/service/user_service.h"

#define PORT 8888
#define MAXCLIENTS 200

static int master_socket;
logger_t logger;	//to use it in other .c files with extern keyword


void sigHandler(int sig) {
	char choice;

	clearBuffer();
	printf(COLOR_RESET "Do you want to kill the server? Y/N: ");
	scanf("%c", &choice);
	
	if(choice == 'Y' || choice == 'y') {
		close(master_socket);
		logger_log(&logger, INFO, __FILE__, __LINE__, "Server shutted down");
		exit(EXIT_SUCCESS);
	}
}

void handle_request(MYSQL* connection, int sd, char* message) {
	if(message == NULL || strlen(message) == 0)
		return;

	char* token = strtok(message, " ");

	if(strcmp(token, "get_exhibitions") == 0) {
		get_exhibitions(connection, sd);
	} else if(strcmp(token, "get_pieces") == 0) {
		token = strtok(NULL, " ");
		get_pieces(connection, sd, token);
	} else if(strcmp(token, "login") == 0) {
		token = strtok(NULL, "");
		login(connection, sd, token);
	} else if(strcmp(token, "register") == 0) {
		token = strtok(NULL, "");
		register_user(connection, sd, token);
	} else {
		logger_log(&logger, WARNING, __FILE__, __LINE__, "User requested a non-valid function");
	}
}
	
int main(int argc, char *argv[])
{
	int opt = true;
	int addrlen, new_socket, client_socket[MAXCLIENTS], activity, i, valread, sd;
	struct sockaddr_in address;
	bool log_to_file = false;
	bool insert_sample_data = false;
	MYSQL* db_connection = NULL;
	char buffer[BUFSIZE + 1];

	//set of socket descriptors
	fd_set readfds;

	//reading args if user wants to log on file or initialize db with mock information
	if(argc > 1) {
		for(int i = 1; i < argc; i++) {
			if(strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--log") == 0 || strcmp(argv[i], "--log=true") == 0)
				log_to_file = true;
			if(strcmp(argv[i], "--log=false") == 0)
				log_to_file = false;
			if(strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--insert") == 0 || strcmp(argv[i], "--insert=true") == 0)
				insert_sample_data = true;
			if(strcmp(argv[i], "--insert=false") == 0)
				insert_sample_data = false;
		}
	}

    //initializing logger, first checking if log directory exists
    if(log_to_file)
        check_and_create_log_dir();
    if(logger_init(&logger, DEBUG, log_to_file) == -1) {
    	logger_init(&logger, DEBUG, false);
    	
    	logger_log(&logger, ERROR, __FILE__, __LINE__, "Cannot log on file: logs will appear on console");
	}

	//init mysql library
	if(mysql_library_init(0, NULL, NULL)) {
		logger_log(&logger, ERROR, __FILE__, __LINE__, "Cannot initialize MySQL Client Library");
    	exit(EXIT_FAILURE);
 	}

	//connecting to database
	db_connection = connect_to_database(insert_sample_data);
	if(db_connection == NULL) {
		logger_log(&logger, ERROR, __FILE__, __LINE__, "Cannot connect to database");
		exit(EXIT_FAILURE);
	} else
		logger_log(&logger, INFO, __FILE__, __LINE__, "Connected to database");
	
	//register sigint handler
	if(signal(SIGINT, sigHandler) == SIG_ERR || signal(SIGUSR1, sigHandler) == SIG_ERR || signal(SIGUSR2, sigHandler) == SIG_ERR) {
  		logger_log(&logger, ERROR, __FILE__, __LINE__, "Cannot register signal handler: %s", strerror(errno));
  		exit(EXIT_FAILURE);
  	}
		
	//create a master socket
	if((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		logger_log(&logger, ERROR, __FILE__, __LINE__, "Socket creation failed: %s", strerror(errno));
		exit(EXIT_FAILURE);
	}
	
	//set master socket to allow multiple connections ,
	if(setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0) {
		logger_log(&logger, ERROR, __FILE__, __LINE__, "Cannot set socket options: %s", strerror(errno));
		exit(EXIT_FAILURE);
	}
	
	//type of socket created
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	//initialize all client_socket[] to -1 so not checked and set first entry to master socket
	for(i = 0; i < MAXCLIENTS; i++) {
		client_socket[i] = -1;
	}
	client_socket[0] = master_socket;
		
	//bind the socket to localhost port 8888
	if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0) {
		logger_log(&logger, ERROR, __FILE__, __LINE__, "Bind failed: %s", strerror(errno));
		exit(EXIT_FAILURE);
	}
	
	logger_log(&logger, INFO, __FILE__, __LINE__, "Listening on port %d", PORT);

	//try to specify maximum of 200 pending connections for the master socket
	if (listen(master_socket, MAXCLIENTS) < 0) {
		logger_log(&logger, ERROR, __FILE__, __LINE__, "Cannot listen: %s", strerror(errno));
		exit(EXIT_FAILURE);
	}	
	//accept the incoming connection
	addrlen = sizeof(address);
	logger_log(&logger, INFO, __FILE__, __LINE__, "Waiting for connections...");

	while(true) {
		//clear the socket set
		FD_ZERO(&readfds);
			
		//add child sockets to set
		for (i = 0 ; i < MAXCLIENTS ; i++) {
			//socket descriptor
			sd = client_socket[i];
				
			//if valid socket descriptor then add to read list
			if(sd >= 0)
				FD_SET(sd , &readfds);
		}
	
		//wait for an activity on one of the sockets, timeout is NULL, so wait indefinitely
		activity = select(FD_SETSIZE , &readfds , NULL , NULL , NULL);
	
		if ((activity < 0) && (errno!=EINTR)) {
			logger_log(&logger, WARNING, __FILE__, __LINE__, "Select error");
			continue;
		}
		
		//If something happened on the master socket, then it's an incoming connection
		if (FD_ISSET(master_socket, &readfds)) {
			if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
				logger_log(&logger, ERROR, __FILE__, __LINE__, "Error accepting new connection: %s", strerror(errno));
				exit(EXIT_FAILURE);
			}
			
			//inform user of socket number - used in send and receive commands
			logger_log(&logger, INFO, __FILE__, __LINE__, "New connection, socket fd is %d, ip is: %s:%d", new_socket, inet_ntoa(address.sin_addr), ntohs(address.sin_port));
				
			//add new socket to array of sockets
			for (i = 0; i < MAXCLIENTS; i++) {
				//if position is empty
				if(client_socket[i] < 0) {
					client_socket[i] = new_socket;
					break;
				}
			}
		}
			
		//else its some IO operation on some other socket
		for (i = 1; i < MAXCLIENTS; i++) {
			sd = client_socket[i];
				
			if (sd > 0 && FD_ISSET(sd, &readfds)) {

				//Check if it was for closing, and also read the incoming message
				if ((valread = read(sd, buffer, BUFSIZE)) == 0) {

					//Somebody disconnected, get his details and print
					getpeername(sd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
					logger_log(&logger, INFO, __FILE__, __LINE__, "Host %s:%d disconnected", inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

					//Close the socket and mark as -1 in list for reuse
					close(sd);
					client_socket[i] = -1;

				}
					
				//handle client request
				else {
					//set the string terminating NULL byte on the end of the data read
					buffer[valread+1] = '\0';

					//handle client request
					handle_request(db_connection, sd, buffer);
				}
			}
			//clear read buffer
			memset(buffer, 0, BUFSIZE + 1);
		}
	}

	for (i = 0; i < MAXCLIENTS; i++) {
        if (client_socket[i] > 0)
             close(client_socket[i]);
    }
	mysql_library_end();
	return 0;
}