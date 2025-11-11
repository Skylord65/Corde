#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "message.h"

/** 
 * \file client.c
 * \brief Gestion du client
 * \author Skylord65
 * \date 03/05/2025
 **/

#define LG_MAX 1000


int main(int argc, char const *argv[]){
    //---------------------------//
    // Déclaration des variables //
    //---------------------------//

    Message_t message_recu, message;
    int sid;
    struct sockaddr_in dst_serv_addr;

    //------------------------------//
    // Initialisation des variables //
    //------------------------------//

    dst_serv_addr.sin_family = AF_INET;
    dst_serv_addr.sin_port = htons(atoi(argv[2]));

    if(argc!= 3) {
        perror("Problème sur le nombre de paramètres.");
        return EXIT_FAILURE;
    }

    sid = socket(AF_INET, SOCK_STREAM, 0);
    
    if(sid==-1) {
        perror("Erreur dans la création de socket.");
        return EXIT_FAILURE;
    }

    fill_message(&message, "message");
    memset(&dst_serv_addr, 0, sizeof(dst_serv_addr)); // Déclarer la mémoire nécessaire
    inet_pton(AF_INET, argv[1], &dst_serv_addr.sin_addr); // Attribuer l'ip pour le socket

    if(connect(sid, (struct sockaddr*) &dst_serv_addr, sizeof(dst_serv_addr))==-1) {
        perror("Erreur lors de la connection.");
        close(sid);
        return EXIT_FAILURE;
    }

    //-----------------------------------//
    // Séparation du client et du server //
    //-----------------------------------//

    pid_t pid = fork();

    switch(pid) {
        case -1:
            perror("Erreur de la création de fils receptioniste des messages.");
            return EXIT_FAILURE;
        case 0:
            do {
                /** code du buffer pour entrer le message (temporairement le message envoyé sera "message") */

                char buffer[LG_MAX];
                char c;
                int i = 0;
                scanf("%c", &c);
                while(c != '\n' && i<LG_MAX-1) {
                    buffer[i] = c;
                    scanf("%c", &c);
                    i++;
                }
                buffer[i] = '\0';
                /* ajouter une commande "/exit" pour quitter le buffer */

                fill_message(&message, buffer);

                send_message(&message, sid);

                printf("(client) :");
                print_message(message);
                
            } while (1);
            break;
        default:
            break;
        }
    do {
        receive_message(&message_recu, sid);
        if (message_recu.lg_message == 0) {
            printf("Le serveur a fermé la connexion.\n");
            close(sid);
            exit(EXIT_FAILURE);
        }
        printf("(server) :");
        print_message(message_recu);
    } while (1);
    
    close(sid);

    return 0;
}

