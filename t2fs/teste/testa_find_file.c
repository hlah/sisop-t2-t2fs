#include "t2fs_internal.h"

#include <stdio.h>

int main() {

	if ( t2fs_init() != 0 ) {
		printf("Erro ao inicializar o sistema de arquivos.\n");
	}

	struct t2fs_record registro = t2fs_find_file( t2fs_superbloco_info.RootDirCluster, "./dir1/" );

	if( registro.TypeVal == TYPEVAL_INVALIDO ) {
		printf("Não foi possivel encontrar arquivo.\n");
	} else {

		printf("Abrindo '%s':\n", registro.name);

		DIR2 dir1 = t2fs_opendir_from_cluster( registro.firstCluster );

		if( dir1 < 0 ) {
			printf("Erro ao abrir diretório\n");
		} else {
			DIRENT2 dir_entry;
			while( readdir2( dir1, &dir_entry ) == 0 ) {
				printf("%d\t%s\t%d\n", dir_entry.fileType, dir_entry.name, dir_entry.fileSize);
			}
			closedir2(dir1);
		}

	}



	return 0;
}
