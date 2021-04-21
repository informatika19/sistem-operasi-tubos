#include "../kernel.h"

//MILESTONE 2
void copySegmentSectorSectors(char *dest, char *src, int n) {
    int i = 0;
    while (i < n) {
        dest[i] = src[i];
        i++;
    }
}

void readSector(char *buffer, int sector){
    interrupt(0x13,0x02*0x100+0x01,buffer,div(sector,36)*0x100+mod(sector,18) + 1,mod(div(sector,18),2)*0x100 + 0);
}

void writeSector(char *buffer, int sector){
    interrupt(0x13,0x03*0x100+0x01,buffer,div(sector,36)*0x100+mod(sector,18) + 1,mod(div(sector,18),2)*0x100 + 0);
}

void readFile(char *buffer, char *path, int *result, char parentIndex){
	char sectorFiles[SECTOR_SIZE*2], sectorSectors[SECTOR_SIZE], segmentSectorSectors[SECTOR_SIZE], fileNameBuffer[14], idxEntrySectors, idxSegmentTarget;

	int i, j, fileFound;
	fileFound = 0;//belum ditemukan

	//read sector
	readSector(sectorFiles, 0x101);
	readSector(sectorFiles + SECTOR_SIZE, 0x102);

	//cek panjang filename valid atau tidak
	if (strlen(path) <= 14)
	{
		//cari filename pada sector files
		for (i = 0; i < (SECTOR_SIZE*2); i+=16)
		{
			if (sectorFiles[i] == parentIndex)//cari file yang index 0(P) == parentIndex
			{
				clear(fileNameBuffer,14);//clear buffer filename
				strncpy(fileNameBuffer,sectorFiles+i+2,14); //copy se
				if (strcmp(fileNameBuffer,path) == 0)//1
				{
					fileFound = 1;//file ditemukan
					idxEntrySectors = sectorFiles[i+1];//index S
					break;
				}
			}	
		}

		//jika file ditemukan copy dari sector sectors ke buffer
		if (fileFound == 1)
		{
			readSector(sectorSectors, 0x103);
			idxSegmentTarget = sectorSectors[idxEntrySectors*16];//idx 1 segment pada sector Sectors
			for (j = 0; j < 16 && sectorSectors[idxEntrySectors*16 + j] != 0x00; j++)
			{
			    idxSegmentTarget = sectorSectors[idxEntrySectors*16 + j];//idx 1 segment pada sector Sectors
				clear(segmentSectorSectors,SECTOR_SIZE);
				readSector(segmentSectorSectors, idxSegmentTarget);
            	copySegmentSectorSectors((buffer+j*SECTOR_SIZE), segmentSectorSectors, SECTOR_SIZE);//copy segment tersebut ke buffer
			}
			*result = 1;//File ditemukan (readFile)
		}

		//file tidak ditemukan
		else
		{
			*result = -1;//File tidak ditemukan (readFile)
		}
	}else{//filename tidak valid(lebih dari 14 byte)
		*result = -1;//File tidak ditemukan (readFile)
	}
}

void removeFile(char *path, int *returnc, char parentIndex){
    char map[512]; //map_sectors
    char files[512*2]; //files_sectors (isinya ada 2)
    char sectors[512]; //sector_sectors

    char file_buff[512]; //isi file
    char filename[14]; //filename (maks 14)

    int i=0;
    int j=0;

    int s_index_sectors = 0; //s index dari sectors sector
    int s_index_files = 0; //s index dari sectors file
    int sector_read = 0; //yang dibaca
    int files_entry = 0; //entry file

    int filefound = 0; //apakah file ketemu
    int isFile = 0; //apakah nama termasuk file

    readSector(files[0], 0x101);
    readSector(files[0], 0x101+1);

    if(strlen(path) < 14){
        for(i; i<2; i++){
            j=0;
            while(!filefound && j<512){
                if(files[i][j] == parentIndex){
                   clear(filename, 14);
                   strcopybounded(filename, files[i]+j+0x2, 14);
                   if(!strcmp(path, filename)){
                       filefound = 1;
                       s_index_sectors = files[i][j+0x1];
                       s_index_files = i;
                       files_entry = j;
                       if(files[i][j*0x10+0x1]!=0xFF){
                           isFile = true;
                       } 
                   } 
                }
                j += 0x10;
            }
        }
    }

    if(filefound){
        if(isFile){
            for(i=0; i<16; i++){
                if(i==0)
                    files[s_index_sectors][files_entry+i] = 0xFF;
                if(i==1)
                    files[s_index_sectors][files_entry+i] = 0xDE;
                else
                    files[s_index_sectors][files_entry+i] = 0x0;
            }

            readSector(map, 0x101);
            readSector(sectors, 0x103);

            for(i=0; i<0x10; i++){
                if(sector_read > 16)
                    map[sector_read] = 0x00;
                sectors[s_index_sectors*0x10+i] = 0x00;
                sector_read = sectors[s_index_sectors*0x10+i];
            }
        }
        writeSector(files[0], 0x101);
        writeSector(files[1], 0x101+1);

        if(isFile){
            writeSector(map, 0x100);
            writeSector(sectors, 0x103);
            *returncore = 0;
        }
    }
    if(!filefound)
        *returncode = -1;
    else
        *returncode = 1;
    
}

