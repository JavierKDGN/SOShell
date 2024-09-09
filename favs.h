#ifndef FAVS_H
#define FAVS_H

#include <stdbool.h>
#include "shell.h"

#define MAX_FAVS 1024

// Struct que define un comando preferido
typedef struct {
    char comando[MAX_COMANDOS];
    int id;
    bool eliminado;
} ComFavorito;

typedef enum {
    crear, //Listo
    mostrar, //Listo
    eliminar, //Listo
    buscar, //Lissto
    borrar, //Listo
    ejecutar,
    cargar, //Listo
    guardar, //Listo
    undefined //Comando no reconodio
} argumentos_favs;

//Prototipos de funciones
argumentos_favs obtenerArgumento(const char *argumento);
void procesarFavs(ComFavorito *favs, int *num_favs, const char *comando);
bool estaEnFavs(ComFavorito *favs, int *num_favs, const char *comando);
void addFav(ComFavorito *favs, int *num_favs, const char *comando);
void mostrarFavs(ComFavorito *favs, int *num_favs);
void buscarStringEnFavs(ComFavorito *favs, int *num_favs, const char* str);
void renumerarFavs(ComFavorito *favs, int *numFavs);
void eliminarParFavs(ComFavorito *favs, int *num_favs, int id1, int id2);
void borrarFavs(ComFavorito **favs, int *num_favs);
void ejecutarFavs(ComFavorito *favs, int id);
void crearArchivoFavs(const char *ruta);
void guardarFavs(ComFavorito *favs, int *num_favs);
void cargarFavs(ComFavorito *favs, int *num_favs);

#endif // FAVS_H