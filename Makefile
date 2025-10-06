CC = gcc

CFLAGS = -Wall -Wextra -Werror -pedantic -std=c17 -g

SRV_SRCS = src/controler/server.c
CLI_SRCS = src/controler/client.c
MSG_SRCS = src/controler/message.c

SRV_OBJS = $(SRV_SRCS:.c=.o)
CLI_OBJS = $(CLI_SRCS:.c=.o)
MSG_OBJS = $(MSG_SRCS:.c=.o)

SRV_EXEC = server
CLI_EXEC = client
MSG_EXEC = message

.PHONY: all clean fclean re

all: $(SRV_EXEC) $(CLI_EXEC) $(MSG_EXEC)

$(SRV_EXEC): $(SRV_OBJS) $(MSG_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

$(CLI_EXEC): $(CLI_OBJS) $(MSG_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

$(MSG_EXEC): $(MSG_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(SRV_OBJS) $(CLI_OBJS) $(MSG_OBJS)