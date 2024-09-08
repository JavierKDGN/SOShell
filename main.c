#include "shell.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(void) {
    char comando[MAX_COMANDOS];
    while (1) { // bucle "infinito"
        printf(">shell:$ ");

        // leer el comando del usuario
        if (fgets(comando, sizeof(comando), stdin) != NULL) {
            // eliminar el salto de linea final
            comando[strcspn(comando, "\n")] = 0;

            if (strcmp(comando, "exit") == 0) {
                break; // Si el comando es "exit", salir del bucle
            } 
            else if (strlen(comando) == 0) {
                continue; // s no hay comando, seguir esperando el siguiente comando
            } 
            // ejecutar el comando
            else {
                ejecutarComando(comando);
            }
        } else {
            // Si hay un error al leer el comando
            perror("fgets error");
            continue; // Imprime el error y sigue esperando un comando
        }
    }

    return 0;

}