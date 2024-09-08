#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "favs.h"

argumentos_favs obtenerArgumento(const char *argumento) {
    if (strcmp(argumento, "crear") == 0) return crear;
    else if (strcmp(argumento, "mostrar") == 0) return mostrar;
    else if (strcmp(argumento, "eliminar") == 0) return eliminar;
    else if (strcmp(argumento, "buscar") == 0) return buscar;
    else if (strcmp(argumento, "borrar") == 0) return borrar;
    else if (strcmp(argumento, "ejecutar") == 0) return ejecutar;
    else if (strcmp(argumento, "cargar") == 0) return cargar;
    else if (strcmp(argumento, "guardar") == 0) return guardar;
    else return undefined;
}

void procesarFavs(ComFavorito *favs, int *num_favs, const char *comando) {
    char *args = strtok((char *)comando, " "); // Primer llamado a strtok para "favs"
    
    // Avanzar al siguiente token (el argumento dsps de "favs")
    args = strtok(NULL, " ");
    
    if (args == NULL) {
        fprintf(stderr, "Error: Faltan argumentos de favs\n");
        return;
    }

    argumentos_favs argumento = obtenerArgumento(args);

    switch (argumento) {
        case crear: {
            char *ruta = strtok(NULL, " "); // obtener el siguiente argumento
            if (ruta == NULL) {
                fprintf(stderr, "Error: No se ha proporcionado una ruta para crear archivo\n");
            } else {
                crearArchivoFavs(ruta); // Llamada a funcion de creacion de archivo
            }
            break;
        }
        case mostrar:
            mostrarFavs(favs, num_favs);
            break;

        case eliminar: {
            char *nums = strtok(NULL, " "); // numeros de favoritos a eliminar
            if (nums != NULL) {
                // Parsear los numeros y llamar a eliminar
            } else {
                fprintf(stderr, "Error: Debe proporcionar números de favoritos para eliminar\n");
            }
            break;
        }

        case buscar: {
            char *cmd = strtok(NULL, " ");
            if (cmd != NULL) {
                buscarStringEnFavs(favs, num_favs, cmd);
            } else {
                fprintf(stderr, "Error: Debe proporcionar un comando para buscar\n");
            }
            break;
        }

        case borrar:
            borrarFavs(&favs, num_favs);
            break;

        case ejecutar:
            break;

        case cargar:
            cargarFavs(favs, num_favs);
            break;

        case guardar:
            guardarFavs(favs, num_favs);
            break;

        default:
            fprintf(stderr, "Error: argumento desconocido para favs\n");
            break;
    }
}


bool estaEnFavs(ComFavorito *favs, int *num_favs, const char *comando) {
    for (int i = 0; i < *num_favs; i++) {
        if (strcmp(favs[i].comando, comando) == 0) {
            if (favs[i].eliminado == true) return false;
            return true; //El comando esta en la lista de favoritos
        }   
    }
    return false;
}

void addFav(ComFavorito *favs, int *num_favs, const char *comando) {
    if (*num_favs >= MAX_FAVS) {
        fprintf(stderr, "Error: lista de favoritos llena (%d de %d).\n", *num_favs, MAX_FAVS);
        return;
    }

    if (!estaEnFavs(favs, num_favs, comando)) {
        favs[*num_favs].id = *num_favs + 1; //agrega a favs el id nuevo
        strncpy(favs[*num_favs].comando, comando, MAX_COMANDOS - 1); //asigna a ese id el comando ingresado
        favs[*num_favs].comando[MAX_COMANDOS - 1] = '\0'; // pone el caracter para terminar la string
        favs[*num_favs].eliminado = false;
        (*num_favs)++; //incrementa el contador de favoritos
    }
}

void mostrarFavs(ComFavorito *favs, int *num_favs) {
    for (int i = 0; i < *num_favs; i++) {
        if (favs[i].eliminado == true) {
            continue;
        } else {
            printf("%d: %s\n", favs[i].id, favs[i].comando);
        }
    }
}

