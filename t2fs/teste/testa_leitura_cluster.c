
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

	printf("\n");

	printf("Numero maximo de arquivos por diretorio: %d\n", t2fs_maximum_files_in_directory);

	printf("\n");

	printf("Setor do diretorio atual: %d\n", t2fs_cwd_cluster_num);
	printf("Caminho do diretorio atual: '%s'\n", t2fs_cwd_path);

	printf("\n");

	printf("Lendo diretório raiz...\n");

	void* cluster_data = t2fs_read_cluster( t2fs_superbloco_info.RootDirCluster );

	if( cluster_data != NULL ) {

		struct t2fs_record* directory_record = (struct t2fs_record*)cluster_data;
		int i = 0;
		while( i < t2fs_maximum_files_in_directory ) {
			if( directory_record->TypeVal != 0 ) {
				printf("Entry: %s\n", directory_record->name);
			}
			i++;
			directory_record += 1;
		}

		free(cluster_data);
	}





}
