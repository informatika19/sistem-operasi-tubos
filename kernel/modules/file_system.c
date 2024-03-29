#include "../kernel.h"
int isFileValid(char *path, char srcIndex, char files[2][512], int *files_idx, int* files_entry_idx);
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
    char files[2][SECTOR_SIZE]; //Files sectors
    char map[SECTOR_SIZE]; //Map sectors
    char sectors[SECTOR_SIZE]; //Sector sectors
    char file_container[512]; //Isi file
    char filename[14]; //Nama file
    int i = 0, j = 0;
    int s_id_sectors = 0; //S byte sectors
    int reader = 0;
    int files_entry_idx = 0;
    int files_idx = 0; //Files Sector ke 1 atau 2
    int filefound = 0;
    int isFile = 0;

    readSector(files[0], FILES_SECTOR);
    readSector(files[1], FILES_SECTOR + 1);

    //Pengecekan input nama file valid
    if (strlen(path) < 14) {
        //Looping pada 2 sector files
        for(i=0; i<2 && !filefound; i++) {
            j = 0;
            while (j < SECTOR_SIZE && !filefound) {
                //Cek apakah sama seperti parent folder
                if (files[i][j] == parentIndex) {
                    clear(filename, 14);
                    strcpybounded(filename, files[i]+j+PATHNAME_BYTE_OFFSET, 14);
                    //Apabila nama sama
                    if (!strcmp(path, filename)) {
                        filefound = 1;
                        s_id_sectors = files[i][j+ENTRY_BYTE_OFFSET];
                        files_idx = i;
                        files_entry_idx = j;
                        if (files[i][j+ENTRY_BYTE_OFFSET] != FOLDER_ENTRY)
                            isFile = 1;
                    }
                }
                j += FILES_ENTRY_SIZE;
            }
        }
    }

    if (filefound) {
        if (isFile) {
            for(i=0; i<16; i++){
                if(i==0)
                    files[files_idx][files_entry_idx] = ROOT_PARENT_FOLDER;
                else if(i==1)
                    files[files_idx][files_entry_idx+i] = EMPTY_FILES_ENTRY;
                else
                    files[files_idx][files_entry_idx + i] = 0x00;
            }

            readSector(sectors, SECTORS_SECTOR);
            readSector(map, MAP_SECTOR);

            for(i = 0 ; i < SECTORS_ENTRY_SIZE; i++){
                reader = sectors[s_id_sectors*SECTORS_ENTRY_SIZE + i];
                if (reader > 16)
                    map[reader] = EMPTY_MAP_ENTRY;
                sectors[s_id_sectors*SECTORS_ENTRY_SIZE + i] = EMPTY_SECTORS_ENTRY;
                
            }
            *returnc = 0; //sebuah file
        }
        //Overwrite ke sector files
        writeSector(files[0], FILES_SECTOR);
        writeSector(files[1], FILES_SECTOR + 1);
        if (isFile) {
            //Overwrite ke sector map dan sectors kalau jenisnya file
            writeSector(map, MAP_SECTOR);
            writeSector(sectors, SECTORS_SECTOR);
        }
        *returnc = 1; //bukan sebuah file
    }

    else
        *returnc = -1; //return code invalid
    
}

