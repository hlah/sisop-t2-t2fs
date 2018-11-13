#include "t2fs_internal.h"
#include "apidisk.h"

int truncate2 (FILE2 handle) {
    t2fs_init();

    unsigned int sector_buffer [SECTOR_SIZE / 4];
    unsigned int current_position_cluster;
    // unsigned int cluster_offset;
    unsigned int next_cluster;
    unsigned int cluster_size = SECTOR_SIZE * t2fs_superbloco_info.SectorsPerCluster;
    unsigned int sector;
    unsigned int sector_offset;
    unsigned char cluster_buffer[cluster_size];
    unsigned int i;
    unsigned int removed_clusters = 0;

    handle = handle - 1;

    // checa se handle é válido
	if( handle < 0 || handle >= MAXIMUM_OPEN_FILES  ) {
		return -1;
	}

	// checa se posição é usada
	if( t2fs_open_files[handle].file_record == NULL ) {
		return -1;
	}

	// current pointer no final do arquivo, nada a apagar
	if (t2fs_open_files[handle].current_pointer == t2fs_open_files[handle].file_record->bytesFileSize)
        return 0;

    // cluster e offset da posição corrente (relativo ao arquivo)
	current_position_cluster = t2fs_open_files[handle].current_pointer / cluster_size;
	// cluster_offset = t2fs_open_files[handle].current_pointer % cluster_size;

    // setor da FAT e offset do primeiro cluster
    sector = t2fs_open_files[handle].file_record->firstCluster / (SECTOR_SIZE / 4);
    sector_offset = t2fs_open_files[handle].file_record->firstCluster % (SECTOR_SIZE / 4);

    // encontra primeiro cluster após o cluster onde está a posição corrente do arquivo
    for (i = 0; i <= current_position_cluster; i++) {
        if (read_sector(sector + t2fs_superbloco_info.pFATSectorStart, (unsigned char*) sector_buffer) != 0)
            return -1;
        next_cluster = sector_buffer[sector_offset];
        sector = next_cluster / (SECTOR_SIZE / 4);
        sector_offset = next_cluster % (SECTOR_SIZE / 4);
    }

    /*

    // zera tudo a partir do offset
    cluster_buffer = (unsigned char*) t2fs_read_cluster((int) next_cluster);
    for (i = cluster_offset; i < cluster_size; i++)
        cluster_buffer[i] = 0;
    if (t2fs_write_cluster((int) next_cluster, (void*) cluster_buffer) != 0)
        return -1;
    free(cluster_buffer);

    // encontra próximo cluster
    if (read_sector(sector + t2fs_superbloco_info.pFATSectorStart, (unsigned char*) sector_buffer) != 0)
        return -1;
    next_cluster = sector_buffer[sector_offset];
    sector = next_cluster / (SECTOR_SIZE / 4);
    sector_offset = next_cluster % (SECTOR_SIZE / 4);

    */

    if (next_cluster != 0xFFFFFFFF) {
        // desaloca clusters seguintes
        do {
            if (read_sector(sector + t2fs_superbloco_info.pFATSectorStart, (unsigned char*) sector_buffer) != 0)
                return -1;
            // verifica se cluster é o último do arquivo
            if (sector_buffer[sector_offset] != 0xFFFFFFFF)
                next_cluster = sector_buffer[sector_offset];
            else
                next_cluster = 1;
            // libera cluster
            sector_buffer[sector_offset] = 0;
            if (write_sector(sector + t2fs_superbloco_info.pFATSectorStart, (unsigned char*) sector_buffer) != 0)
                return -1;
            // encontra proximo setor da FAT e offset
            if (next_cluster != 1){
                sector = next_cluster / (SECTOR_SIZE / 4);
                sector_offset = next_cluster % (SECTOR_SIZE / 4);
            }
            removed_clusters++;
        } while (next_cluster != 1);
    }

    // atualiza tamanho do arquivo
    t2fs_open_files[handle].file_record->clustersFileSize = t2fs_open_files[handle].file_record->clustersFileSize - removed_clusters;
    t2fs_open_files[handle].file_record->bytesFileSize = t2fs_open_files[handle].current_pointer;

	return 0;
}
