CC = gcc

CFLAGS = -Wall -Wextra -Werror -pedantic -std=c17 -g

SRV_SRCS = tests/server.c
CLI_SRCS = tests/client.c
MAINPAGE_SRCS = src/boundary/mainpage.c

SRV_OBJS = $(SRV_SRCS:.c=.o)
CLI_OBJS = $(CLI_SRCS:.c=.o)
MAINPAGE_OBJS = $(MAINPAGE_SRCS:.c=.o) 

SRV_EXEC = ./bin/server
CLI_EXEC = ./bin/client
MAIN_EXEC = ./bin/main
MAINPAGE_EXEC = ./bin/mainpage

.PHONY: all clean fclean re

all: $(SRV_EXEC) $(CLI_EXEC)

$(SRV_EXEC): $(SRV_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

$(CLI_EXEC): $(CLI_OBJS) $(MSG_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

$(MAINPAGE_EXEC): $(MAINPAGE_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(SRV_OBJS) $(CLI_OBJS) $(MAINPAGE_OBJS)