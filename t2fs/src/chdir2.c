#include "t2fs_internal.h"

#include <assert.h>
#include <string.h>

void add_path_node_name( int cluster, int parent_cluster ) {
	
	// obtém cluster do pai, se não informado
	if( parent_cluster < 0 ) {
		struct t2fs_record registro = t2fs_find_file( cluster, ".." );
		assert( registro.TypeVal == TYPEVAL_DIRETORIO );
		parent_cluster = registro.firstCluster;
	}

	// lê cluster
	void* cluster_data = t2fs_read_cluster( parent_cluster );
	assert(cluster_data != NULL);
	// acha registro do cluster atual e cluster do pai
	int i = 0;
	int next_parent = -1;
	char* name = NULL;
	while( i < t2fs_maximum_files_in_directory && (name == NULL || next_parent == -1) ) {
		struct t2fs_record* registro_pointer = (struct t2fs_record*)cluster_data+i;
		if( registro_pointer->firstCluster == cluster ) {
			name = registro_pointer->name;
		}
		if( strcmp( registro_pointer->name, ".." ) == 0 ) {
			next_parent = registro_pointer->firstCluster;
		}
		i++;
	}
	// tem que ter achado o nome e próximo pai
	assert( next_parent != -1 );
	assert( name != NULL );

	// checa se atingiu raiz, se não adicoina nome recursivamenteo
	if( next_parent != parent_cluster ) {
		add_path_node_name( parent_cluster, next_parent );
	}

	// concatena nome
	strcat( t2fs_cwd_path, name );
	strcat( t2fs_cwd_path, "/" );

}

int chdir2 (char *pathname) {
	t2fs_init();

	// obtém cluster do novo cwd
	struct t2fs_record registro = t2fs_find_file( t2fs_cwd_cluster_num, pathname );
	if( registro.TypeVal != TYPEVAL_DIRETORIO ) {
		return -1;
	}

	// obtem 
	t2fs_cwd_cluster_num = registro.firstCluster;

	// atualiza path
	strcpy( t2fs_cwd_path, "/" );
	if( registro.firstCluster != t2fs_superbloco_info.RootDirCluster ) {
		add_path_node_name( registro.firstCluster, -1 );
	}

	/*
	int current_cluster = t2fs_cwd_cluster_num;
	strcpy( t2fs_cwd_path, "/" );
	while( current_cluster != t2fs_superbloco_info.RootDirCluster ) {
		// copia atual
		char tmp_str[MAXIMUM_PATH_LEN+1];
		strcpy( tmp_str, t2fs_cwd_path );
		// coloca barra
		strcpy( t2fs_cwd_path, "/" );
		// obtém nome da pasta atual
		struct t2fs_record registro = t2fs_find_file( current_cluster, "." );
		assert( registro.TypeVal == TYPEVAL_DIRETORIO );
		// coloca nome
		strcat( t2fs_cwd_path, registro.name );
		// coloca anteriores
		strcat( t2fs_cwd_path, tmp_str );

		// obtém nome da pasta pai
		registro = t2fs_find_file( current_cluster, ".." );
		assert( registro.TypeVal == TYPEVAL_DIRETORIO );
		current_cluster = registro.firstCluster;

	}
	*/
	
	return 0;
}
