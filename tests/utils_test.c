#include <stdio.h>
#include <sys/wait.h>
#include "../src/utils/utils.h"

void child1(child_context *ctx) { printf("Début %d: tâche %d\n", ctx->pid, ctx->index); sleep(3); printf("Fin %d: tâche %d\n", ctx->pid, ctx->index); }
void child2(child_context *ctx) { printf("Début %d: tâche %d\n", ctx->pid, ctx->index); sleep(2); printf("Fin %d: tâche %d\n", ctx->pid, ctx->index); }
void child3(child_context *ctx) { printf("Début %d: tâche %d\n", ctx->pid, ctx->index); sleep(1); printf("Fin %d: tâche %d\n", ctx->pid, ctx->index); }

int main() {
    forked("Error forked test", child1, child2, child3, NULL);
    return 0;
}