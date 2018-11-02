#include "t2fs_internal.h"

#include <assert.h>

// abre diretório a partir do seu cluster, retorna handler ou -1 em erro
int t2fs_opendir_from_cluster(int cluster) {
	assert( t2fs_initialized == 1 );

	int dir_handler = 0;

	// acha posição livre no array de diretórios abertos
	while( dir_handler < MAXIMUM_OPEN_DIRS && t2fs_open_directories[dir_handler].directory_cluster_data != NULL) {
		dir_handler++;
	}

	// numero máximo de diretórios abertos atingido
	if( dir_handler == MAXIMUM_OPEN_DIRS ) {
		return -1;
	}

	// lê cluster
	t2fs_open_directories[dir_handler].directory_cluster_data = t2fs_read_cluster( cluster );
	// erro de leitura do cluster
	if( t2fs_open_directories[dir_handler].directory_cluster_data == NULL ) {
		return -1;
	}

	// seta como 0 a posição atual
	t2fs_open_directories[dir_handler].current_file = 0;

	return dir_handler;
}
