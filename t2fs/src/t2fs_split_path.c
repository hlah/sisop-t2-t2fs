#include "t2fs_internal.h"

// separa nome de arquivo do resto do caminho (pathname não é preservado);
void t2fs_split_path( char* pathname, char** path, char** name ) {
	*path = NULL;
	*name = NULL;
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
		*path = pathname;
		*name = pathname + i + 1;
	} else {
		*name = pathname;
	}
}

