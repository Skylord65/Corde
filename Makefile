CC = gcc

CFLAGS = -Wall -Wextra -Werror -pedantic -std=c17 -g

SRV_SRCS = src/controler/server.c
CLI_SRCS = src/controler/client.c
MAINPAGE_SRCS = src/boundary/mainpage.c
MAIN_SRCS = src/controler/main.c
MSG_SRCS = src/controler/message.c

SRV_OBJS = $(SRV_SRCS:.c=.o)
CLI_OBJS = $(CLI_SRCS:.c=.o)
MAIN_OBJS = $(MAIN_SRCS:.c=.o)
MAINPAGE_OBJS = $(MAINPAGE_SRCS:.c=.o) 
MSG_OBJS = $(MSG_SRCS:.c=.o)

SRV_EXEC = ./bin/server
CLI_EXEC = ./bin/client
MAIN_EXEC = ./bin/main
MAINPAGE_EXEC = ./bin/mainpage

.PHONY: all clean fclean re

all: $(SRV_EXEC) $(CLI_EXEC) $(MSG_EXEC)

$(SRV_EXEC): $(SRV_OBJS) $(MSG_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

$(CLI_EXEC): $(CLI_OBJS) $(MSG_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

$(MAINPAGE_EXEC): $(MAINPAGE_OBJS) $(MSG_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

$(MAIN_EXEC): $(MAIN_OBJS) $(MSG_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(SRV_OBJS) $(CLI_OBJS) $(MAIN_OBJS) $(MAINPAGE_OBJS) $(MSG_OBJS)