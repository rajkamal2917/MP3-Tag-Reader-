#include "main.h"
#include "id3_reader.h"
typedef unsigned int uint;

/**
 * Display the tag details 
 */
void display_metadata(const TagData *data)
{
    printf("---------------------------------------------\n");
    printf(BLUE"               MP3 TAG DETAILS \n"RESET);
    printf("---------------------------------------------\n");

    printf(BLUE"                               VERSION :"RESET" %s\n", data->version);
    printf(BLUE"TITLE   :"RESET" %s\n", data->title);
    printf(BLUE"ARTIST  :"RESET" %s\n", data->artist);
    printf(BLUE"ALBUM   :"RESET" %s\n", data->album);
    printf(BLUE"YEAR    :"RESET" %s\n", data->year);
    printf(BLUE"GENRE   :"RESET" %s\n", data->genre);
    printf(BLUE"COMMENT :"RESET" %s\n", data->comment);
    printf("---------------------------------------------\n");
}


int big_little(const unsigned char *size)                           // To convert the data from big endian to little endian
{
    return ((((uint)size[0]) << 24) |
            (((uint)size[1]) << 16) |
            (((uint)size[2]) << 8) |
            ((uint)size[3]));
}

TagData *read_id3_tags(const char *filename)                        
{
    FILE *fptr = fopen(filename, "r");                              // Open the file in read mode
    if(fptr== NULL)
    {
        printf(RED"Error message : Unable to open the file\n"RESET);
        return NULL;
    }                           

    unsigned char header[11];                                       // To store the header information
    uint total_size;                                                   

    fread(header, 1, 10, fptr); 

    if (strncmp(header, "ID3", 3) != 0)                             // To check whether it has version 2 or not
    {
        printf(RED"Error message : MP3 file contains different version\n"RESET);
        return NULL;
    }

    TagData *data = create_tag_data();                              // Create the tag data 
    data->version = malloc(3);                                      // Allocating memory dynamically for version 
    if (header[3] == 3)
    {
        strcpy(data->version, "2.3");

        total_size = (((uint)header[6] << 21) |                     // total _tag information
                      ((uint)header[7] << 14) |
                      ((uint)header[8] << 7) |
                      ((uint)header[9]));

        if ((header[5] & 0X40) != 0)                                // Check the file has extended header
        {
            printf(BLUE"The file has extended header\n"RESET);
            uint ext_header_size;
            unsigned char ext_size[5];
            fread(ext_size, 1, 4, fptr);
            ext_size[4] = '\0';
            ext_header_size = (((uint)ext_size[0] << 21) |
                               ((uint)ext_size[1] << 14) |
                               ((uint)ext_size[2] << 7) |
                               ((uint)ext_size[3]));
            fseek(fptr, ext_header_size - 4, SEEK_CUR);
        }
    }
    else
    {
        printf(RED"Error message : MP3 file has some other version\n"RESET);
        return NULL;
    }

    unsigned char tag_name[5];                                          // To store the tag name 
    unsigned char tag_size[5];                                          // To store tag size
    unsigned char tag_flag[3];                                          // To store flag data
    unsigned char encode_byte;                                          // To store encoded byte
    uint tag_data_size;

    while (((ftell(fptr) - 10) < total_size))                           // Check the tag name upto total tag size
    {

        fread(tag_name, 1, 4, fptr);                                    // Read tag name
        tag_name[4] = '\0';
        fread(tag_size, 1, 4, fptr);                                    // Read tag size
        tag_size[4] = '\0';
        tag_data_size = big_little(tag_size);                           // Find the tag data size

        fread(tag_flag, 1, 2, fptr);                                    // Read flag data
        tag_flag[2] = '\0';

        if (strcmp(tag_name, "TIT2") == 0)                              // If the tag is match ,
        {                                                                   // Allocate a memory dynamically and store the tag information
            fread(&encode_byte, 1, 1, fptr);
            data->title = malloc(tag_data_size);
            fread(data->title, 1, tag_data_size - 1, fptr);
            data->title[tag_data_size - 1] = '\0';
        }

        else if (strcmp(tag_name, "TPE1") == 0)
        {
            fread(&encode_byte, 1, 1, fptr);
            data->artist = malloc(tag_data_size);
            fread(data->artist, 1, tag_data_size - 1, fptr);
            data->artist[tag_data_size - 1] = '\0';
        }
        else if (strcmp(tag_name, "TALB") == 0)
        {
            fread(&encode_byte, 1, 1, fptr);
            data->album = malloc(tag_data_size);
            fread(data->album, 1, tag_data_size - 1, fptr);
            data->album[tag_data_size - 1] = '\0';
        }
        else if (strcmp(tag_name, "TYER") == 0)
        {
            fread(&encode_byte, 1, 1, fptr);
            data->year = malloc(tag_data_size);
            fread(data->year, 1, tag_data_size - 1, fptr);
            data->year[tag_data_size - 1] = '\0';
        }
        else if (strcmp(tag_name, "TCON") == 0)
        {
            fread(&encode_byte, 1, 1, fptr);
            data->genre = malloc(tag_data_size);
            fread(data->genre, 1, tag_data_size - 1, fptr);
            data->genre[tag_data_size - 1] = '\0';
        }

        else if (strcmp(tag_name, "COMM") == 0)
        {
            fread(&encode_byte, 1, 1, fptr);
            data->comment = malloc(tag_data_size);
            fread(data->comment, 1, tag_data_size - 1, fptr);
            data->comment[tag_data_size - 1] = '\0';
        }
        else
            fseek(fptr, tag_data_size, SEEK_CUR);
    }

    return data;
}


void view_tags(const char *filename)
{
    TagData *data = read_id3_tags(filename);
    if (!data)
    {
        printf(RED"Error message : Failed to read ID3 tags\n"RESET);
        return;
    }
    display_metadata(data);
    free_tag_data(data);
}