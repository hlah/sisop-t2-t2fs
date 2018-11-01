#include "t2fs_internal.h"

#include <string.h>

int readdir2 (DIR2 handle, DIRENT2 *dentry) {
	t2fs_init();

	// checa se handle é válido
	if( handle < 0 || handle >= MAXIMUM_OPEN_DIRS  ) {
		return -1;
	}

	// checa se posição é usada
	if( t2fs_open_directories[handle].directory_cluster_data == NULL ) {
		return -1;
	}


	// aumenta posição até achar uma usada
	while( t2fs_open_directories[handle].current_file < t2fs_maximum_files_in_directory && 
			((struct t2fs_record*)(t2fs_open_directories[handle].directory_cluster_data)+t2fs_open_directories[handle].current_file)->TypeVal == 0 ) {
		t2fs_open_directories[handle].current_file++;
	}

	// checa se chegou no final
	if( t2fs_open_directories[handle].current_file == t2fs_maximum_files_in_directory ) {
		return -1;
	}

	// preenche estrutura de arquivo
	strcpy( dentry->name, ((struct t2fs_record*)(t2fs_open_directories[handle].directory_cluster_data)+t2fs_open_directories[handle].current_file)->name );
	dentry->fileType = ((struct t2fs_record*)(t2fs_open_directories[handle].directory_cluster_data)+t2fs_open_directories[handle].current_file)->TypeVal;
	dentry->fileSize = ((struct t2fs_record*)(t2fs_open_directories[handle].directory_cluster_data)+t2fs_open_directories[handle].current_file)->bytesFileSize;

	// incrementa proximo arquivo
	t2fs_open_directories[handle].current_file++;

	return 0;
}
