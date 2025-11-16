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

    //------------------------------//
    // Initialisation des variables //
    //------------------------------//
    pid_server=fork(), pid_client=fork(), pid_main_page=fork();

    //---------------------------------//
    // Repartition des différents fork //
    //---------------------------------//

    // TODO : création des tubes et gestion des communications inter-processus.

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
        // compléter avec les paramètres du serveur
        execlp("./server", "127.0.0.1", "5555");
        exit(1);
        break;
    default:
        break;
    }

    // fork client
    switch (pid_client)
    {
    case -1:
        perror("erreur fork client");
        exit(1);
        break;
    case 0:
        // compléter avec les paramètres du client
        execlp("./client", "127.0.0.1", "5555");
        exit(1);
        break;
    default:
        break;
    }

    return 0;
}
