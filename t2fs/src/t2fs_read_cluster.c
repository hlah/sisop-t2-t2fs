#include "t2fs_internal.h"

#include "apidisk.h"
#include <assert.h>

// lê cluster, retorna ponteiro para buffer (deve ser desalocado por quem chamou), NULL se erro
void* t2fs_read_cluster(int cluster) {
	int i;
	unsigned int cluster_first_sector;
	assert( t2fs_initialized == 1 );

	// TODO: checar cluster máximo
	
	// alloca buffer
	void* buffer = malloc( SECTOR_SIZE * t2fs_superbloco_info.SectorsPerCluster );

	if( buffer != NULL ) {

		// calcula setor inicial do cluster
		cluster_first_sector = t2fs_superbloco_info.DataSectorStart + cluster * t2fs_superbloco_info.SectorsPerCluster;

		

		// lê cluster setor por setor
		for( i=0; i<t2fs_superbloco_info.SectorsPerCluster; i++ ) {
			if( read_sector( cluster_first_sector + i, (unsigned char*)buffer + i * SECTOR_SIZE ) != 0 ) {
				// libera buffer se erro na leitura do setor
				free(buffer);
				buffer = NULL;
				break;
			}  
		}
	}
	

	return buffer;
}
