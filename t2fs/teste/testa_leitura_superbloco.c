#include "t2fs_internal.h"

#include <stdio.h>

int main() {

	struct t2fs_superbloco superbloco;

	if( t2fs_read_superblock(&superbloco) == 0 ) {
		printf("id: %c%c%c%c\n", superbloco.id[0], superbloco.id[1], superbloco.id[2] ,superbloco.id[3]);
		printf("version: %x\n",  superbloco.version);
		printf("tamanho do superbloco: %u setores\n", superbloco.superblockSize);
		printf("tamanho do disco: %u bytes\n", superbloco.DiskSize);
		printf("setores: %u \n", superbloco.NofSectors);
		printf("setores por cluster: %u \n", superbloco.SectorsPerCluster);
		printf("setor inicial da FAT: %u \n", superbloco.pFATSectorStart);
		printf("cluster do diretório raiz: %u \n", superbloco.RootDirCluster);
		printf("setor inicial da area de dados (cluster 0): %u \n", superbloco.DataSectorStart);
	}



}
