#include <stdio.h>
#include "../../libs/sqlite3/sqlite3.h"

int main(void) {
    
    printf("%s\n", sqlite3_libversion());
    
    return 0;
}