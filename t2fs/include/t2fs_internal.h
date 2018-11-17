#ifndef __t2fs_internal__
#define __t2fs_internal__

#include "t2fs.h"
#include <stdlib.h>

///// deifiniçoes
#define MAXIMUM_OPEN_DIRS 200
#define MAXIMUM_OPEN_FILES 200

#define MAXIMUM_PATH_LEN 256

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

typedef struct {
    DWORD dir_cluster;   /* Cluster do diretório que contém o arquivo */
    struct t2fs_record *file_record;   /* Ponteiro para registro de diretorio do arquivo */
    DWORD current_pointer;   /* Posição corrente do arquivo */
} t2fs_file_data;


/////// funções internas da biblioteca ///////////
// inicializa t2fs, retorna 0 se sucedidio, outro valor se erro
int t2fs_init();
// lê superbloco, retorna 0 se sucedidio, outro valor se erro
int t2fs_read_superblock(struct t2fs_superbloco* superbloco_p);
// lê cluster, retorna ponteiro para buffer (deve ser desalocado por quem chamou), NULL se erro
void* t2fs_read_cluster( int cluster);
// escreve cluster, retorna 0 se sucesso, outro valor se erro (em casoe de erro o cluster pode ficar corrompido)
int t2fs_write_cluster(int cluster, void* buffer);
// abre diretório a partir do seu cluster, retorna handler ou -1 em erro
int t2fs_opendir_from_cluster(int cluster);
// retrona registro de diretório em 'path' a partir do diretório em cluster como referencia
struct t2fs_record t2fs_find_file(int cluster, const char* path);
// retorna posição de cluster vazio ou -1 se não houver
int t2fs_get_free_cluster();
// libera clusters a partir do cluster dado retorna 0 se sucesso, outro valor se erro
int t2fs_free_clusters_from( int cluster );
// retorna posição libre em diretório, ou -1 se diretório cheio; entrada: buffer de cluster do diretório
int t2fs_get_dir_empty_pos( void* cluster_data );
// checa se diretório está vazio (apenas '.' e '..') (1 = vazio, 0 = não vazio)
int t2fs_is_dir_empty( void* cluster_data );
// separa nome de arquivo do resto do caminho (pathname não é preservado);
void t2fs_split_path( char* pathname, char** path, char** name );
// obtem caminho absoluto de arquivo a partir do seu cluster
void t2fs_dir_abs_path( int cluster, char * path_name ); 

/////// VARIAVEIS INTERNAS ////////

// indica se t2fs foi inicializado
extern int t2fs_initialized;
// buffer superbloco
extern struct t2fs_superbloco t2fs_superbloco_info;
// caminho do diretório atual
extern char t2fs_cwd_path[MAXIMUM_PATH_LEN+1];
// cluster do diretório atual
extern unsigned int t2fs_cwd_cluster_num;

// numero máximo de arquivos em diretório
extern unsigned int t2fs_maximum_files_in_directory;

// array de diretórios abertos (posiçoes não usadas: directory_cluster_data = NULL)
extern t2fs_directory_data t2fs_open_directories[MAXIMUM_OPEN_DIRS];

// array de arquivos abertos (posiçoes não usadas: record = NULL)
extern t2fs_file_data t2fs_open_files[MAXIMUM_OPEN_FILES];

#endif
