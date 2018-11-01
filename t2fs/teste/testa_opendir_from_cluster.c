#include "t2fs_internal.h"

#include <stdio.h>

int main() {

	if ( t2fs_init() != 0 ) {
		printf("Erro ao inicializar o sistema de arquivos.\n");
	}

	printf("id: %c%c%c%c\n", t2fs_superbloco_info.id[0], t2fs_superbloco_info.id[1], t2fs_superbloco_info.id[2] ,t2fs_superbloco_info.id[3]);
	printf("version: %x\n",  t2fs_superbloco_info.version);
	printf("tamanho do t2fs_superbloco_info: %u setores\n", t2fs_superbloco_info.superblockSize);
	printf("tamanho do disco: %u bytes\n", t2fs_superbloco_info.DiskSize);
	printf("setores: %u \n", t2fs_superbloco_info.NofSectors);
	printf("setores por cluster: %u \n", t2fs_superbloco_info.SectorsPerCluster);
	printf("setor inicial da FAT: %u \n", t2fs_superbloco_info.pFATSectorStart);
	printf("cluster do diretório raiz: %u \n", t2fs_superbloco_info.RootDirCluster);
	printf("setor inicial da area de dados (cluster 0): %u \n", t2fs_superbloco_info.DataSectorStart);

	// abre diretório raiz
	DIR2 root = t2fs_opendir_from_cluster( t2fs_superbloco_info.RootDirCluster );

	printf("\nConteudo do diretorio raiz:\n");

	if( root < 0 ) {
		printf("Erro ao abrir diretório\n");
	} else {
		DIRENT2 dir_entry;
		while( readdir2( root, &dir_entry ) == 0 ) {
			printf("%d\t%s\t%d\n", dir_entry.fileType, dir_entry.name, dir_entry.fileSize);
		}
		closedir2(root);
	}



}