//referensi tanur
void writeFile(char *buffer, char *path, int *sectors, char parentIndex) {
    // TODO : Extra, Extra, Extra, use multidimensional array damnit
    // TODO : Extra, special flag for link
    char map_buf[SECTOR_SIZE], files_buf[2][SECTOR_SIZE], sectors_buf[SECTOR_SIZE]; // Filesystem buffer
    char file_segment_buffer[SECTOR_SIZE]; // Buffer for writing to sector, always get clear()
    char filename_buffer[16], adjusted_path[16];
    char parent_entry_byte; // Temporary "P" byte holder / parent index at files filesystem
    int i = 0, j = 0, segment_idx = 0; // Iterator index
    int f_entry_idx = 0, f_entry_sector_idx = 0, sectors_entry_idx = 0; // Targets Index
    int map_empty_bytes_sum = 0, buffer_size = 0, write_file_error_code = 0;
    int is_empty_dir_exist = 0, is_enough_sector = 0;
    int is_empty_sectors_idx_exist = 0, is_empty = 1;
    int is_ready_to_write_file = 0, is_done_write_file = 0;
    int buffer_type_is_file = 1, is_file_already_exist = 0;
    int f_target_found = 0;
    int valid_parent_folder = 1, valid_filename = 1, valid_filename_length = 1;

    // Filename length check
    if (strlen(path) > 14) {
        valid_filename_length = 0;
        valid_filename = 0;
    }

    // Directory checking in files filesystem
    // valid_filename_length will represent validity of filename length
    // while valid_filename representing whether file / folder is duplicate in single parent
    if (valid_filename_length) {
        readSector(files_buf[0], 0x101);
        readSector(files_buf[1], 0x101 + 1);
        while (i < 2 && valid_filename) {
            j = 0;
            while (j < SECTOR_SIZE && valid_filename) {
                // Checking entry byte flag ("S" byte)
                if (files_buf[i][j+ENTRY_BYTE_OFFSET] == EMPTY_FILES_ENTRY && !f_target_found) {
                    f_entry_sector_idx = i;
                    f_entry_idx = j;
                    f_target_found = 1; // If empty dir entry exists, stop find new one (Pick first empty entry)
                    is_empty_dir_exist = 1;
                }
                // Checking existing filename in same parent folder
                if (files_buf[i][j+PARENT_BYTE_OFFSET] == parentIndex) {
                    // Needed buffer because entry may ignoring null terminator
                    clear(filename_buffer, 16);
                    strcpybounded(filename_buffer, files_buf[i]+j+PATHNAME_BYTE_OFFSET, 14);
                    if (!strcmp(path, filename_buffer))
                        valid_filename = 0;
                }
                j += FILES_ENTRY_SIZE;
            }
            i++;
        }
    }

    // Checking buffer type, either writing folder or file
    if (buffer == NULL)
        buffer_type_is_file = 0;

    // Checking whether folder located at parentIndex is valid
    // parentIndex == ROOT_PARENT_FOLDER always valid parent folder
    if (parentIndex != ROOT_PARENT_FOLDER) {
        // div(parentIndex, SECTOR_SIZE/FILES_ENTRY_SIZE) -> Because 1 files filesystem only contain SECTOR_SIZE/FILES_ENTRY_SIZE index
        // mod(parentIndex*FILES_ENTRY_SIZE, SECTOR_SIZE)+ENTRY_BYTE_OFFSET ->
        //      2 files filesystem span from 0 to 2*SECTOR_SIZE-1 bytes, 1 files only contain 1 SECTOR_SIZE.
        //      ENTRY_BYTE_OFFSET used for checking "S" byte / entry byte in files filesystem
        parent_entry_byte = files_buf[div(parentIndex,SECTOR_SIZE/FILES_ENTRY_SIZE)][mod(parentIndex*FILES_ENTRY_SIZE, SECTOR_SIZE)+ENTRY_BYTE_OFFSET];
        if (parent_entry_byte != FOLDER_ENTRY)
            valid_parent_folder = 0;
    }

    // Writing file / folder
    if (is_empty_dir_exist && valid_parent_folder && valid_filename) {
        // Updating files filesystem buffer
        files_buf[f_entry_sector_idx][f_entry_idx+PARENT_BYTE_OFFSET] = parentIndex;
        strcopy(path, (files_buf[f_entry_sector_idx]+f_entry_idx+PATHNAME_BYTE_OFFSET));

        // ----------- Folder Writing Branch-----------
        // Folder writing does not need to readSector() sectors and map
        if (!buffer_type_is_file) {
            // Updating files filesystem buffer
            files_buf[f_entry_sector_idx][f_entry_idx+ENTRY_BYTE_OFFSET] = FOLDER_ENTRY;
        }


        // ----------- File Writing Branch-----------
        // readSector() for sectors and map will be called if needed
        // Checking whether enough empty space or not in map filesystem
        if (buffer_type_is_file) {
            
            readSector(map_buf, MAP_SECTOR);
            i = 0;
            buffer_size = strlen(buffer); // In bytes,
            // FIXME : Extra, due to strlen() stop at null byte, it cannot write in pure binary mode
            while (i < (SECTOR_SIZE >> 1) && !is_enough_sector) {
                // Finding empty sector in map
                if (map_buf[i] == EMPTY_MAP_ENTRY)
                    map_empty_bytes_sum += SECTOR_SIZE;
                if (buffer_size <= map_empty_bytes_sum)
                    is_enough_sector = 1;
                i++;
            }
        }

        // Checking available entry in sectors filesystem
        if (is_enough_sector) {
            readSector(sectors_buf, SECTORS_SECTOR);
            // Outer loop checking per files (1 file = 16 bytes in sectors filesystem)
            i = 0;
            while (i < SECTORS_ENTRY_COUNT && !is_empty_sectors_idx_exist) {
                j = 0;
                is_empty = 1;
                // Inner loop checking is 1 file is all EMPTY_SECTORS_ENTRY byte or not
                while (j < SECTORS_ENTRY_SIZE && is_empty) {
                    if (sectors_buf[i*SECTORS_ENTRY_SIZE + j] != EMPTY_SECTORS_ENTRY)
                        is_empty = 0;
                    j++;
                }

                // If found empty index set flag
                if (is_empty) {
                    is_empty_sectors_idx_exist = 1;
                    is_ready_to_write_file = 1;
                    sectors_entry_idx = i;
                }

                i++; // Jumping 16 bytes
            }
        }

        // File writing
        if (is_ready_to_write_file) {
            // Updating files filesystem buffer
            files_buf[f_entry_sector_idx][f_entry_idx+ENTRY_BYTE_OFFSET] = sectors_entry_idx;

            // Find empty sector between 0x0 and 0x100
            // (256, limitation of 1 byte entry in sectors filesystem) and write
            i = 0;
            j = 0;
            while (i < MAXIMUM_SECTOR_MAPPED && !is_done_write_file) {
                if (map_buf[i] == EMPTY_MAP_ENTRY) {
                    // Updating map filesystem
                    map_buf[i] = FILLED_MAP_ENTRY;

                    // Updating sectors filesystem
                    // FIXME : Extra, split to multiple sectors
                    // WARNING : Will stop writing if file more than 8192 bytes
                    if (j < SECTORS_ENTRY_SIZE)
                        sectors_buf[sectors_entry_idx*SECTORS_ENTRY_SIZE+j] = i;
                    j++;

                    // Entry writing at sector
                    clear(file_segment_buffer, SECTOR_SIZE);
                    copySegmentSectorSectors(file_segment_buffer, (buffer+segment_idx*SECTOR_SIZE), SECTOR_SIZE);
                    writeSector(file_segment_buffer, i);
                    segment_idx++;
                    buffer_size -= SECTOR_SIZE;
                }
                // Checking is file done writing
                if (buffer_size <= 0)
                    is_done_write_file = 1;

                i++;
            }

            // If theres still not-filled sectors bytes, fill with FILLED_EMPTY_SECTORS_BYTE
            while (j < SECTORS_ENTRY_SIZE) {
                sectors_buf[sectors_entry_idx*SECTORS_ENTRY_SIZE+j] = FILLED_EMPTY_SECTORS_BYTE;
                j++;
            }

        }


        // Filesystem records update
        writeSector(files_buf[0], 0x101);
        writeSector(files_buf[1], 0x101 + 1);
        if (buffer_type_is_file) {
            writeSector(map_buf, MAP_SECTOR);
            writeSector(sectors_buf, SECTORS_SECTOR);
        }
    }



    // Error code writing
    if (!valid_filename)
        (*sectors) = -1; // Filename too long or file exists
    else if (!is_empty_dir_exist)
        (*sectors) = -2; // Not enough entry in files filesystem
    else if (!is_enough_sector && buffer_type_is_file)
        (*sectors) = -3; // Not enough empty sectors, only for files
    else if (!valid_parent_folder)
        (*sectors) = -4; // Parent folder not valid
    else
        (*sectors) = 0;
}

void executeProgram(char *filename, int segment, int *success, char parentIndex) {
    // Buat buffer
    int isSuccess;
    char fileBuffer[512 * 16];
    // Buka file dengan readFile
    clear(fileBuffer, 512 * 16);
    readFile(&fileBuffer, filename, &isSuccess, parentIndex);
    // If success, salin dengan putInMemory
    if (isSuccess == 1) {
        int i = 0;
        // launchProgram
        for (i = 0; i < 512*16; i++) {
        putInMemory(segment, i, fileBuffer[i]);}
        launchProgram(segment);
    } else {
        interrupt(0x21, 0, "File not found!", 0,0);
    }
}
