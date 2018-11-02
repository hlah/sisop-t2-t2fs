#ifndef __t2fs_internal__
#define __t2fs_internal__

#include "t2fs.h"
#include <stdlib.h>

///// deifiniçoes
#define MAXIMUM_OPEN_DIRS 200
#define MAXIMUM_OPEN_FILES 200

/*
typedef struct {
    HANDLE *ant;
    HANDLE handle;
    HANDLE *pos;
} NODE_HANDLE;

typedef *NODE_HANDLE P_NODE_HANDLE;
*/

// ESTRUTURAS

// handle de diretório:
typedef struct {
	char* directory_cluster_data;
	unsigned int current_file;
} t2fs_directory_data;


/////// funções internas da biblioteca ///////////
// inicializa t2fs, retorna 0 se sucedidio, outro valor se erro
int t2fs_init();
// lê superbloco, retorna 0 se sucedidio, outro valor se erro
int t2fs_read_superblock(struct t2fs_superbloco* superbloco_p);
// lê cluster, retorna ponteiro para buffer (deve ser desalocado por quem chamou), NULL se erro
void* t2fs_read_cluster( int cluster);
// abre diretório a partir do seu cluster, retorna handler ou -1 em erro
int t2fs_opendir_from_cluster(int cluster);
// retrona registro de diretório em 'path' a partir do diretório em cluster como referencia
struct t2fs_record t2fs_find_file(int cluster, const char* path);

/////// VARIAVEIS INTERNAS ////////

// indica se t2fs foi inicializado
extern int t2fs_initialized;
// buffer superbloco
extern struct t2fs_superbloco t2fs_superbloco_info;
// caminho do diretório atual
extern char t2fs_cwd_path[256];
// cluster do diretório atual
extern unsigned int t2fs_cwd_cluster_num;

// numero máximo de arquivos em diretório
extern unsigned int t2fs_maximum_files_in_directory;

// array de diretórios abertos (posiçoes não usadas: directory_cluster_data = NULL)
extern t2fs_directory_data t2fs_open_directories[MAXIMUM_OPEN_DIRS];

#endif
