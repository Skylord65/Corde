#ifndef __MESSAGE_H__
#define __MESSAGE_H__

/** 
 * \file message.h
 * \brief Gestion des messages
 * \author Skylord65
 * \date 03/05/2025
 **/


 /**
 * \def LG_MAX
 * \brief Taille max des message (octets)
 */
#define LG_MAX 1000


/**
 * \struct Message_s
 * \brief Information d'un message
**/
typedef struct Message_s{
    int lg_message;
    char Message[LG_MAX];

}Message_t;

/**
 * \struct User_s
 * \brief Information d'un utilisateur
 */
typedef struct User_s {
    int socket;
    char pseudo[50];
} User_t;
/**
 * \author Skylord65
 * \date 03/05/2025
 * \version 0
 * \brief Rempli une structure message_t
 * \param[in] Message structure qui devra contenir le message
 * \param[in] message chaine de caractère contenant le message
 * \pre message déjà aloué
 * \post 
**/
extern void fill_message(Message_t *Message, char *message);

/**
 * \author Skylord65
 * \date 03/05/2025
 * \version 0
 * \brief Rempli une structure message_t
 * \param[in] Message structure qui devra contenir le message
 * \pre structure déjà aloué
 * \post 
**/
extern void print_message(Message_t Message);

/**
 * \author Skylord65
 * \date 03/05/2025
 * \version 0
 * \brief Evoie une structure message_t
 * \param[in] Message structure qui devra contenir le message envoyé
 * \param[in] socket entier d'identification du socket permettant la communication réseau
 * \pre socket déjà initialisé et structure remplie
 * \post 
**/
extern void send_message(Message_t *Message, int socket);

/**
 * \author Skylord65
 * \date 03/05/2025
 * \version 0
 * \brief Reçoit une structure message_t
 * \param[in] Message structure qui devra contenir le message reçu
 * \param[in] socket entier d'identification du socket permettant la communication réseau
 * \pre socket déjà initialisé
 * \post 
**/
extern void receive_message(Message_t *Message, int socket);

#endif // __MESSAGE_H__