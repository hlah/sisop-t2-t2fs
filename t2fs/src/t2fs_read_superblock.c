#include "apidisk.h"
#include "t2fs_internal.h"

#include <string.h>

int t2fs_read_superblock(struct t2fs_superbloco* superbloco_p) {
	unsigned char buffer[SECTOR_SIZE];
	// le primeiro bloco
	if( read_sector(0, buffer) != 0 ) {
		return -1;
	}
	// coloca na estrutura
	memcpy((void*)superbloco_p, buffer, sizeof(struct t2fs_superbloco));

	return 0;
}
