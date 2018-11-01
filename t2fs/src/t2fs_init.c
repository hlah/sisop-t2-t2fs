#include "t2fs_internal.h"

#include <string.h>

int t2fs_init(){
    if ( !t2fs_initialized ){

        // le informaçoes do superbloco
		if( t2fs_read_superblock(&t2fs_superbloco_info) != 0 ) {
			return -1;
		}

        // seta o current path para o diretorio raiz
		t2fs_cwd_cluster_num = t2fs_superbloco_info.RootDirCluster;
		strcpy(t2fs_cwd_path, "/");

        // carrega o diretorio raiz
        // TODO

        t2fs_initialized = 1;
    }
    return 0;
}
