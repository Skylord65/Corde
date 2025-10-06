#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "message.h"

/** 
 * \file message.c
 * \brief Gestion des messages \see message.h
 * \author Skylord65
 * \date 03/05/2025
 **/


#define LG_MAX 1000

void fill_message(Message_t *Message, char *message){
    Message->lg_message = strlen(message);
    if (Message->lg_message > LG_MAX) {
        fprintf(stderr, "Erreur : le message dépasse la longueur maximale.\n");
        exit(EXIT_FAILURE);
    }
    strcpy(Message->Message, message);
}

int send_message(Message_t *Message, int socket){
    int len = htonl(Message->lg_message);
    if (send(socket, &len, sizeof(len), 0) == -1) {
        perror("Erreur lors de l'envoi de la longueur du message.");
        return -1;
    }
    if (send(socket, Message->Message, Message->lg_message, 0) == -1) {
        perror("Erreur lors de l'envoi du message.");
        return -1;
    }
    return 0;
}

int receive_message(Message_t *Message, int socket){
    int len;
    ssize_t n = recv(socket, &len, sizeof(len), MSG_WAITALL);
    if (n <= 0) return -1;
    Message->lg_message = ntohl(len);
    if (Message->lg_message > LG_MAX) return -1;
    n = recv(socket, Message->Message, Message->lg_message, MSG_WAITALL);
    if (n <= 0) return -1;
    Message->Message[Message->lg_message] = '\0';
    return 0;
}

void print_message(Message_t Message){
    printf("Message : %s\n", Message.Message);
    printf("Longueur du message : %d\n", Message.lg_message);
}