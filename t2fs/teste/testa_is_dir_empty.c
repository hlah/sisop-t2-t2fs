#include "t2fs_internal.h"

#include <stdio.h>

int is_dir_empty( char * path ) {

	struct t2fs_record registro = t2fs_find_file( t2fs_superbloco_info.RootDirCluster, path );
	if( registro.TypeVal == TYPEVAL_INVALIDO ) {
		printf("Não foi possivel encontrar '%s'.\n", path);
		return 0;
	}

	void* cluster_data = t2fs_read_cluster( registro.firstCluster );
	if( cluster_data != NULL )  {
		return t2fs_is_dir_empty( cluster_data );
	} else {
		printf("Não foi possivel ler '%s'.\n", path);
		return 0;
	}

}


int main() {

	if ( t2fs_init() != 0 ) {
		printf("Erro ao inicializar o sistema de arquivos.\n");
		return 1;
	}



	if( is_dir_empty( "/dir1/" ) ) {
		printf( "dir1 is empty!\n" );
		printf(" blash\n");
	} else {
		printf( "dir1 is not empty!\n" );
	}

	printf("criando diretorio /dir1/newdir/...");
	mkdir2( "/dir1/newdir/" );
	printf("criado!\n");

	if( is_dir_empty( "/dir1/newdir/" ) ) {
		printf( "dir1/newdir is empty!\n" );
	} else {
		printf( "dir1/newdir is not empty!\n" );
	}


}
