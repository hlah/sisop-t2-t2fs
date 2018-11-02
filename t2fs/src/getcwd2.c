#include "t2fs_internal.h"

#include <string.h>

int getcwd2 (char *pathname, int size) {
	t2fs_init();

	if( size < strlen( t2fs_cwd_path ) + 1 ) {
		return -1;
	}

	strcpy( pathname, t2fs_cwd_path );

	return 0;
}
