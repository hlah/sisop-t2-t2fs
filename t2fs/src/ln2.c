#include "t2fs_internal.h"

#include <string.h> 
#include <assert.h>

int ln2(char *linkname, char *filename) {
	t2fs_init();

	char* path = NULL;
	char* name = NULL;

	char filename_abs[MAXIMUM_PATH_LEN+1];

	// procura arquivo
	struct t2fs_record filename_registro = t2fs_find_file( t2fs_cwd_cluster_num, filename );
	if( filename_registro.TypeVal == TYPEVAL_INVALIDO ) {
		return -1;
	}

	// copia filename
	char* filename_copy = malloc( strlen(filename)+1 );
	strcpy( filename_copy, filename );

	// checa se não é diretório
	if( filename_registro.TypeVal != TYPEVAL_DIRETORIO ) {
		// extrai path
		t2fs_split_path( filename_copy, &path, &name );
		int parent_cluster = t2fs_cwd_cluster_num;
		if( path != NULL ) {
			// procura path
			struct t2fs_record path_registro = t2fs_find_file( t2fs_cwd_cluster_num, path );
			assert( path_registro.TypeVal == TYPEVAL_DIRETORIO ); 
			parent_cluster = path_registro.firstCluster;
		} 
		// obtém caminho absoluto
		t2fs_dir_abs_path( parent_cluster, filename_abs );
		// concatena nome
		strcat( filename_abs, name );

	} else {
		// obtém caminho absoluto
		t2fs_dir_abs_path( filename_registro.firstCluster, filename_abs );
	}

	free( filename_copy );
	
	// checa tamanho do filename
	if( strlen(filename_abs) + 1 > t2fs_superbloco_info.SectorsPerCluster * SECTOR_SIZE ) {
		return -1;
	}

	/////////////////////////////////////////////

	// copia pathname
	char* linkname_copy = malloc( strlen(linkname)+1 );
	strcpy( linkname_copy, linkname );

	t2fs_split_path( linkname_copy, &path, &name );

	// obtem informações do caminho
	struct t2fs_record registro; 
	if( path != NULL ) {
		registro = t2fs_find_file(t2fs_cwd_cluster_num, path);
	} else {
		registro = t2fs_find_file(t2fs_cwd_cluster_num, ".");
	}
	if( registro.TypeVal != TYPEVAL_DIRETORIO ) {
		free(linkname_copy);
		return -1;
	}

	// checa se já existe	
	struct t2fs_record tmp_registro = t2fs_find_file( registro.firstCluster, name );
	if( tmp_registro.TypeVal != TYPEVAL_INVALIDO ) {
		free(linkname_copy);
		return -1;
	}

	// lê cluster do diretorio
	void* cluster_data = t2fs_read_cluster( registro.firstCluster );
	if( cluster_data == NULL ) {
		free(linkname_copy);
		return -1;
	}	

	int cluster_registry = t2fs_get_dir_empty_pos( cluster_data );
	// diretório cheio, retorna erro
	if( cluster_registry < 0 ) {
		free(linkname_copy);
		free(cluster_data);
		return -1;
	}

	// obtém novo cluster
	int new_cluster = t2fs_get_free_cluster();
	if( new_cluster < 0 ) {
		free(linkname_copy);
		free(cluster_data);
		return -1;
	}

	struct t2fs_record* novo_registro = (struct t2fs_record*)cluster_data + cluster_registry;
	novo_registro->TypeVal = TYPEVAL_LINK;
	strcpy(novo_registro->name, name);
	novo_registro->bytesFileSize = SECTOR_SIZE * t2fs_superbloco_info.SectorsPerCluster;
	novo_registro->clustersFileSize = 1;
	novo_registro->firstCluster = new_cluster;

	// reescreve todo o diretorio
	if( t2fs_write_cluster( registro.firstCluster, cluster_data ) != 0 ) {
		free(linkname_copy);
		free(cluster_data);
		return -1;
	}

	// copia filename para o buffer
	strcpy( (char*) cluster_data, filename_abs );
	// escreve cluster do link
	if( t2fs_write_cluster( new_cluster, cluster_data ) != 0 ) {
		free(linkname_copy);
		free(cluster_data);
		return -1;
	}


	free(linkname_copy);
	free(cluster_data);
	return 0;
}
