#include "t2fs_internal.h"

#include <string.h>
#include <assert.h>

#include <stdio.h>

void add_path_node_name( int cluster, int parent_cluster, char * path_name ) {
	
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
		add_path_node_name( parent_cluster, next_parent, path_name );
	}

	// concatena nome
	strcat( path_name, name );
	strcat( path_name, "/" );

}

// obtem caminho absoluto de arquivo a partir do seu cluster
void t2fs_dir_abs_path( int cluster, char * path_name ) {
	strcpy( t2fs_cwd_path, "/" );
	if( cluster != t2fs_superbloco_info.RootDirCluster ) {
		add_path_node_name( cluster,  -1, path_name );
	}
}

