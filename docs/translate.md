# Traduction

## Contribution

Le texte étant en francais, pour generer les fichiers `.pot` il faut utiliser l'encodage en UTF-8   
Exemple : `xgettext -k_ --from-code=UTF-8 -o local/Corde.pot tests/client.c`

Compiler la traduction :
```sh
    just locales
```

### Exemple

```C
// Correct
printf(_("Début %d: tâche %d\n"), ctx->pid, ctx->index); 
// Incorrect
printf(_("Début %d: tâche %d\n", ctx->pid, ctx->index)); 
```

> [!NOTE]
> ```
>#: tests/utils_test.c:13 tests/utils_test.c:19 tests/utils_test.c:25
>#, fuzzy, c-format
>#| msgid "Début %d: tâche %d\n"
>msgid "Fin %d: tâche %d\n"
>msgstr "End %d: task %d\n"
>```
> Bien pensé a transformer cela en 
> ```
>#: tests/utils_test.c:13 tests/utils_test.c:19 tests/utils_test.c:25
>msgid "Fin %d: tâche %d\n"
>msgstr "End %d: task %d\n"
>```
>Pour que la traduction sois appliqué
