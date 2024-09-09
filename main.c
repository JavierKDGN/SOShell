#include "shell.h"
#include "favs.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(void) {
    ComFavorito *favs = malloc(sizeof(ComFavorito) * MAX_FAVS);
    int num_favs = 0;
    char comando[MAX_COMANDOS];
    while (1) { // bucle "infinito"
        printf(">shell:$ ");

        // leer el comando del usuario
        if (fgets(comando, sizeof(comando), stdin) != NULL) {
            // eliminar el salto de linea final
            comando[strcspn(comando, "\n")] = '\0';

            if (strcmp(comando, "exit") == 0) {
                if (num_favs > 0) {
                    guardarFavs(favs, &num_favs);
                }
                break; // Si el comando es "exit", salir del bucle
            } 
            else if (strlen(comando) == 0) {
                continue; // s no hay comando, seguir esperando el siguiente comando
            } 
            else if (strncmp(comando, "favs", 4) == 0)  { //Si el comando empieza con favs
                procesarFavs(favs, &num_favs, comando);
            }
            else if (strncmp(comando, "set recordatorio", 16) == 0) {
                crearRecordatorio(comando);
            }
            // ejecutar el comando
            else {
                if (ejecutarComando(comando) == 0) {
                addFav(favs, &num_favs, comando);
                }
            }
        } else {
            // Si hay un error al leer el comando
            perror("fgets error");
            continue; // Imprime el error y sigue esperando un comando
        }
    }

    return 0;

}