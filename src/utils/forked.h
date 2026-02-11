#ifndef __FORKED_H__
#define __FORKED_H__

/**
 * \file forked.h
 * \brief Utilitaires et macros pour la gestion des processus enfants.
 * \author Manolo-dev
 * \date 11/02/2026
 * \version 0.2
 **/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdarg.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "variadic.h"

/**
 * \struct child_context
 * \brief Contexte pour chaque processus enfant.
 * \details Contient le contexte et les pipes.
 * \note À utiliser dans les fonctions enfants passées à forked().
 * \warning Ne pas modifier directement les membres de cette structure.
 * \example |
 * void child_function(child_context *context) {
 *    // Utilisez context->index, context->pid, etc.
 * }
 **/
typedef struct {
    /**
     * \brief Index du processus enfant.
     **/
    int index;
    /**
     * \brief PID du processus enfant.
     **/
    pid_t pid;
    /**
     * \brief Nombre total de processus enfants.
     **/
    int nb_children;
    /**
     * \brief Pipe de lecture pour le processus enfant.
     **/
    int pipe_read;
    /**
     * \brief Pipe d'écriture pour le processus enfant.
     **/
    int pipe_write;
    /**
     * \brief Tableau des pipes pour tous les processus enfants.
     **/
    int *all_pipes;
} child_context;

/**
 * \def child_fn
 * \brief Type de fonction pour les processus enfants.
 **/
typedef void (*child_fn)(child_context *context);

/**
 * \def Macro appellant l'implémentation forked_impl en utilisant les macros pour compter le nombre d'argument et gérer une variadique.
 **/
#define forked(error_msg, ...) \
    forked_impl(error_msg, COUNT_ARGS(__VA_ARGS__), (child_fn[]){__VA_ARGS__})

/**
 * \brief Forks plusieurs processus enfants pour exécuter des fonctions données.
 * \param[in] error_msg Message d'erreur à afficher en cas d'échec du fork.
 * \param[in] nb_children Nombre de processus enfants à créer.
 * \param[in] functions Tableau de fonctions à exécuter dans les processus enfants.
 * \warning Il est impossible de donner plus de 10 arguments.
 * \details |
    Cette fonction crée nb_children processus enfants, chacun exécutant la fonction correspondante
    dans le tableau functions. Chaque processus enfant reçoit un contexte contenant son index, son PID,
    le nombre total de processus enfants, et les pipes pour communiquer avec les autres processus.
 **/
static inline void forked_impl(char* error_msg, int nb_children, child_fn functions[]) {
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
            perror(error_msg);
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

#endif // __FORKED_H__