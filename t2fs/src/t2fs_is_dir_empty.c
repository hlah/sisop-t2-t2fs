#include "t2fs_internal.h"

#include <string.h>

// retorna posição libre em diretório, ou -1 se diretório cheio; entrada: buffer de cluster do diretório
int t2fs_is_dir_empty( void* cluster_data ) {

	// procura por espaço vazio
	int cluster_registry = 0;
	while( cluster_registry < t2fs_maximum_files_in_directory && (
			((struct t2fs_record*)cluster_data+cluster_registry)->TypeVal == TYPEVAL_INVALIDO  ||
			strcmp(((struct t2fs_record*)cluster_data+cluster_registry)->name, "." ) == 0 ||
			strcmp(((struct t2fs_record*)cluster_data+cluster_registry)->name, ".." ) == 0 
			)
			) {
		cluster_registry++;
	}
	// diretório vazio, retorna 1
	if( cluster_registry == t2fs_maximum_files_in_directory ) {
		return 1;
	}

	return 0;
}
