CC := "gcc"
CFLAGS := "-Wall -Wextra -g"
BUILD_DIR := "build"

default:
    @just --list

#================================#
#           Compilation          #
#================================#

# Compile le test des utilitaires
build-utils-test:
    @echo "Compilation du test utils..."
    {{CC}} {{CFLAGS}} tests/utils_test.c src/utils/utils.c -o {{BUILD_DIR}}/utils-test

# Compile tous les tests disponibles
build-tests: build-utils-test
    @echo "Tous les tests compilés"

# Compile le client (quand il sera prêt)
build-client:
    @echo "Compilation du client..."
    {{CC}} {{CFLAGS}} tests/client.c -o {{BUILD_DIR}}/client

# Compile le serveur (quand il sera prêt)
build-server:
    @echo "Compilation du serveur..."
    {{CC}} {{CFLAGS}} tests/server.c -o {{BUILD_DIR}}/server

# Compile tout le projet
build-all: setup build-tests
    @echo "Projet compilé"

#================================#
#            Exécution           #
#================================#

# Lance le test des utilitaires
test-utils: build-utils-test
    @echo "Exécution du test utils..."
    ./{{BUILD_DIR}}/utils-test

# Lance tous les tests
test: test-utils
    @echo "Tous les tests passés"

# Lance le client
run-client: build-client
    @echo "Lancement du client..."
    ./{{BUILD_DIR}}/client

# Lance le serveur
run-server: build-server
    @echo "Lancement du serveur..."
    ./{{BUILD_DIR}}/server

#================================#
#         Base de données        #
#================================#

# Initialise la base de données
db-init:
    @echo "Initialisation de la base de données..."
    sqlite3 database.db < src/ressource/db_make.sql
    @echo "Base de données créée"

# Affiche le schéma de la base de données
db-schema:
    @echo "Schéma de la base de données:"
    sqlite3 database.db ".schema"

#================================#
#           Utilitaires          #
#================================#

# Crée le répertoire de build
setup:
    @mkdir -p {{BUILD_DIR}}

# Nettoie les fichiers compilés
clean:
    @echo "Nettoyage..."
    rm -rf {{BUILD_DIR}}
    rm -f database.db
    @echo "Nettoyage terminé"

# Compte les lignes de code
sloc:
    @echo "Lignes de code:"
    @find src tests libs -name "*.c" -o -name "*.h" | xargs wc -l | tail -1

# Vérifie la syntaxe de tous les fichiers C
check:
    @echo "Vérification de la syntaxe..."
    {{CC}} {{CFLAGS}} -fsyntax-only src/utils/utils.c
    @echo "Syntaxe correcte"

# Affiche l'arborescence du projet
tree:
    @tree -I 'build|.git' --dirsfirst

#================================#
#          Développement         #
#================================#

# Mode développement : compile et lance les tests à chaque modification
watch:
    @echo "Mode watch activé..."
    @while true; do \
        inotifywait -e modify -r src/ tests/ 2>/dev/null; \
        clear; \
        just test || true; \
    done

# Formate le code (si clang-format est installé)
format:
    @echo "Formatage du code..."
    find src tests libs -name "*.c" -o -name "*.h" | xargs clang-format -i
    @echo "Code formaté"