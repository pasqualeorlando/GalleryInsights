# base image
FROM gcc:latest
# copy source files
COPY . /server
# enter in the copied directory
WORKDIR /server/
# compile c program
RUN gcc -o server $(mysql_config --cflags) server.c database/database.c database/repository/user_repository.c database/repository/piece_repository.c database/repository/exhibition_repository.c database/service/user_service.c database/service/piece_service.c database/service/exhibition_service.c utils/utils.c utils/log.c $(mysql_config --libs)
# execute the server
CMD ["./server", "-i"]