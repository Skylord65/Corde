#ifndef __VARIADIC_H__
#define __VARIADIC_H__

/**
 * \file variadic.h
 * \brief Utilitaires et macros pour la gestion des variadiques.
 * \author Manolo-dev
 * \date 11/02/2026
 * \version 0.1
 * \note On différenciera les variadiques illimitées (finies par NULL) et les limitées (avec un nombre d'arguments défini).
 **/

/**
 * \def VARIADIC_TO_ARRAY(type, array_name, count_name, first_arg)
 * \brief Macro pour convertir des arguments variadiques illimités en un tableau.
 * \param type Type des arguments variadiques.
 * \param array_name Nom du tableau résultant.
 * \param count_name Nom de la variable contenant le nombre d'arguments.
 * \param first_arg Le premier argument variadique.
 * \details |
    Cette macro initialise une liste d'arguments variadiques,
    compte le nombre d'arguments jusqu'à NULL,
    puis crée un tableau et le remplit avec les arguments.
 * \example VARIADIC(child_fn, functions, nb_children, first);
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

/**
 * \def COUNT_ARGS(...)
 * \brief Macro pour compter le nombre d'arguments variadiques limités.
 * \details |
    Cette macro utilise une astuce de préprocesseur pour compter le nombre d'arguments
    en les alignant avec une séquence de nombres décroissants. [insérer meme OSS 117 "C'est une astuce"]
 * \example COUNT_ARGS(child1, child2, child3) // Renvoie 3
 **/
#define COUNT_ARGS(...) COUNT_ARGS_IMPL(__VA_ARGS__, 10,9,8,7,6,5,4,3,2,1)

/**
 * \def COUNT_ARGS_IMPL(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,N,...)
 * \brief Macro interne pour COUNT_ARGS.
 * \details |
    Cette macro prend les arguments variadiques alignés avec une séquence de nombres décroissants
    et renvoie le nombre d'arguments en utilisant la position du dernier argument.
 * \example COUNT_ARGS_IMPL(child1, child2, child3, 10,9,8,7,6,5,4,3,2,1) // Renvoie 3

 **/
#define COUNT_ARGS_IMPL(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,N,...) N

#endif // __VARIADIC_H__