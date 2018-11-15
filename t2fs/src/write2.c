#include "t2fs_internal.h"
#include "apidisk.h"


int write2 (FILE2 handle, char *buffer, int size) {
	t2fs_init();

    unsigned int num_written_bytes;
    unsigned int final_current_pointer;
    unsigned int required_clusters;
	unsigned int num_new_clusters;
    unsigned int current_position_cluster;
    unsigned int cluster_offset;
	unsigned int cluster_size = SECTOR_SIZE * t2fs_superbloco_info.SectorsPerCluster;
	unsigned int sector_buffer [SECTOR_SIZE / 4];
    unsigned int sector;
    unsigned int sector_offset;
    unsigned int next_cluster;
	unsigned int i;
    unsigned int new_cluster;
    char* cluster_buffer;
    unsigned int buffer_offset = 0;

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

    num_written_bytes = size;

    // verifica se o current pointer após a escrita ultrapassa o tamanho do arquivo
    // se sim, pode ser necessário alocar novos clusters
    final_current_pointer = t2fs_open_files[handle].current_pointer + size;
    required_clusters = (final_current_pointer / cluster_size) + 1;
    num_new_clusters = t2fs_open_files[handle].file_record->clustersFileSize - required_clusters;

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

        // atualiza tamanho em clusters
        t2fs_open_files[handle].file_record->clustersFileSize = t2fs_open_files[handle].file_record->clustersFileSize + num_new_clusters;
    }

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

    // verifica se numero de bytes a serem escritos ultrapassa primeiro cluster
    // se não ultrapassa, realiza escrita e retorna. Se ultrapassa, realiza escrita e busca próximo cluster
	if ( (cluster_offset + size) <= cluster_size ) {
        for (i = cluster_offset; i < (cluster_offset + size); i++) {
            // buffer[buffer_offset] = cluster_buffer[i];
            cluster_buffer[i] = buffer[buffer_offset]
            buffer_offset++;
        }
        if (t2fs_write_cluster((int) next_cluster, cluster_buffer) != 0)
            return -1;
        free (cluster_buffer);
        t2fs_open_files[handle].current_pointer = final_current_pointer;
        if (t2fs_open_files[handle].current_pointer > t2fs_open_files[handle].file_record->bytesFileSize)
            t2fs_open_files[handle].file_record->bytesFileSize = t2fs_open_files[handle].current_pointer;
        return num_written_bytes;
	}

	// escreve até o final do cluster onde está a posição corrente
    for (i = cluster_offset; i < cluster_size; i++) {
        cluster_buffer[i] = buffer[buffer_offset];
        buffer_offset++;
    }
    if (t2fs_write_cluster((int) next_cluster, cluster_buffer) != 0)
        return -1;
    free (cluster_buffer);

    // atualiza numero de bytes a serem escritos
    size = size - (cluster_size - cluster_offset);

    // encontra proximos clusters e continua escrevendo até zerar o size
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

        // escreve cluster e finaliza
        if (size <= cluster_size) {
            for (i = 0; i < size; i++) {
                cluster_buffer[i] = buffer[buffer_offset];
                buffer_offset++;
            }
            break;
        }

        // escreve cluster e continua no laço
        for (i = 0; i < cluster_size; i++) {
            cluster_buffer[i] = buffer[buffer_offset];
            buffer_offset++;
        }

        if (t2fs_write_cluster((int) next_cluster, cluster_buffer) != 0)
            return -1;
        free (cluster_buffer);

        // atualiza numero de bytes a serem escritos
        size = size - cluster_size;

    } while (1);

    if (t2fs_write_cluster((int) next_cluster, cluster_buffer) != 0)
        return -1;
    free (cluster_buffer);
    t2fs_open_files[handle].current_pointer = final_current_pointer;
    if (t2fs_open_files[handle].current_pointer > t2fs_open_files[handle].file_record->bytesFileSize)
        t2fs_open_files[handle].file_record->bytesFileSize = t2fs_open_files[handle].current_pointer;
	return num_written_bytes;

	return 0;
}
