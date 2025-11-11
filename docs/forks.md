# Séparation fonctionnelle

## Main

Le main fork sur trois fichiers : `boundary/mainpage.c`, `controler/server.c`, `controler/client.c`.

```mermaid
flowchart TD
    MAIN[MAIN] -->|fork| SERVER
    MAIN -->|fork| CLIENT
    MAIN -->|fork| MAINPAGE

    CLIENT -->|PIPE 1| MAIN
    SERVER -->|PIPE 2| MAIN
    MAIN -->|PIPE 3| MAINPAGE

    subgraph CONTROLER [Couche Controler]
        SERVER
        CLIENT
    end

    subgraph BOUNDARY [Couche Boundary]
        MAINPAGE
    end

    MAINPAGE -->|entrées| CLIENT
    SERVER -->|affichage| MAINPAGE
    
    class CONTROLER controller
    class BOUNDARY boundary
    class MAIN main
```

### Server

Le server s'occupe de recevoir les messages.

### Client

Le client s'occupe d'envoyer les messages.

### MainPage

La mainpage s'occuper d'afficher et de prendre en entrée les messages.

## Communication

Il y a trois pipe : une entre client et main, une entre server et main, et une entre main et mainpage.