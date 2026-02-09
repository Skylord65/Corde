CC := "gcc"
CFLAGS := "-Wall -Wextra -g"
BUILD_DIR := "build"
LOCALEDIR := "locale"
MESSAGE_TEST := "Message de test du client"

default:
    @just --list

#================================#
#      Internationalization      #
#================================#

# Compile les fichiers .po en .mo et les nettoie du flag 'fuzzy'
locales: extract
    @echo "Mise à jour et compilation des fichiers de traduction..."
    @for po_file in {{LOCALEDIR}}/*/*.po; do \
        lang=$(basename "$po_file" .po); \
        mkdir -p "{{LOCALEDIR}}/$lang/LC_MESSAGES"; \
        msgmerge -U "$po_file" "{{LOCALEDIR}}/Koncord.pot"; \
        sed -i 's/#, fuzzy//g' "$po_file"; \
        msgfmt "$po_file" -o "{{LOCALEDIR}}/$lang/LC_MESSAGES/Koncord.mo"; \
    done

# Extrait toutes les chaînes de caractères du projet
extract:
    @echo "Extraction des chaînes..."
    @mkdir -p {{LOCALEDIR}}
    xgettext -k_ --from-code=UTF-8 -o {{LOCALEDIR}}/Koncord.pot */*.c

#================================#
#           Compilation          #
#================================#

# Compile le test des utilitaires
build-utils-test: setup
    @echo "Compilation du test utils..."
    {{CC}} {{CFLAGS}} tests/utils_test.c src/utils/utils.c -o {{BUILD_DIR}}/utils-test

build-client-test: setup
    @echo "Compilation du test client..."
    {{CC}} {{CFLAGS}} tests/client.c src/utils/utils.c -o {{BUILD_DIR}}/client-test

build-server-test: setup
    @echo "Compilation du test serveur..."
    {{CC}} {{CFLAGS}} tests/server.c src/utils/utils.c -o {{BUILD_DIR}}/server-test

# Compile tous les tests disponibles
build-tests: build-utils-test build-server-test build-client-test
    @echo "Tous les tests compilés"

# Compile le client
#build-client: setup
#    @echo "Compilation du client..."
#    {{CC}} {{CFLAGS}} tests/client.c -o {{BUILD_DIR}}/client

# Compile le serveur
#build-server: setup
#    @echo "Compilation du serveur..."
#    {{CC}} {{CFLAGS}} tests/server.c -o {{BUILD_DIR}}/server

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

test-client: build-client-test
    @echo "Exécution du test client..."
    ./{{BUILD_DIR}}/client-test

test-server: build-server-test
    @echo "Exécution du test serveur..."
    ./{{BUILD_DIR}}/server-test &
    @sleep 1
    echo "{{MESSAGE_TEST}}" | ./{{BUILD_DIR}}/client-test
    @sleep 1
    @kill `pgrep server-test` || true
    @echo "Test serveur réussi fermeture. "

# Lance tous les tests sauf le client qui est testé avec le serveur
test: test-utils test-server
    @echo "Tous les tests passés"

# Lance le client
# run-client: build-client
#     @echo "Lancement du client..."
#     ./{{BUILD_DIR}}/client

# # Lance le serveur
# run-server: build-server
#     @echo "Lancement du serveur..."
#     ./{{BUILD_DIR}}/server

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
    rm -rf {{LOCALEDIR}}/*/LC_MESSAGES/*.mo
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
