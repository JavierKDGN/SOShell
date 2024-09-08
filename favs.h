#ifndef FAVS_H
#define FAVS_H

#include <stddef.h>
#include <stdbool.h>

#define MAX_FAVS 1024
#define MAX_COMANDOS 1024

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
    guardar //Listo
} argumentos_favs;

//Prototipos de funciones
bool estaEnFavs(ComFavorito *favs, int *num_favs, const char *comando);
void addFav(ComFavorito *favs, int *num_favs, const char *comando);
void mostrarFavs(ComFavorito *favs, int *num_favs);
void borrarFavs(ComFavorito **favs, int *num_favs);
void crearArchivoFavs(const char *ruta);
void guardarFavs(ComFavorito *favs, int *num_favs);
void cargarFavs(ComFavorito *favs, int *num_favs);

#endif // FAVS_H