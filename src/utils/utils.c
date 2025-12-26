#include "utils.h"

void forked(child_fn first, ...) {
    //================================================================//
    //            Variables pour les arguments variadiques            //
    //================================================================//

    va_list args;
    va_start(args, first);

    int nb_children = 0;
    while (va_arg(args, child_fn) != NULL) {
        nb_children++;
    }
    va_end(args);
    
    child_fn functions[nb_children];
    functions[0] = first;
    
    va_start(args, first);
    for (int i = 1; i < nb_children; i++) {
        functions[i] = va_arg(args, child_fn);
    }
    va_end(args);

    //================================================================//
    //                              Pipes                             //
    //================================================================//
    
    int pipes[nb_children][2];
    for (int i = 0; i < nb_children; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe failed");
            exit(1);
        }
    }

    //================================================================//
    //                      Création des enfants                      //
    //================================================================//

    pid_t child_pids[nb_children];

    for (int i = 0; i < nb_children; i++) {
        pid_t pid = fork();
        
        switch (pid) {
        case -1:
            perror("fork failed");
            exit(1);
            
        case 0:
            child_context ctx;
            ctx.index = i;
            ctx.pid = getpid();
            ctx.nb_children = nb_children;
            ctx.pipe_read = pipes[i][0];
            ctx.pipe_write = pipes[i][1];

            // Fermeture des pipes inutiles dans l'enfant
            for (int j = 0; j < nb_children; j++) {
                if (j == i) {
                    continue;
                } else {
                    close(pipes[j][1]);
                }
            }

            // Préparation du tableau de tous les pipes
            ctx.all_pipes = malloc(nb_children * 2 * sizeof(int));
            for (int j = 0; j < nb_children; j++) {
                ctx.all_pipes[j * 2] = pipes[j][0];
                ctx.all_pipes[j * 2 + 1] = pipes[j][1];
            }

            // Exécution de la fonction enfant
            if (functions[i]) {
                functions[i](&ctx);
            }
            
            // Nettoyage dans l'enfant
            free(ctx.all_pipes);
            
            for (int j = 0; j < nb_children; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }
            
            exit(0);
            
        default:
            child_pids[i] = pid;
            break;
        }
    }

    //================================================================//
    //                            Nettoyage                           //
    //================================================================//
    
    for (int i = 0; i < nb_children; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    for (int i = 0; i < nb_children; i++) {
        int status;
        waitpid(child_pids[i], &status, 0);
    }
}