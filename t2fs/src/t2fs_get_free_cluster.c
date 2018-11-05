#include "t2fs_internal.h"
#include "apidisk.h"

#include <stdio.h>

#include <assert.h>

int t2fs_get_free_cluster() {
	assert( t2fs_initialized == 1 );
	int cluster = 2;
	int current_sector = -1;
	int found = 0;

	unsigned int sector_buffer[SECTOR_SIZE];

	while( 1 ) {
		// obtém setor e offset
		int sector = cluster / (SECTOR_SIZE / 4); 
		int offset = cluster % (SECTOR_SIZE / 4); 

		// atingiu final
		if( sector >= t2fs_superbloco_info.DataSectorStart - t2fs_superbloco_info.pFATSectorStart ) {
			break;
		}

		if( sector != current_sector ) {
			if( read_sector(sector + t2fs_superbloco_info.pFATSectorStart, (unsigned char*)sector_buffer) != 0 ) {
				return -1;
			}
			current_sector = sector;
		}

		//printf("sector: %d, offset: %d, content: %x\n", sector, offset, sector_buffer[offset]);

		// verifica se está livre
		if( sector_buffer[offset] == 0 ) {
			// escreve como ocupado
			sector_buffer[offset] -= 1;
			if( write_sector(sector + t2fs_superbloco_info.pFATSectorStart, (unsigned char*)sector_buffer) != 0 ) {
				return -1;
			}
			//printf("found -> sector: %d, offset: %d, content: %x\n", sector, offset, sector_buffer[offset]);
			found = 1;
			break;
		}

		cluster++;
	}

	if( !found ) {
		return -1;
	}

	return cluster;
}
