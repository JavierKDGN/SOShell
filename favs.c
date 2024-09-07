#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_FAVS 1024
#define MAX_COMANDOS 1024

//Struct que define un comando preferido
typedef struct {
    char comando[MAX_COMANDOS];
    size_t id;
} ComFavorito;

bool estaEnFavs(ComFavorito *favs, size_t *num_favs, const char *comando) {

}

void addFav(ComFavorito *favs, size_t *num_favs, const char *comando) {
    if (*num_favs >= MAX_FAVS) {
        fprintf(stderr, "Error: lista de favoritos llena (%d de %d).\n", num_favs, MAX_FAVS);
        return;
    }

    if (!estaEnFavs(favs, *num_favs, comando)) { //FALTA IMPLEMENTAR

    }
}

void mostrarFavs(ComFavorito *favs, size_t *num_favs) {
    for (int i = 0; i < *num_favs; i++) {
        printf("%d: %s\n", favs[i].id, favs[i].comando);
    }
}
