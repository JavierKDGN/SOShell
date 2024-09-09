#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // llamadas (fork, execvp)
#include <string.h>
#include <sys/wait.h> // para esperar la finalización de procesos hijos (wait)
#include "shell.h"
#include "favs.h"

int ejecutarComando(char *comando) {
    char *comandos[MAX_PIPES + 1]; //n+1, porque para pipes usamos n+1 division de comandos, si tenemos dos pipes hay 3 comandos entre ellos
    char *comandoActual; 
    int numPipes = 0;
    int i;
    int error = 0; //si ocurre un error terminaremos la funcion

    comandos[numPipes] = strtok(comando, "|"); // encontrtamos el primer comando del pipe y lo guardamos
    while (comandos[numPipes] != NULL) { //mientras tengamos elementos en la lista de comandos
        numPipes++; //aumentamos num de pipes
        if (numPipes > MAX_PIPES) { //si el numero de pipes supera el maximo que asignamos
            fprintf(stderr, "Error: supera la cantidad de pipes\n"); //damos error
            error = 1;//marcamos error como 1, true, asi que debemos terminar la funcion
            break;
        }
        comandos[numPipes] = strtok(NULL, "|"); //añadimos el siguiente comando al pipe
    }

    if (error == 1)
    {
        return -1; //se sale de la funcion
    }
    
    int fd[2]; //fd, file desvcriptor almacenara dos valores, 0 y 1, pues fd[0] lectura y fd[1] escritura
    int entrada = 0; //inicializamos el descriptor de entrada en 0

    for (i = 0; i < numPipes; i++) { //un for que recorra los pipes que conseguimos antes
        pipe(fd); //creamos el pipe , fd tiene dentro 0 y 1 lectura y escritura
        pid_t pid = fork();  //pid_t identificador de proceso, pid identifica un proceso y fork el proceso padre crea un hijo

        if (pid == 0) { //si el proceso hijo es 0, es decir, lectura
        //dup 2 viene de <unistd.h>, permite duplicar un descriptor de archivo existente y redirigirlo a un nuevo descriptor de archivo especificado
            dup2(entrada, 0); //duplicamos el contenido de entrada a 0 (stdin)
            if (i < numPipes - 1) { //si no estamos en el ultimo comando 
                dup2(fd[1], 1); // la salida del descriptor de escritura se dirige a escritura(stdout)
            }
            close(fd[0]); //cerramos el descriptor de lectura

            // dividir el comando por argumentos
            char *argumentos[MAX_ARGUMENTOS]; //un arreglo para almacenar los argumentos con tamaño MAX_ARGUMENTOS
            char *argumento = strtok(comandos[i], " ");//usamos el espacio para dividir los argumentps
            int indiceArgumento = 0; //aqui iremos viendo el indice de los argumentos
            while (argumento != NULL) { //mientras el argumento no sea nulo
                argumentos[indiceArgumento++] = argumento; //añadimos cada argumento al arreglo argumentos, y aumentamos el indice
                argumento = strtok(NULL, " "); //obtenemos el siguiente argumento
            }
            argumentos[indiceArgumento] = NULL; //el ultimo argumento debe ser null(para usar execvp)

            //execvp es una función que reemplaza el proceso actual con un nuevo programa.
            execvp(argumentos[0], argumentos);  //ejecutamos el primer comando
            perror("execvp error");
            exit(EXIT_FAILURE);
        } else if (pid > 0) {
            int status;
            waitpid(pid, &status, 0);  // Espera al proceso hijo y guarda el estado
            if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
                // Si el hijo terminó con error, devolver -1
                return -1;
            }
            close(fd[1]);
            entrada = fd[0];
        } else {
            perror("fork error");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}

void crearRecordatorio(char *comando) {
    char *args[MAX_ARGUMENTOS]; //buffer de argumentos
    char *aux_token = strtok(comando, " "); //separa comando en sus argumentos
    int aux_contador = 0;

    while (aux_token != NULL && aux_contador < MAX_ARGUMENTOS - 1) {
        args[aux_contador++] = aux_token;
        aux_token = strtok(NULL, " ");
    }
    args[aux_contador] = NULL; //fijamos la ultima palabra de args como null

    if (aux_contador < 4) {
        //Formato: set recordatorio segundos "mensaje"
        fprintf(stderr, "Error: debe incluir un numero y un mensaje despues de set recordatorio\n");
        return;
    }
    int segundos = atoi(args[2]);
    const char *mensaje = args[3];

    //terminamos de parsear el comando
    pid_t pid = fork();
    if (pid == 0) {
        sleep(segundos);
        printf("\n Recordatorio: %s\n", mensaje);
        exit(0); //cerramos el hijo encaragdo del recordatorio
    } else if (pid < 0) {
        perror("fork error");
    }


}