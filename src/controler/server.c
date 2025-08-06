#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "message.h"

#define MAX_USER 100

/** 
 * \file server.c
 * \brief Gestion du serveur
 * \author Skylord65
 * \date 03/05/2025
 **/

User_t user_list[MAX_USER];
pthread_mutex_t user_mutex = PTHREAD_MUTEX_INITIALIZER;

void broadcast_message(Message_t *msg, int from_sock) {
    pthread_mutex_lock(&user_mutex);
    for (int i = 0; i < MAX_USER; i++) {
        if (user_list[i].socket != 0 && user_list[i].socket != from_sock) {
            send_message(msg, user_list[i].socket);
        }
    }
    pthread_mutex_unlock(&user_mutex);
}

void* handle_client(void* arg) {
    int client_sock = *(int*)arg;
    free(arg);
    Message_t msg;
    int idx = -1;

    // Receive pseudo
    if (receive_message(&msg, client_sock) < 0) {
        close(client_sock);
        return NULL;
    }
    pthread_mutex_lock(&user_mutex);
    for (int i = 0; i < MAX_USER; i++) {
        if (user_list[i].socket == 0) {
            user_list[i].socket = client_sock;
            strncpy(user_list[i].pseudo, msg.Message, sizeof(user_list[i].pseudo)-1);
            user_list[i].pseudo[sizeof(user_list[i].pseudo)-1] = '\0';
            idx = i;
            break;
        }
    }
    pthread_mutex_unlock(&user_mutex);

    if (idx == -1) {
        close(client_sock);
        return NULL;
    }

    printf("Client %s connecté\n", user_list[idx].pseudo);

    while (1) {
        if (receive_message(&msg, client_sock) == -1) break;
        printf("(%s) : %s\n", user_list[idx].pseudo, msg.Message);
        broadcast_message(&msg, client_sock);
    }

    pthread_mutex_lock(&user_mutex);
    user_list[idx].socket = 0;
    pthread_mutex_unlock(&user_mutex);
    close(client_sock);
    printf("Client %s déconnecté\n", user_list[idx].pseudo);
    return NULL;
}

int main(void) {
    memset(user_list, 0, sizeof(user_list));
    int sid = socket(AF_INET, SOCK_STREAM, 0);
    if (sid == -1) { perror("socket"); return EXIT_FAILURE; }

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5555);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sid, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("bind"); close(sid); return EXIT_FAILURE;
    }
    if (listen(sid, 15) == -1) {
        perror("listen"); close(sid); return EXIT_FAILURE;
    }
    printf("Serveur en attente de connexions...\n");

    while (1) {
        struct sockaddr_in cli_addr;
        socklen_t cli_len = sizeof(cli_addr);
        int* client_sock = malloc(sizeof(int));
        *client_sock = accept(sid, (struct sockaddr*)&cli_addr, &cli_len);
        if (*client_sock < 0) { perror("accept"); free(client_sock); continue; }
        pthread_t tid;
        pthread_create(&tid, NULL, handle_client, client_sock);
        pthread_detach(tid);
    }
    close(sid);
    return 0;
}