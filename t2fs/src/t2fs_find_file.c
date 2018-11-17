#include "t2fs_internal.h"

#include <string.h>
#include <assert.h>

#include <stdio.h>

// registro invalido
struct t2fs_record registro_invalido = {
	.TypeVal = TYPEVAL_INVALIDO,
	.name = {0},
	.bytesFileSize = 0,
	.clustersFileSize = 0,
	.firstCluster = 0
};

// retrona registro de diretório em 'path' a partir do diretório em cluster como referencia
struct t2fs_record t2fs_find_file(int cluster, const char* path) {
	assert( t2fs_initialized == 1 );

	// checa se é enereço absoluto
	if( path[0] == '/' ) {
		cluster = t2fs_superbloco_info.RootDirCluster;
		path = path+1;
	}
	// obtem nome do diretório base e remove do path
	char base_name[MAX_FILE_NAME_SIZE+1];
	int i = 0;
	while( path[i] != '/' && path[i] != '\0' ) {
		base_name[i] = path[i];
		i++;
	}
	base_name[i] = '\0';
	path += i;	// remove base
	if( path[0] == '/' ) {
		path++;	// remove '/' se existe
	}


	// lê cluster
	void* cluster_data = t2fs_read_cluster( cluster );

	// procura pela base
	int cluster_registry = 0;
	while( cluster_registry < t2fs_maximum_files_in_directory &&
			strcmp(((struct t2fs_record*)cluster_data+cluster_registry)->name, base_name) != 0) {
		cluster_registry++;
	}

	// se não achou, retorna erro
	if( cluster_registry == t2fs_maximum_files_in_directory ) {
		free(cluster_data);
		return registro_invalido;
	}

	// carrega informação do registro
	struct t2fs_record record;
	memcpy(&record, (struct t2fs_record*)cluster_data+cluster_registry, sizeof(struct t2fs_record)) ;
	free(cluster_data);

	// se é link, procura valor
	if( record.TypeVal == TYPEVAL_LINK ) {
		// TODO: obtém cluster do link, le path e obtém registro a partir dele
		cluster_data = t2fs_read_cluster( record.firstCluster );
		if( cluster_data == NULL ) {
			return registro_invalido;
		}
		record = t2fs_find_file( cluster, (char*) cluster_data );
	}

	// se path vazio, retorna registro
	if( path[0] == '\0' ) {
		// TODO: suporte a links para arquivos
		return record;
	}
	// checka se é diretório TODO: suporte a links para diretorios
	if( record.TypeVal != TYPEVAL_DIRETORIO ) {
		return registro_invalido;
	}
	// se não, procura recursivamente
	return t2fs_find_file( record.firstCluster, path );
}
