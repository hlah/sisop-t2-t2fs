#include "t2fs_internal.h"

// indica se t2fs foi inicializado
int t2fs_initialized = 0;
// buffer superbloco
struct t2fs_superbloco t2fs_superbloco_info;
// caminho do diretório atual
char t2fs_cwd_path[MAXIMUM_PATH_LEN+1];
// cluster do diretório atual
unsigned int t2fs_cwd_cluster_num;

// numero máximo de arquivos em diretório
unsigned int t2fs_maximum_files_in_directory;

// array de diretórios abertos (posiçoes não usadas: directory_cluster_data = NULL)
t2fs_directory_data t2fs_open_directories[MAXIMUM_OPEN_DIRS];

// array de arquivos abertos (posiçoes não usadas: record = NULL)
t2fs_file_data t2fs_open_files[MAXIMUM_OPEN_FILES];
