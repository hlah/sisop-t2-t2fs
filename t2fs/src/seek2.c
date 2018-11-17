#include "t2fs_internal.h"
#include "apidisk.h"

int seek2 (FILE2 handle, DWORD offset) {
	t2fs_init();

    unsigned int sector_buffer [SECTOR_SIZE / 4];
    // unsigned int required_clusters;
	// unsigned int num_new_clusters;
	unsigned int cluster_size = SECTOR_SIZE * t2fs_superbloco_info.SectorsPerCluster;
    unsigned int sector;
    unsigned int sector_offset;
    unsigned int next_cluster;
	unsigned int i;
    unsigned int new_cluster;

    handle = handle - 1;

    // checa se handle é válido
	if( handle < 0 || handle >= MAXIMUM_OPEN_FILES  ) {
		return -1;
	}

	// checa se posição é usada
	if( t2fs_open_files[handle].file_record == NULL ) {
		return -1;
	}

	// checa se offset ultrapassa o tamanho arquivo
	if( offset > t2fs_open_files[handle].file_record->bytesFileSize && (int) offset != -1) {
		return -1;
	}

    // seta current_pojnter e verifica se é necessário alocar um novo cluster
    if ((int) offset == -1 || offset == t2fs_open_files[handle].file_record->bytesFileSize) {
        t2fs_open_files[handle].current_pointer = t2fs_open_files[handle].file_record->bytesFileSize;

        // aloca novo cluster se necessário
        if ( (t2fs_open_files[handle].current_pointer / cluster_size) == 0 ) {

            // encontra último cluster do arquivo
            sector = t2fs_open_files[handle].file_record->firstCluster / (SECTOR_SIZE / 4);
            sector_offset = t2fs_open_files[handle].file_record->firstCluster % (SECTOR_SIZE / 4);
            for (i = 0; i < t2fs_open_files[handle].file_record->clustersFileSize; i++) {
                if (read_sector(sector + t2fs_superbloco_info.pFATSectorStart, (unsigned char*) sector_buffer) != 0)
                    return -1;
                next_cluster = sector_buffer[sector_offset];
                if (next_cluster != 0xFFFFFFFF) {
                    sector = next_cluster / (SECTOR_SIZE / 4);
                    sector_offset = next_cluster % (SECTOR_SIZE / 4);
                }
            }

            // encontra cluster livre e associa ao arquivo
            new_cluster = t2fs_get_free_cluster();
            if ((int) new_cluster == -1)
                return -1;
            sector_buffer[sector_offset] = new_cluster;
            if (write_sector(sector + t2fs_superbloco_info.pFATSectorStart, (unsigned char*) sector_buffer) != 0)
                return -1;
            sector = new_cluster / (SECTOR_SIZE / 4);
            sector_offset = new_cluster % (SECTOR_SIZE / 4);
            if (read_sector(sector + t2fs_superbloco_info.pFATSectorStart, (unsigned char*) sector_buffer) != 0)
                return -1;
            sector_buffer[sector_offset] = 0xFFFFFFFF;
            if (write_sector(sector + t2fs_superbloco_info.pFATSectorStart, (unsigned char*) sector_buffer) != 0)
                return -1;

            // atualiza tamanho
            t2fs_open_files[handle].file_record->clustersFileSize = t2fs_open_files[handle].file_record->clustersFileSize + 1;
        }

        return 0;
    }

    t2fs_open_files[handle].current_pointer = offset;

    return 0;


    /*

    // atualiza tamanho e verifica clusters se necessário
    if (t2fs_open_files[handle].current_pointer >= t2fs_open_files[handle].file_record->bytesFileSize) {
        t2fs_open_files[handle].file_record->bytesFileSize = t2fs_open_files[handle].current_pointer;
        required_clusters = (t2fs_open_files[handle].current_pointer / cluster_size) + 1;
        num_new_clusters = required_clusters - t2fs_open_files[handle].file_record->clustersFileSize;

        // aloca novos clusters se necessário
        if (num_new_clusters > 0) {

            // encontra último cluster do arquivo
            sector = t2fs_open_files[handle].file_record->firstCluster / (SECTOR_SIZE / 4);
            sector_offset = t2fs_open_files[handle].file_record->firstCluster % (SECTOR_SIZE / 4);
            for (i = 0; i < t2fs_open_files[handle].file_record->clustersFileSize; i++) {
                if (read_sector(sector + t2fs_superbloco_info.pFATSectorStart, (unsigned char*) sector_buffer) != 0)
                    return -1;
                next_cluster = sector_buffer[sector_offset];
                if (next_cluster != 0xFFFFFFFF) {
                    sector = next_cluster / (SECTOR_SIZE / 4);
                    sector_offset = next_cluster % (SECTOR_SIZE / 4);
                }
            }

            // encontra clusters livres e associa ao arquivo
            for (i = 0; i < num_new_clusters; i++) {
                new_cluster = t2fs_get_free_cluster();
                if ((int) new_cluster == -1)
                    return -1;
                sector_buffer[sector_offset] = new_cluster;
                if (write_sector(sector + t2fs_superbloco_info.pFATSectorStart, (unsigned char*) sector_buffer) != 0)
                    return -1;
                sector = new_cluster / (SECTOR_SIZE / 4);
                sector_offset = new_cluster % (SECTOR_SIZE / 4);
                if (read_sector(sector + t2fs_superbloco_info.pFATSectorStart, (unsigned char*) sector_buffer) != 0)
                    return -1;
            }
            sector_buffer[sector_offset] = 0xFFFFFFFF;
            if (write_sector(sector + t2fs_superbloco_info.pFATSectorStart, (unsigned char*) sector_buffer) != 0)
                return -1;

            // atualiza tamanho
            t2fs_open_files[handle].file_record->clustersFileSize = t2fs_open_files[handle].file_record->clustersFileSize + num_new_clusters;
        }
    }

	return 0;

	*/

}
