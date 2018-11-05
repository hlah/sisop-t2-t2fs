#include "t2fs_internal.h"

#include <stdio.h>
#include <string.h>

int mkdir2 (char *pathname) {
	t2fs_init();

	char* path = NULL;
	char* name = NULL;

	// separa caminho do nome do novo diretório
	int i = 0;
	while( pathname[i] != '\0' ) {
		i++;
	}
	// pula ultimo '/'
	if( pathname[i-1] == '/' ) {
		pathname[i-1] = '\0';
		i-=2;
	}
	while( pathname[i] != '/' && i >= 0 ) {
		i--;
	}
	if( i >= 0 ) {
		pathname[i] = '\0';
		path = pathname;
		name = pathname + i + 1;
	} else {
		name = pathname;
	}

	// obtem informações do caminho
	struct t2fs_record registro; 
	if( path != NULL ) {
		registro = t2fs_find_file(t2fs_cwd_cluster_num, path);
	} else {
		registro = t2fs_find_file(t2fs_cwd_cluster_num, ".");
	}
	if( registro.TypeVal != TYPEVAL_DIRETORIO ) {
		return -1;
	}

	// checa se já existe	
	struct t2fs_record tmp_registro = t2fs_find_file( registro.firstCluster, name );
	if( tmp_registro.TypeVal != TYPEVAL_INVALIDO ) {
		return -1;
	}

	// lê cluster do diretorio
	void* cluster_data = t2fs_read_cluster( registro.firstCluster );
	if( cluster_data == NULL ) {
		return -1;
	}	

	// procura por espaço vazio
	int cluster_registry = 0;
	while( cluster_registry < t2fs_maximum_files_in_directory &&
			((struct t2fs_record*)cluster_data+cluster_registry)->TypeVal != TYPEVAL_INVALIDO ) {
		//printf("%d\n", cluster_registry);
		cluster_registry++;
	}
	// diretório cheio, retorna erro
	if( cluster_registry == t2fs_maximum_files_in_directory ) {
		free(cluster_data);
		return -1;
	}

	// obtém novo cluster
	int new_cluster = t2fs_get_free_cluster();
	if( new_cluster < 0 ) {
		free(cluster_data);
		return -1;
	}

	//printf("Criando pasta '%s' em posicao %d em cluster %d\n", name, cluster_registry, new_cluster);

	struct t2fs_record* novo_registro = (struct t2fs_record*)cluster_data + cluster_registry;
	novo_registro->TypeVal = TYPEVAL_DIRETORIO;
	strcpy(novo_registro->name, name);
	novo_registro->bytesFileSize = SECTOR_SIZE * t2fs_superbloco_info.SectorsPerCluster;
	novo_registro->clustersFileSize = 1;
	novo_registro->firstCluster = new_cluster;

	// reescreve todo o diretorio
	if( t2fs_write_cluster( registro.firstCluster, cluster_data ) != 0 ) {
		free(cluster_data);
		return -1;
	}

	// copia nova entrada
	struct t2fs_record novo_diretorio = *novo_registro;

	// renomeia pai e self
	strcpy( registro.name, ".." );
	strcpy( novo_diretorio.name, "." );

	// zera buffer 
	memset( cluster_data, 0, SECTOR_SIZE * t2fs_superbloco_info.SectorsPerCluster );
	
	// coloca pai e self
	((struct t2fs_record*)cluster_data)[0] = novo_diretorio;
	((struct t2fs_record*)cluster_data)[1] = registro;

	// escreve novo diretorio
	if( t2fs_write_cluster( new_cluster, cluster_data ) != 0 ) {
		free(cluster_data);
		return -1;
	}

	free(cluster_data);
	return 0;
}
