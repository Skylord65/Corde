#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>

int main(int argc, char const *argv[])
{

    //---------------------------//
    // Déclaration des variables //
    //---------------------------//
    pid_t pid_server, pid_client, pid_main_page;
    int pipe_server[2], pipe_client[2];

    //------------------------------//
    // Initialisation des variables //
    //------------------------------//
    pid_server=fork(), pid_client=fork(), pid_main_page=fork();

    //-------------------------------------//
    // Création des tubes de communication //
    //-------------------------------------//

    pipe(pipe_client);
    pipe(pipe_server);

    //---------------------------------//
    // Repartition des différents fork //
    //---------------------------------//

    // fork mainpage
    switch (pid_main_page)
    {
    case -1:
        perror("erreur fork main page");
        exit(1);
        break;
    case 0:
        // compléter avec les paramètres du mainpage
        execlp("../boundary/mainpage", "");
        exit(1);
        break;
    default:
        break;
    }

    //fork server
    switch (pid_server)
    {
    case -1:
        perror("erreur fork server");
        exit(1);
        break;
    case 0:
        // argv0 : nom programme, argv1 : ip, argv2 : port, argv3 : tube serveur, argv4 : tube client 
        execlp("./server", "server", "127.0.0.1", "5555", pipe_server, pipe_client);
        exit(1);
        break;
    default:
        break;
    }

    // fork client (client en terminal)
    switch (pid_client)
    {
    case -1:
        perror("erreur fork client");
        exit(1);
        break;
    case 0:
        // // argv0 : nom programme, argv1 : ip, argv2 : port, argv3 : tube client, argv4 : tube serveur
        execlp("./client", "client", "127.0.0.1", "5555", pipe_client, pipe_server);
        exit(1);
        break;
    default:
        break;
    }

    return 0;
}
