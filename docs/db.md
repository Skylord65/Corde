# Base de données

## Utilisateurs

Chaque utilisateur est identifié par un pseudo ainsi qu'un numéro d'identification. Par exemple, les mainteneurs actuels de ce projet auraient comme identifiant : `Skylord#65`, `Rhexephon#0`, `Manolo#0`.

## Architecture discussions

L'architecture des discussions est sous forme d'arborescence. Un _Topic_ (noeud de l'arbre) peut contenir lui même d'autres topics, ainsi qu'un  (ou plusieurs) _Thread_ (feuilles de l'arbre).

Un _Server_ est un _Topic_ dit **racine**, lui même n'étant contenu dans aucun autre topic. Il contient à minima un thread (qui soit ou non dans un sous-topic).

### Règles

Un _Thread_ peut être régit par des règles (par exemple, seul certains rôles peuvent envoyer des messages, ou un message ne peut pas répondre à un autre, etc.). Un _Topic_ peut aussi être régit par des règles, dans ce cas tous ses descendants (topics comme threads) seront régit par (au moins) ces règles.

## Messages

Un message peut (non obligatoire) répondre à un autre message.

## Schéma relationnel

```mermaid
flowchart
    RULE[RULE<hr>
        int id
        string content
    ]
    ANSWER((ANSWER<hr>))

    MESSAGE[MESSAGE<hr>
        int id
        string content
        time created_at
    ]
    THREAD[THREAD<hr>
        int id
        string title
    ]
    USER[USER<hr>
        string pseudo
        int id
        blob picture
        string hashed_password
        time created_at
    ]
    TOPIC[TOPIC<hr>
        int id
        string title
        enum type
    ]

    TOPIC_RULE((TOPIC_RULE<hr>))
    
    MEMBER((MEMBER<hr>))
    ROLE[ROLE<hr>
        int id
        string name
    ]

    ON((ON<hr>))
    BY((BY<hr>))
    PARENT((PARENT<hr>))
    LINK((LINK<hr>))
    THREAD_RULE((THREAD_RULE<hr>))

    MESSAGE o--o|"0-1"| ANSWER
    ANSWER o--o|"0-1"| MESSAGE
    MESSAGE o--o|"1-1"| ON
    MESSAGE o--o|"0-n"| BY
    BY o--o|"1-1"| USER
    PARENT o--o|"0-n"| TOPIC
    PARENT o--o|"0-1"| TOPIC
    LINK o--o|"0-n"| TOPIC
    LINK o--o|"1-1"| THREAD
    ON o--o|"0-n"| THREAD
    USER o--o|"0-n"| MEMBER
    MEMBER o--o|"0-n"| ROLE
    TOPIC o--o|"0-n"| MEMBER
    THREAD_RULE o--o|"0-n"| THREAD
    THREAD_RULE o--o|"0-n"| RULE
    TOPIC o--o|"0-n"| TOPIC_RULE
    RULE o--o|"0-n"| TOPIC_RULE
```