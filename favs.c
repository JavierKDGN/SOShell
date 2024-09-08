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

    if (!estaEnFavs(favs, num_favs, comando)) { //FALTA IMPLEMENTAR
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

void eliminarParFavs(ComFavorito *favs, int id1, int id2) {
    int dif_abs = (id1 - id2) * (((id1 - id2) > 0) - (id1 - id2) < 0); //algoritmo para sacar el valor absoluto
    int menor;
    if (id1 < id2) {
        menor = id1;
    } else menor = id2;

    int i = 0;
    for (i = menor - 1; i <= menor + dif_abs; i++) {
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

    FILE *aux_data = fopen("shell_data.txt", "w"); //Archivo auxiliar para recordar ruta
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

void guardarFavs(ComFavorito *favs, int num_favs) {}

void cargarFavs(ComFavorito *favs, int *num_favs) {
    FILE *aux_data = fopen("shell_data,txt", "r");
    if (aux_data == NULL) {
        fprintf(stderr, "Error: no se pudo abrir archivo\n");
        return;
    }

    const char ruta[1024]; //variable para leer ruta guardada

    if (fgets(ruta, sizeof(ruta), aux_data) == NULL) {
        fprintf(stderr, "Error: No se ha detectado una ruta, cree una con el comando 'favs crear'\n");
        fclose(aux_data);
        return;
    }
    fclose(aux_data);

    FILE *arch = fopen(ruta, "r");
    if (arch == NULL) {
        fprintf(stderr, "Error: no se pudo abrir archivo\n");
        return;
    }
    
    *num_favs = 0;
    char comando[MAX_COMANDOS];
    int id;

    //el fscanf lee cada linea
    while (fscanf(arch, "%d %[^\n]s, &id, comando") != EOF) {
        if (*num_favs >= MAX_FAVS) {
            fprintf(stderr, "Error: No se pudo cargar favoritos");
            return;
        } else {
            favs[*num_favs].id = id;
            strncpy(favs[*num_favs].comando, comando, MAX_COMANDOS - 1);
            favs[*num_favs].comando[MAX_COMANDOS - 1] = '\0';
            favs[*num_favs].eliminado = false;
            (*num_favs)++;
        }
    }

    fclose(arch);
    mostrarFavs(favs, num_favs);
}



/* TEST GENERADO POR COPILOT, EN LA COMMIT ACTUAL FUNCIONA CORRECTAMENTE
int main() {
    // Allocate memory for favorites
    ComFavorito *favs = malloc(sizeof(ComFavorito) * MAX_FAVS);
    if (favs == NULL) {
        fprintf(stderr, "Error: No se pudo asignar memoria para los favoritos.\n");
        return 1;
    }
    int num_favs = 0;

    // Add some favorites
    addFav(favs, &num_favs, "comando1");
    addFav(favs, &num_favs, "comando2");
    addFav(favs, &num_favs, "comando3");

    // Show favorites
    printf("Lista de favoritos:\n");
    mostrarFavs(favs, &num_favs);

    // Check if a command is in favorites
    const char *comando = "comando2";
    if (estaEnFavs(favs, &num_favs, comando)) {
        printf("El comando '%s' está en la lista de favoritos.\n", comando);
    } else {
        printf("El comando '%s' no está en la lista de favoritos.\n", comando);
    }

    // Clear favorites
    eliminarParFavs(favs,2,3);
    mostrarFavs(favs,&num_favs);

    // Clean up
    free(favs);

    return 0;
}

*/