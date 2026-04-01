#include <stdio.h>
#include <sys/wait.h>
#include "../src/utils/forked.h"

#include <libintl.h>
#include <locale.h>

#define _(String) gettext (String)

void child1(child_context *ctx) { 
    printf(_("Début %d: tâche %d\n"), ctx->pid, ctx->index); 
    sleep(3); 
    printf(_("Fin %d: tâche %d\n"), ctx->pid, ctx->index); 
}

void child2(child_context *ctx) { 
    printf(_("Début %d: tâche %d\n"), ctx->pid, ctx->index); 
    sleep(2); 
    printf(_("Fin %d: tâche %d\n"), ctx->pid, ctx->index); 
}

void child3(child_context *ctx) { 
    printf(_("Début %d: tâche %d\n"), ctx->pid, ctx->index); 
    sleep(1); 
    printf(_("Fin %d: tâche %d\n"), ctx->pid, ctx->index); 
}

int main() {
    setlocale(LC_ALL, "");
    bindtextdomain("Corde", "./locale");
    textdomain("Corde");
    
    forked(_("Erreur test de fork"), child1, child2, child3);
    
    return 0;
}