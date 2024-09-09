#ifndef SHELL_H
#define SHELL_H

#define MAX_ARGUMENTOS 1024 //cantidad de arg max
#define MAX_PIPES 100 //cantidad de pipes max
#define MAX_COMANDOS 1024

int ejecutarComando(char *comando); //declaracion de la funcion
void crearRecordatorio(char *comando);

#endif