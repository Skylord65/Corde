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
 * \param[in] error_msg Message d'erreur à afficher en cas d'échec du fork.
 * \param[in] first La première fonction enfant à exécuter.
 * \param ... Les fonctions enfants supplémentaires à exécuter, terminées par NULL. Utilise les variadic arguments.
 * \warning Ne pas oublier de terminer la liste des fonctions par NULL.
 **/
void forked(char* error_msg, child_fn first, ...);

/**
 * \def VARIADIC_TO_ARRAY(type, array_name, count_name, first_arg)
 * \brief Macro pour convertir des arguments variadiques en tableau.
 * \param type Type des arguments variadiques.
 * \param array_name Nom du tableau résultant.
 * \param count_name Nom de la variable contenant le nombre d'arguments.
 * \param first_arg Le premier argument variadique.
 * \details Cette macro initialise une liste d'arguments variadiques, compte le nombre d'arguments jusqu'à NULL,
 **/
#define VARIADIC(type, array_name, count_name, first_arg) \
    va_list _va_args; \
    va_start(_va_args, first_arg); \
    int count_name = 1; \
    while (va_arg(_va_args, type) != NULL) { \
        count_name++; \
    } \
    va_end(_va_args); \
    type array_name[count_name]; \
    array_name[0] = first_arg; \
    va_start(_va_args, first_arg); \
    for (int _va_i = 1; _va_i < count_name; _va_i++) { \
        array_name[_va_i] = va_arg(_va_args, type); \
    } \
    va_end(_va_args)

#endif // __UTILS_H__