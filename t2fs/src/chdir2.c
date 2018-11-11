#include "t2fs_internal.h"

#include <assert.h>
#include <string.h>


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
	t2fs_get_abs_path( registro.firstCluster, t2fs_cwd_path );

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
