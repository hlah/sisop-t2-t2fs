#include "t2fs_internal.h"

#include <string.h>

int close2 (FILE2 handle) {
	t2fs_init();

	void* cluster_data;
	unsigned int cluster_registry = 0;

	handle = handle - 1;

	// checa se handle é válido
	if( handle < 0 || handle >= MAXIMUM_OPEN_FILES  ) {
		return -1;
	}

	// checa se posição é usada
	if( t2fs_open_files[handle].file_record == NULL ) {
		return -1;
	}

    // procura registro do arquivo no seu diretório
	cluster_data = t2fs_read_cluster( t2fs_open_files[handle].dir_cluster );
	while( strcmp(((struct t2fs_record*)cluster_data+cluster_registry)->name, t2fs_open_files[handle].file_record->name) != 0) {
		cluster_registry++;
	}

	// salva dados atuais em disco
	((struct t2fs_record*)cluster_data+cluster_registry)->bytesFileSize = t2fs_open_files[handle].file_record->bytesFileSize;
	((struct t2fs_record*)cluster_data+cluster_registry)->clustersFileSize = t2fs_open_files[handle].file_record->clustersFileSize;
	((struct t2fs_record*)cluster_data+cluster_registry)->firstCluster = t2fs_open_files[handle].file_record->firstCluster;

	if (t2fs_write_cluster(t2fs_open_files[handle].dir_cluster, cluster_data) != 0)
        return -1;

    free(cluster_data);

	// reseta handle
	free(t2fs_open_files[handle].file_record);
	t2fs_open_files[handle].file_record = NULL;

	return 0;
}
