#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

//FALTA MOVER MACROS Y DEFINICIONES A HEADER

#define MAX_FAVS 1024
#define MAX_COMANDOS 1024

typedef enum {
    crear, //Listo
    mostrar, //Listo
    eliminar, //Listo
    buscar, 
    borrar, //Listo
    ejecutar,
    cargar,
    guardar
} argumentos_favs;

//Struct que define un comando preferido
typedef struct {
    char comando[MAX_COMANDOS];
    int id;
    bool eliminado;
} ComFavorito;

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
            addFav(coincidentes, &num_coincidentes, favs[i].comando);
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

void guardarFavs(ComFavorito *favs, int num_favs) {
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

    for (int i = 0; i < num_favs; i++) {
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

//Nuevo test creado por copilot :Commit actual funciona bien
void testCrearGuardarCargar() {
    ComFavorito *favs = malloc(sizeof(ComFavorito) * MAX_FAVS);
    if (favs == NULL) {
        fprintf(stderr, "Error: No se pudo asignar memoria para los favoritos.\n");
        return;
    }
    int num_favs = 0;

    // Crear favoritos
    addFav(favs, &num_favs, "comando1");
    addFav(favs, &num_favs, "comando2");
    addFav(favs, &num_favs, "comando3");

    //Buscar comando1
    buscarStringEnFavs(favs, &num_favs, "1");

    // Guardar favoritos
    const char *filename = "favs.txt";
    crearArchivoFavs(filename);

    guardarFavs(favs, num_favs);

    // Borrar favoritos
    borrarFavs(&favs, &num_favs);

    // Cargar favoritos
    cargarFavs(favs, &num_favs);

    // Mostrar favoritos cargados
    printf("Lista de favoritos cargados:\n");
    mostrarFavs(favs, &num_favs);

    // Clean up
    free(favs);
}

int main() {
    testCrearGuardarCargar();
    return 0;
}