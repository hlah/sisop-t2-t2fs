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
// inicializa t2fs, retorna 0 se sucedidio, outro valor se erro
int t2fs_init();
// lê superbloco, retorna 0 se sucedidio, outro valor se erro
int t2fs_read_superblock(struct t2fs_superbloco* superbloco_p);


/////// variaveis internas da t2fs ////////
// indica se t2fs foi inicializado
extern int t2fs_initialized;
// buffer superbloco
extern struct t2fs_superbloco t2fs_superbloco_info;
// caminho do diretório atual
extern char t2fs_cwd_path[256];
// cluster do diretório atual
extern unsigned int t2fs_cwd_cluster_num;
// extern unsigned char current_path[SECTOR_SIZE];
// id do proximo FILE2
// extern FILE2 next_FILE2 = 0;

#endif
