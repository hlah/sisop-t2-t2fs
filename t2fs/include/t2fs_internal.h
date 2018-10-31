#ifndef __t2fs_internal__
#define __t2fs_internal__

#include "t2fs.h"
#include <stdlib.h>

///// deifiniçoes

typedef struct t2fs_record DESCRIPTOR;

typedef struct {
    DESCRIPTOR   *buffer;   /* Ponteiro para buffer contendo o descritor do arquivo */
    int          current_pointer;   /* Posição corrente do arquivo */
} HANDLE;

/*
typedef struct {
    HANDLE *ant;
    HANDLE handle;
    HANDLE *pos;
} NODE_HANDLE;

typedef *NODE_HANDLE P_NODE_HANDLE;
*/

/////// funções internas da biblioteca ///////////
// inicializa t2fs
int t2fs_init();
// lê superbloco
int t2fs_read_superblock(struct t2fs_superbloco* superbloco_p);


/////// variaveis internas da t2fs ////////
// indica se t2fs foi inicializado
// extern int t2fs_initialized = 0;
// buffer superbloco
// extern struct t2fs_superbloco superbloco;
// caminho do CP
// extern char *CP;
// ponteiro para buffer contendo as entradas do CP atual
// extern unsigned char current_path[SECTOR_SIZE];
// id do proximo FILE2
// extern FILE2 next_FILE2 = 0;

#endif