//referensi kelompok lain (13519214)
void writeFile(char *buffer, char *path, int *sectors, char parentIndex) {
    // TODO : Extra, Extra, Extra, use multidimensional array damnit
    // TODO : Extra, special flag for link
    char map_buf[SECTOR_SIZE], files_buf[2][SECTOR_SIZE], sectors_buf[SECTOR_SIZE]; // Filesystem buffer
    char file_container[SECTOR_SIZE]; // Buffer for writing to sector, always get clear()
    char filename[16], adjusted_path[16];
    char parent_entry_byte; // Temporary "P" byte holder / parent index at files filesystem
    int i = 0, j = 0, segment_idx = 0; // Iterator index
    int f_entry_idx = 0, f_entry_sector_idx = 0, s_id_sectors = 0; // Targets Index
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
                    clear(filename, 16);
                    strcpybounded(filename, files_buf[i]+j+PATHNAME_BYTE_OFFSET, 14);
                    if (!strcmp(path, filename))
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
                    s_id_sectors = i;
                }

                i++; // Jumping 16 bytes
            }
        }

        // File writing
        if (is_ready_to_write_file) {
            // Updating files filesystem buffer
            files_buf[f_entry_sector_idx][f_entry_idx+ENTRY_BYTE_OFFSET] = s_id_sectors;

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
                        sectors_buf[s_id_sectors*SECTORS_ENTRY_SIZE+j] = i;
                    j++;

                    // Entry writing at sector
                    clear(file_container, SECTOR_SIZE);
                    copySegmentSectorSectors(file_container, (buffer+segment_idx*SECTOR_SIZE), SECTOR_SIZE);
                    writeSector(file_container, i);
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
                sectors_buf[s_id_sectors*SECTORS_ENTRY_SIZE+j] = FILLED_EMPTY_SECTORS_BYTE;
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

//referensi kelompok lain (13519214)
char directoryEvaluator(char *dirtable, char *dirstr, int *returncode, char current_dir) {
    char evaluated_dir = current_dir;
    char parent_byte_buffer = -1;
    char directory_name[8][32];
    char filename_buffer[16];
    int i, j, k, dirnamecount;
    int is_valid_args = 1, is_folder_found = 0;
    clear(directory_name, 8*32);

    // TODO : Extra, maybe std -> strsplit()
    // Arguments splitting -> From argv in shell(), with some modification
    i = 0;
    j = 0;
    k = 0;
    while (dirstr[i] != 0x00) {
        // If found slash in commands and not within double quote mark, make new
        if (dirstr[i] == 0x2F && j < 8) {
            k = 0;
            j++;
        }
        else {
            // Only copy if char is not double quote
            // and space outside double quote
            directory_name[j][k] = dirstr[i];
            k++;
        }
        i++;
    }
    dirnamecount = j + 1; // Due j is between counting space between 2 args

    // Deleting last slash (ex. mnt/a/b/ -> argv entries = {mnt, a, b, ""} to argv = {mnt, a, b})
    if (!strcmp(directory_name[dirnamecount-1], ""))
        dirnamecount--;


    // Parsing priority :
    // 1. If found "." -> change evaluated dir to current dir, will ignoring previous evaluation
    // 2. If found ".." -> move to parent folder
    // 3. If found foldername -> search foldername in evaluated dir
    i = 0;
    while (i < dirnamecount && is_valid_args) {
        if (!strcmp(directory_name[i], "."))
            evaluated_dir = current_dir;
        else if (!strcmp(directory_name[i], "..")) {
            // If evaluated dir is NOT in between files entry count and 0 (or valid files index), do nothing
            // (Root flag by default is on 0xFF which is by default not in range)
            // else, change evaluated dir to parent evaluated dir
            if (0 <= evaluated_dir && evaluated_dir < FILES_ENTRY_COUNT)
                evaluated_dir = dirtable[evaluated_dir*FILES_ENTRY_SIZE+PARENT_BYTE_OFFSET];
        }
        else {
            // If string matching not found, break loop return -1 as failed evaluation
            j = 0;
            is_folder_found = 0;
            while (j < FILES_ENTRY_COUNT && !is_folder_found) {
                clear(filename_buffer, 16);
                strcpybounded(filename_buffer, dirtable+j*FILES_ENTRY_SIZE+PATHNAME_BYTE_OFFSET, 14);
                // If within same parent folder and pathname match, change evaluated_dir
                parent_byte_buffer = dirtable[j*FILES_ENTRY_SIZE+PARENT_BYTE_OFFSET];
                if (!strcmp(directory_name[i], filename_buffer) && parent_byte_buffer == evaluated_dir) {
                    is_folder_found = 1;
                    evaluated_dir = j; // NOTE : j represent files entry index
                }
                j++;
            }

            if (!is_folder_found)
                is_valid_args = 0;
        }
        i++;
    }

    if (!is_valid_args)
        *returncode = -1;
    else
        *returncode = 0;

    return evaluated_dir;
}

void getDir(char *dir){
    interrupt(0x21, 0x0002, dir, FILES_SECTOR, 0);
    interrupt(0x21, 0x0002, dir+SECTOR_SIZE, FILES_SECTOR+1, 0);
}

void moveFile(char *path, int *returnc, char srcIndex, char destIndex){
    char files[2][SECTOR_SIZE]; //Files sectors
    char filename[14]; //Nama file
    int i = 0, j = 0;
    int reader = 0;
    int files_entry_idx = 0;
    int files_idx = 0; //Files Sector ke 1 atau 2
    int isFound = 0;

    readSector(files[0], FILES_SECTOR);
    readSector(files[1], FILES_SECTOR + 1);

    isFound = isFileValid(path,srcIndex,files,&files_idx,&files_entry_idx);

    if (isFound) {
        files[files_idx][files_entry_idx] = destIndex;
        *returnc = 0; //sebuah file
        //Overwrite ke sector files
        writeSector(files[0], FILES_SECTOR);
        writeSector(files[1], FILES_SECTOR + 1);
    }

    else
        *returnc = -1; //return code invalid
    
}

int isFileValid(char *path, char srcIndex, char files[2][512], int *files_idx, int* files_entry_idx){
    int filefound = 0;
    int i =0;
    int j =0;
    char filename[14];
    if (strlen(path) < 14) {
        //Looping pada 2 sector files
        for(i=0; i<2 && !filefound; i++) {
            j = 0;
            while (j < SECTOR_SIZE && !filefound) {
                //Cek apakah sama seperti parent folder
                if (files[i][j] == srcIndex) {
                    clear(filename, 14);
                    strcpybounded(filename, files[i]+j+PATHNAME_BYTE_OFFSET, 14);
                    //Apabila nama sama
                    if (!strcmp(path, filename)) {
                        filefound = 1;
                        *files_idx = i;
                        *files_entry_idx = j;
                    }
                }
                j += FILES_ENTRY_SIZE;
            }
        }
    }
    return filefound;
}