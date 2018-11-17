#include "t2fs_internal.h"
#include "apidisk.h"

#include <string.h>

int delete2(char* filename) {
	t2fs_init();

    FILE2 handle;
    int i;
	unsigned int sector_buffer [SECTOR_SIZE / 4];
    unsigned int sector;
    unsigned int sector_offset;
    void* cluster_data;
	unsigned int cluster_registry = 0;
    // struct t2fs_record record;

    /*

    // encontra registro do arquivo
    record = t2fs_find_file(t2fs_cwd_cluster_num, filename)
    if (record.TypeVal != TYPEVAL_REGULAR)
        return -1;

    // verifica se o arquivo já está aberto
    for (i = 0; i < MAXIMUM_OPEN_FILES; i++)
        if (t2fs_open_files[handle].file_record->name == record.name)
            break;

    // se arquivo já está aberto, é associado a variavel handle. Se não está aberto, é aberto.
    if (i < MAXIMUM_OPEN_FILES)
        handle = i + 1;
    else handle = open2(filename);

    */

    // abre arquivo
    handle = open2(filename);
    handle = handle - 1;
    if (t2fs_open_files[handle].file_record->TypeVal != TYPEVAL_REGULAR)
        return -1;

    // verifica se o arquivo já está aberto com outro handle. Se já está, arquivo é fechado
    for (i = 0; i < MAXIMUM_OPEN_FILES; i++)
        if (t2fs_open_files[handle].file_record->name == t2fs_open_files[i].file_record->name && t2fs_open_files[handle].dir_cluster == t2fs_open_files[i].dir_cluster && handle != i)
            close2(i + 1);

    // desaloca clusters do arquivo
    seek2(handle + 1, 0);
    truncate2(handle + 1);

    // desaloca primeiro cluster
    sector = t2fs_open_files[handle].file_record->firstCluster / (SECTOR_SIZE / 4);
    sector_offset = t2fs_open_files[handle].file_record->firstCluster % (SECTOR_SIZE / 4);
    if (read_sector(sector + t2fs_superbloco_info.pFATSectorStart, (unsigned char*) sector_buffer) != 0)
        return -1;
    sector_buffer[sector_offset] = 0;
    if (write_sector(sector + t2fs_superbloco_info.pFATSectorStart, (unsigned char*) sector_buffer) != 0)
        return -1;

    // procura registro do arquivo no seu diretório
	cluster_data = t2fs_read_cluster( t2fs_open_files[handle].dir_cluster );
	while( strcmp(((struct t2fs_record*)cluster_data+cluster_registry)->name, t2fs_open_files[handle].file_record->name) != 0) {
		cluster_registry++;
	}

	// libera registro e escreve no disco
    ((struct t2fs_record*)cluster_data+cluster_registry)->TypeVal = TYPEVAL_INVALIDO;
	if (t2fs_write_cluster(t2fs_open_files[handle].dir_cluster, cluster_data) != 0)
        return -1;
    free(cluster_data);

	return 0;
}
