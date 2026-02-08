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

int main() {
    int client_fd;
    struct sockaddr_in adresse_serveur;
    char message[BUFFER_SIZE];
    setlocale(LC_ALL, "");

    bindtextdomain("Koncord", "./locale");
    textdomain("Koncord");
    
    // 1. Créer le socket
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1) {
        perror(_("Erreur création socket"));
        exit(EXIT_FAILURE);
    }
    printf(_("[Client] Socket créé\n"));
    
    // 2. Configurer l'adresse du serveur
    adresse_serveur.sin_family = AF_INET;
    adresse_serveur.sin_port = htons(PORT);
    
    // Convertir l'adresse IP (localhost = 127.0.0.1)
    if (inet_pton(AF_INET, "127.0.0.1", &adresse_serveur.sin_addr) <= 0) {
        perror(_("Adresse invalide"));
        close(client_fd);
        exit(EXIT_FAILURE);
    }
    
    // 3. Se connecter au serveur
    if (connect(client_fd, (struct sockaddr *)&adresse_serveur, sizeof(adresse_serveur)) < 0) {
        perror(_("Erreur connexion au serveur"));
        close(client_fd);
        exit(EXIT_FAILURE);
    }
    printf(_("[Client] Connecté au serveur\n\n"));
    
    // 4. Demander un message à l'utilisateur
    printf(_("Entrez votre message : "));
    fflush(stdout);
    
    // Lire le message (avec gestion du retour à la ligne)
    if (fgets(message, BUFFER_SIZE, stdin) == NULL) {
        perror(_("Erreur lecture message"));
        close(client_fd);
        exit(EXIT_FAILURE);
    }
    
    // 5. Envoyer le message au serveur
    if (send(client_fd, message, strlen(message), 0) < 0) {
        perror(_("Erreur envoi"));
        close(client_fd);
        exit(EXIT_FAILURE);
    }
    printf(_("[Client] Message envoyé avec succès\n"));
    
    // 6. Fermer la connexion
    close(client_fd);
    printf(_("[Client] Connexion fermée\n"));
    
    return 0;
}