void buscarStringEnFavs(ComFavorito *favs, int *num_favs, const char* str) {
    ComFavorito *coincidentes = malloc(sizeof(ComFavorito) * MAX_FAVS);
    int num_coincidentes = 0;
    for (int i = 0; i < *num_favs; i++) {
        if (strstr(favs[i].comando, str) != NULL) { //Busca la palabra str en el comando
            coincidentes[num_coincidentes].id = favs[i].id;
            strncpy(coincidentes[num_coincidentes].comando, favs[i].comando, MAX_COMANDOS - 1);
            coincidentes[num_coincidentes].comando[MAX_COMANDOS - 1] = '\0';
            coincidentes[num_coincidentes].eliminado = false;
            num_coincidentes++;
        }
    }

    if (num_coincidentes > 0) {
        mostrarFavs(coincidentes, &num_coincidentes);
    } else {
        printf("No se encontraron favoritos que contengan la palabra '%s'\n", str);
    }

} 



void borrarFavs(ComFavorito **favs, int *num_favs) {
    ComFavorito *new_favs = malloc(sizeof(ComFavorito) * MAX_FAVS);
    if (new_favs == NULL) {
        fprintf(stderr, "Error: No se pudo asignar memoria para los nuevos favoritos.\n");
        return;
    }
    free(*favs);
    *favs = new_favs;
    *num_favs = 0;
}

void crearArchivoFavs(const char *ruta) {
    FILE *arch = fopen(ruta, "w");
    if (arch == NULL) {
        fprintf(stderr, "Error: no se pudo crear archivo\n");
        return;
    }
    fclose(arch);
    printf("Archivo creado en: %s\n", ruta);

    FILE *aux_data = fopen("shell_data.txt", "w"); // Archivo auxiliar para recordar ruta
    if (aux_data == NULL) {
        fprintf(stderr, "Error: no se pudo crear archivo\n");
        return;
    }
    fprintf(aux_data, "%s", ruta);
    fclose(aux_data);
}

/*FORMATO PARA GUARDAR FAVS:
    id comando \n
*/

void guardarFavs(ComFavorito *favs, int *num_favs) {
    FILE *aux_data = fopen("shell_data.txt", "r");
    if (aux_data == NULL) {
        fprintf(stderr, "Error: no se pudo abrir archivo\n");
        return;
    }

    char ruta[1024]; // Variable para leer ruta guardada
    if (fgets(ruta, sizeof(ruta), aux_data) == NULL) {
        fprintf(stderr, "Error: No se ha detectado una ruta, cree una con el comando 'favs crear'\n");
        fclose(aux_data);
        return;
    }
    ruta[strcspn(ruta, "\n")] = 0; // Elimina el salto de línea
    fclose(aux_data);

    FILE *arch = fopen(ruta, "w");
    if (arch == NULL) {
        fprintf(stderr, "Error: no se pudo abrir archivo\n");
        return;
    }

    for (int i = 0; i < *num_favs; i++) {
        if (!favs[i].eliminado) {
            fprintf(arch, "%d %s\n", favs[i].id, favs[i].comando);  // Guardamos el id y el comando en el formato establecido
        }
    }
    fclose(arch);
}

void cargarFavs(ComFavorito *favs, int *num_favs) {
    FILE *aux_data = fopen("shell_data.txt", "r");
    if (aux_data == NULL) {
        fprintf(stderr, "Error: no se pudo abrir archivo\n");
        return;
    }

    char ruta[1024]; // Variable para leer ruta guardada
    if (fgets(ruta, sizeof(ruta), aux_data) == NULL) {
        fprintf(stderr, "Error: No se ha detectado una ruta, cree una con el comando 'favs crear'\n");
        fclose(aux_data);
        return;
    }
    ruta[strcspn(ruta, "\n")] = 0; // Elimina el salto de línea
    fclose(aux_data);

    FILE *arch = fopen(ruta, "r");
    if (arch == NULL) {
        fprintf(stderr, "Error: no se pudo abrir archivo\n");
        return;
    }
    
    *num_favs = 0;
    char comando[MAX_COMANDOS];
    int id;

    // fscanf lee cada línea
    while (fscanf(arch, "%d %[^\n]s", &id, comando) != EOF) {
        if (*num_favs >= MAX_FAVS) {
            fprintf(stderr, "Error: No se pudo cargar favoritos\n");
            return;
        }
        favs[*num_favs].id = id;
        strncpy(favs[*num_favs].comando, comando, MAX_COMANDOS - 1);
        favs[*num_favs].comando[MAX_COMANDOS - 1] = '\0';
        favs[*num_favs].eliminado = false;
        (*num_favs)++;
    }

    fclose(arch);
    mostrarFavs(favs, num_favs);
}
