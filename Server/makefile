# Compiler
CC = gcc

# Compiler flags
CFLAGS = $(shell mysql_config --cflags)

# Linker flags
LDFLAGS = $(shell mysql_config --libs)

# Target executable name
TARGET = server

# Source files
SRCS = server.c utils/log.c utils/utils.c database/database.c database/service/exhibition_service.c database/service/piece_service.c database/service/user_service.c database/repository/exhibition_repository.c database/repository/piece_repository.c database/repository/user_repository.c

# Object files (automatically generated from source files)
OBJS = $(SRCS:.c=.o)

# Phony targets (non-file targets)
.PHONY: depend clean

# Default target
all: $(TARGET)

# Linking rule
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

# Compilation rule
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(TARGET) $(OBJS)
	
# Start rule
start:
	@printf "\e[32m%b\e[0m" "Starting server...\n"
	./server
	
# Start init rule
start_init:
	@printf "\e[32m%b\e[0m" "Starting server and initialization...\n"
	./server -i

# Valgrind rule
valgrind:
	@printf "\e[32m%b\e[0m" "Valgrind memory check...\n"
	valgrind --leak-check=full \
		--show-leak-kinds=all \
		--track-origins=yes \
		--verbose \
		--log-file=valgrind-out.txt \
		./server