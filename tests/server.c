#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <libintl.h>
#include <locale.h>

#define _(String) gettext (String)

#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 5

int main(void)
{
    int server_fd, client_fd;
    struct sockaddr_in adresse_serveur, adresse_client;
    socklen_t taille_addresse_client;
    char buffer[BUFFER_SIZE];
    int bytes_recus;
    int opt = 1;

    setlocale(LC_ALL, "");
    bindtextdomain("Koncord", "./locale");
    textdomain("Koncord");
    
    // 1. Créer le socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror(_("Erreur création socket"));
        exit(EXIT_FAILURE);
    }
    printf(_("[Serveur] Socket crée\n"));

    // 2. Configurer les options du socket (réutilisation de l'adresse)
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror(_("Error setsockopt"));
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 3. Configurer l'adresse du serveur
    adresse_serveur.sin_family = AF_INET;
    adresse_serveur.sin_addr.s_addr = INADDR_ANY;
    adresse_serveur.sin_port = htons(PORT);

    // 4. Lier le socket à l'addresse et au port
    if(bind(server_fd, (struct sockaddr *)&adresse_serveur, sizeof(adresse_serveur)) < 0) {
        perror(_("Erreur bind"));
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf(_("[Serveur] Socket lié au port %d\n"), PORT);

    // 5. Mettre le socket en mode écoute
    if(listen(server_fd, MAX_CLIENTS) < 0) {
        perror(_("Erreur listen"));
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf(_("[Serveur] En attente de connexion...\n\n"));

    // 6. Boucle principale pour accepter les connexions
    while (1) {
        taille_addresse_client = sizeof(adresse_client);

        // Accepter une connexion entrante
        client_fd = accept(server_fd, (struct sockaddr *)&adresse_client, &taille_addresse_client);
        if(client_fd < 0) {
            perror(_("Erreur accept"));
            continue;
        }

        // Afficher les informations du client connecté
        char ip_client[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &adresse_client.sin_addr, ip_client, INET_ADDRSTRLEN);
        printf(_("[Serveur] Client connecté depuis %s:%d\n"), ip_client, ntohs(adresse_client.sin_port));

        // 7. Recevoir le message du client
        memset(buffer, 0, BUFFER_SIZE);
        bytes_recus = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);

        if(bytes_recus < 0) {
            perror(_("Erreur réception"));
        } else if(bytes_recus == 0) {
            printf(_("[Serveur] Client déconnecté\n"));
        } else {
            buffer[bytes_recus] = '\0';
            printf(_("[Serveur] Message reçu (%d octets) : %s"), bytes_recus, buffer);
        }

        // 8. Fermer la connexion avec ce client
        close(client_fd);
        printf(_("[Serveur] Connexion fermée avec le client\n\n"));
        printf(_("[Serveur] En attente de nouvelle connexions...\n\n"));
    }
    
    // 9. Fermer le socket serveur (code jamais atteint dans cette boucle infinie)
    close(server_fd);

    return 0;
}
