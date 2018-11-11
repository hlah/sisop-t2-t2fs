#include "t2fs_internal.h"

#include <string.h>
#include <assert.h>

int rmdir2 (char *pathname) {
	t2fs_init();

	// procura caminho
	struct t2fs_record registro = t2fs_find_file(t2fs_cwd_cluster_num, pathname);
	if( registro.TypeVal != TYPEVAL_DIRETORIO ) {
		return -1;
	}

	// lê cluster
	void* cluster_data = t2fs_read_cluster( registro.firstCluster );
	if( cluster_data == NULL ) {
		return -1;
	}

	// checa se é vazio
	if( t2fs_is_dir_empty( cluster_data ) != 1 ) {
		free(cluster_data);
		return -1;
	}
	free(cluster_data);

	// obtem pai
	struct t2fs_record registro_pai = t2fs_find_file(registro.firstCluster, "..");
	assert( registro_pai.TypeVal == TYPEVAL_DIRETORIO );

	// não pode deletar diretório raiz ( checa verificando se o pai é o propio diretorio )
	if( registro_pai.firstCluster == registro.firstCluster ) {
		return -1;
	} 

	// libera cluster do diretório
	t2fs_free_clusters_from( registro.firstCluster );

	// lê cluster do pai
	cluster_data = t2fs_read_cluster( registro_pai.firstCluster );
	if( cluster_data == NULL ) {
		return -1;
	}

	// encotra e zera registro
	int cluster_registry = 0;
	while( cluster_registry < t2fs_maximum_files_in_directory &&
			strcmp(((struct t2fs_record*)cluster_data+cluster_registry)->name, registro.name) != 0) {
		cluster_registry++;
	}
	assert( cluster_registry != t2fs_maximum_files_in_directory );
	memset( (void*)((struct t2fs_record*)cluster_data+cluster_registry), 0, sizeof(struct t2fs_record) );
	// reescreve todo o diretorio
	if( t2fs_write_cluster( registro_pai.firstCluster, cluster_data ) != 0 ) {
		free(cluster_data);
		return -1;
	}

	free(cluster_data);
	return 0;
}
