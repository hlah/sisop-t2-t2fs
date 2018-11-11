#include "t2fs_internal.h"
#include "apidisk.h"

#include <assert.h>

int t2fs_free_clusters_from( int cluster ) {
	assert( t2fs_initialized == 1 );

	unsigned int sector_buffer[SECTOR_SIZE];
	int current_sector = -1;

	// libera cluster
	while( cluster != (int)0xFFFFFFFF) {

		// obtém setor e offset
		int sector = cluster / (SECTOR_SIZE / 4); 
		int offset = cluster % (SECTOR_SIZE / 4); 

		// le se em novo setor
		if( sector != current_sector ) {
			if( read_sector(sector + t2fs_superbloco_info.pFATSectorStart, (unsigned char*)sector_buffer) != 0 ) {
				return -1;
			}
			current_sector = sector;
		}

		// atualiza cluster
		cluster = sector_buffer[offset];

		// escreve como livre
		sector_buffer[offset] = 0;
		if( write_sector(sector + t2fs_superbloco_info.pFATSectorStart, (unsigned char*)sector_buffer) != 0 ) {
			return -1;
		}


	}

	return 0;
}
