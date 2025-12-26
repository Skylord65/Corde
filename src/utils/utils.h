#ifndef __UTILS_H__
#define __UTILS_H__

/**
 * \file utils.h
 * \brief Utilitaires et macros pour la gestion des processus enfants.
 * \author Manolo-dev
 * \date 26/12/2025
 * \version 0.1
 **/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdarg.h>
#include <sys/wait.h>
#include <sys/types.h>

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
 * \brief Forks plusieurs processus enfants pour exécuter des fonctions données.
 * \param[in] first La première fonction enfant à exécuter.
 * \param ... Les fonctions enfants supplémentaires à exécuter, terminées par NULL. Utilise les variadic arguments.
 **/
void forked(child_fn first, ...);

#endif // __UTILS_H__