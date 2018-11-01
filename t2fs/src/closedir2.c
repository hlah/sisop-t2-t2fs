#include "t2fs_internal.h"

int closedir2 (DIR2 handle) {
	t2fs_init();

	// checa se handle é válido
	if( handle < 0 || handle >= MAXIMUM_OPEN_DIRS  ) {
		return -1;
	}

	// checa se posição é usada
	if( t2fs_open_directories[handle].directory_cluster_data == NULL ) {
		return -1;
	}

	// libera memoria e reseta posição
	free(t2fs_open_directories[handle].directory_cluster_data);
	t2fs_open_directories[handle].current_file = 0;

	return 0;
}
