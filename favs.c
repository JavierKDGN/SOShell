#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

//FALTA MOVER MACROS Y DEFINICIONES A HEADER

#define MAX_FAVS 1024
#define MAX_COMANDOS 1024

typedef enum {
    crear,
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
    size_t id;
    bool eliminado;
} ComFavorito;

bool estaEnFavs(ComFavorito *favs, size_t *num_favs, const char *comando) {
    for (int i = 0; i < *num_favs; i++) {
        if (strcmp(favs[i].comando, comando) == 0) {
            if (favs[i].eliminado == true) return false;
            return true; //El comando esta en la lista de favoritos
        }   
    }
    return false;
}

void addFav(ComFavorito *favs, size_t *num_favs, const char *comando) {
    if (*num_favs >= MAX_FAVS) {
        fprintf(stderr, "Error: lista de favoritos llena (%d de %d).\n", num_favs, MAX_FAVS);
        return;
    }

    if (!estaEnFavs(favs, *num_favs, comando)) { //FALTA IMPLEMENTAR
        favs[*num_favs].id = *num_favs + 1; //agrega a favs el id nuevo
        strncpy(favs[*num_favs].comando, comando, MAX_COMANDOS - 1); //asigna a ese id el comando ingresado
        favs[*num_favs].comando[MAX_COMANDOS - 1] = '\0'; // pone el caracter para terminar la string
        favs[*num_favs].eliminado = false;
        (*num_favs)++; //incrementa el contador de favoritos
    }
}

void mostrarFavs(ComFavorito *favs, size_t *num_favs) {
    for (int i = 0; i < *num_favs; i++) {
        if (favs[i].eliminado == true) continue;
        printf("%d: %s\n", favs[i].id, favs[i].comando);
    }
}

void eliminarParFavs(ComFavorito *favs, int id1, int id2) {
    int dif_abs = (id1 - id2) * (((id1 - id2) > 0) - (id1 - id2) < 0); //algoritmo para sacar el valor absoluto
    int menor;
    if (id1 > id2) {
        menor = id1;
    } else menor = id2;

    int i = 0;
    for (i = menor; i <= menor + dif_abs; i++) {
        favs[i].eliminado = true;
    }

}

void borrarFavs(ComFavorito **favs, int *num_favs) {
    ComFavorito *new_favs = malloc(sizeof(ComFavorito) * MAX_FAVS);;
    if (new_favs == NULL) {
        fprintf(stderr, "Error: No se pudo asignar memoria para los nuevos favoritos.\n");
        return;
    }
    free(*favs);
    favs = new_favs;
    *num_favs = 0;
}


