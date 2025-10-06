#include <stdio.h>
#include <stdlib.h>
#include "message.h"
#include <string.h>
#include <sys/socket.h>

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

void send_message(Message_t *Message, int socket){

    // Envoi de la longueur du message
    if (send(socket, Message, sizeof(Message_t), 0) == -1) {
        perror("Erreur lors de l'envoi de la longueur du message.");
        exit(EXIT_FAILURE);
    }
}

void receive_message(Message_t *Message, int socket){
    // Réception de la longueur du message
    if (recv(socket, Message, sizeof(Message_t), 0) == -1) {
        perror("Erreur lors de la réception de la longueur du message.");
        exit(EXIT_FAILURE);
    }
}

void print_message(Message_t Message){
    printf("Message : %s\n", Message.Message);
    printf("Longueur du message : %d\n", Message.lg_message);
}