#include "t2fs_internal.h"
#include "apidisk.h"

int read2 (FILE2 handle, char *buffer, int size) {
    t2fs_init();

    unsigned int num_bytes_to_eof;
    unsigned int current_position_cluster;
    unsigned int cluster_offset;
    unsigned int cluster_size = SECTOR_SIZE *   t2fs_superbloco_info.SectorsPerCluster;
    unsigned int next_cluster;
    unsigned int sector;
    unsigned int sector_offset;
    char* cluster_buffer;
    unsigned int buffer_offset = 0;
    unsigned int num_read_bytes;
    int i;
    unsigned int sector_buffer [SECTOR_SIZE / 4];

    handle = handle - 1;

    // checa se handle é válido
	if( handle < 0 || handle >= MAXIMUM_OPEN_FILES  ) {
		return -1;
	}

	// checa se posição é usada
	if( t2fs_open_files[handle].file_record == NULL ) {
		return -1;
	}

	// tamanho invalido
	if (size < 0)
        return -1;

	// verifica se numero de bytes lidos ultrapassa tamanho do arquivo
    num_bytes_to_eof = t2fs_open_files[handle].file_record->bytesFileSize - t2fs_open_files[handle].current_pointer;
    if (size > num_bytes_to_eof)
        size = num_bytes_to_eof;

    num_read_bytes = size;

    // encontra cluster e offset da posição corrente (relativo ao arquivo)
	current_position_cluster = t2fs_open_files[handle].current_pointer / cluster_size;
	cluster_offset = t2fs_open_files[handle].current_pointer % cluster_size;

    // setor da FAT e offset do primeiro cluster
    next_cluster = t2fs_open_files[handle].file_record->firstCluster;
    sector = next_cluster / (SECTOR_SIZE / 4);
    sector_offset = next_cluster % (SECTOR_SIZE / 4);

    // encontra cluster onde está a posição corrente do arquivo
    for (i = 0; i < current_position_cluster; i++) {
        if (read_sector(sector + t2fs_superbloco_info.pFATSectorStart, (unsigned char*) sector_buffer) != 0)
            return -1;
        next_cluster = sector_buffer[sector_offset];
        sector = next_cluster / (SECTOR_SIZE / 4);
        sector_offset = next_cluster % (SECTOR_SIZE / 4);
    }

    // obtem cluster onde está a posição corrente do arquivo
    cluster_buffer = (char*) t2fs_read_cluster((int) next_cluster);
    if (cluster_buffer == NULL)
        return -1;

    // verifica se numero de bytes a serem lidos ultrapassa primeiro cluster
    // se não ultrapassa, realiza leitura e retorna. Se ultrapassa, realiza leitura e busca próximo cluster
	if ( (cluster_offset + size) <= cluster_size ) {
        for (i = cluster_offset; i < (cluster_offset + size); i++) {
            buffer[buffer_offset] = cluster_buffer[i];
            buffer_offset++;
        }
        free (cluster_buffer);
        t2fs_open_files[handle].current_pointer = t2fs_open_files[handle].current_pointer + num_read_bytes;
        return num_read_bytes;
	}

	// le até o final do cluster onde está a posição corrente
    for (i = cluster_offset; i < cluster_size; i++) {
        buffer[buffer_offset] = cluster_buffer[i];
        buffer_offset++;
    }

    free (cluster_buffer);

    // atualiza numero de bytes a serem lidos
    size = size - (cluster_size - cluster_offset);

    // encontra proximos clusters e continua lendo até zerar o size
    do {
        // encontra proximo cluster
        if (read_sector(sector + t2fs_superbloco_info.pFATSectorStart, (unsigned char*) sector_buffer) != 0)
            return -1;
        next_cluster = sector_buffer[sector_offset];
        sector = next_cluster / (SECTOR_SIZE / 4);
        sector_offset = next_cluster % (SECTOR_SIZE / 4);

        // obtem cluster
        cluster_buffer = (char*) t2fs_read_cluster((int) next_cluster);
        if (cluster_buffer == NULL)
            return -1;

        // le cluster e finaliza
        if (size <= cluster_size) {
            for (i = 0; i < size; i++) {
                buffer[buffer_offset] = cluster_buffer[i];
                buffer_offset++;
            }
            break;
        }

        // le cluster e continua no laço
        for (i = 0; i < cluster_size; i++) {
            buffer[buffer_offset] = cluster_buffer[i];
            buffer_offset++;
        }

        free (cluster_buffer);

        // atualiza numero de bytes a serem lidos
        size = size - cluster_size;

    } while (1);

    free (cluster_buffer);
    t2fs_open_files[handle].current_pointer = t2fs_open_files[handle].current_pointer + num_read_bytes;
	return num_read_bytes;
}
