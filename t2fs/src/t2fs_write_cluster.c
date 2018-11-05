#include "t2fs_internal.h"

#include "apidisk.h"
#include <assert.h>

// escreve cluster, retorna 0 se sucesso, outro valor se erro (em casoe de erro o cluster pode ficar corrompido)
int t2fs_write_cluster(int cluster, void* buffer) {
	int i;
	unsigned int cluster_first_sector;
	assert( t2fs_initialized == 1 );
	assert( buffer != NULL );

	// TODO: checar cluster máximo
	
	// calcula setor inicial do cluster
	cluster_first_sector = t2fs_superbloco_info.DataSectorStart + cluster * t2fs_superbloco_info.SectorsPerCluster;

	// escreve cluster setor por setor
	for( i=0; i<t2fs_superbloco_info.SectorsPerCluster; i++ ) {
		if( write_sector( cluster_first_sector + i, (unsigned char*)buffer + i * SECTOR_SIZE ) != 0 ) {
			// libera buffer se erro na leitura do setor
			return -1;
		}  
	}

	return 0;
}

