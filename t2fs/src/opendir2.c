#include "t2fs_internal.h"

DIR2 opendir2 (char *pathname) {
	t2fs_init();

	// checa se é o diretório root
	if( pathname[0] == '/' && pathname[1] == '\0' ) {
		return t2fs_opendir_from_cluster( t2fs_superbloco_info.RootDirCluster );
	}

	// pega registro do filepath
	struct t2fs_record registro = t2fs_find_file( t2fs_cwd_cluster_num, pathname );
	if( registro.TypeVal != TYPEVAL_DIRETORIO ) {
		return -1;
	}
	
	return t2fs_opendir_from_cluster( registro.firstCluster );
}
