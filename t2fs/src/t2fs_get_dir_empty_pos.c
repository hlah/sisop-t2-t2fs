#include "t2fs_internal.h"

// retorna posição libre em diretório, ou -1 se diretório cheio; entrada: buffer de cluster do diretório
int t2fs_get_dir_empty_pos( void* cluster_data ) {

	// procura por espaço vazio
	int cluster_registry = 0;
	while( cluster_registry < t2fs_maximum_files_in_directory &&
			((struct t2fs_record*)cluster_data+cluster_registry)->TypeVal != TYPEVAL_INVALIDO ) {
		//printf("%d\n", cluster_registry);
		cluster_registry++;
	}
	// diretório cheio, retorna erro
	if( cluster_registry == t2fs_maximum_files_in_directory ) {
		return -1;
	}

	return cluster_registry;
}
