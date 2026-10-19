#include "main.h"
#include "id3_writer.h"
#include "id3_reader.h"
#include "id3_utils.h"

typedef unsigned int uint;

unsigned char tag_id[5];            // To store the tag name which one want to edit    

/*
 * To check whether the tag name is matching or not
 * If it is match store the tag name 
 */
int find_tag(const char *tag)       
{                                      

    if (strcmp(tag, "-t") == 0)
        strcpy(tag_id, "TIT2");
    else if (strcmp(tag, "-a") == 0)
        strcpy(tag_id, "TPE1");
    else if (strcmp(tag, "-A") == 0)
        strcpy(tag_id, "TALB");
    else if (strcmp(tag, "-y") == 0)
        strcpy(tag_id, "TYER");
    else if (strcmp(tag, "-g") == 0)
        strcpy(tag_id, "TCON");
    else if (strcmp(tag, "-c") == 0)
        strcpy(tag_id, "COMM");
    else    
    {
        printf(RED"Error message : Trying to modify the non existing tag\n"RESET);
        return 1;
    }
    return 0;
}

void little_to_big(char *tag_size, uint size)           //To convert the data from little endian to big endian 
{
    tag_size[0] = (size >> 24) & 0XFF;
    tag_size[1] = (size >> 16) & 0xFF;
    tag_size[2] = (size >> 8) & 0xFF;
    tag_size[3] = size & 0XFF;
}
int big_to_little(const unsigned char *size)            // To convert the data from big endian to little endian
{
    return ((((uint)size[0]) << 24) |
            (((uint)size[1]) << 16) |
            (((uint)size[2]) << 8) |
            ((uint)size[3]));
}
void convert_synchsafe(int size, char *tag_size)        // Convert from integer to synchsafe integer
{
    tag_size[0] = (size >> 21) & 0X7F;
    tag_size[1] = (size >> 14) & 0X7F;
    tag_size[2] = (size >> 7) & 0X7F;
    tag_size[3] = (size) & 0X7F;
}

int edit_tag(const char *filename, const char *tag, const char *value)
{
    printf("------------------------------------------------------\n");
    printf(BLUE"                 Edit option enabled\n"RESET);
    printf("------------------------------------------------------\n");
    int res=find_tag(tag);
    if(res==1)
        return 1;
    FILE *org_fptr = fopen(filename, "r");       // Open the file in read mode
    if(org_fptr== NULL)
    {
        printf(RED"Error message : Unable to open the file\n"RESET);
        return 1;
    }
    FILE *temp_fptr = fopen("temp.mp3", "w+");      // create a temporary file to store the data from original file
    if(temp_fptr == NULL)
    {
        printf(RED"Error message : Unable to open the file\n"RESET);
        return 1;
    }

    unsigned char header[11];                       // To store the header data's

    fread(header, 1, 10, org_fptr);
    header[10] = '\0';
    fwrite(header, 1, 10, temp_fptr);
    uint total_size = (((uint)header[6] << 21) |    // Finding the total tag data size
                       ((uint)header[7] << 14) |
                       ((uint)header[8] << 7) |
                       ((uint)header[9]));

    unsigned char tag_name[5];                      // To store the tag name
    unsigned char tag_size[5];                      // To store the tag size
    unsigned char tag_flag[3];                      // To store the tag flag data
    unsigned char encoded_byte;                     // To store encoded byte value
    unsigned char new_tag_size[5];                  // To store the new tag data size
    uint new_size;                                  // To store the new size in integer
    uint data_size = 0;                             // To store the tag data size in integer
    int size_diff = 0;                              // To store the difference b/w new data size and old data size
    int found = 0;                                  // To check whether the tag is found or not

    while ((ftell(org_fptr) - 10) < total_size)     // To check upto tag data size
    {
        fread(tag_name, 1, 4, org_fptr);
        tag_name[4] = '\0';             

        if (tag_name[0] == 0 &&                     // To check whether the padding data is attained or not
            tag_name[1] == 0 &&
            tag_name[2] == 0 &&
            tag_name[3] == 0)
        {

            fwrite(tag_name, 1, 4, temp_fptr);
            break;
        }

        if (strcmp(tag_name, "APIC") == 0)      // To check whether it reaches APIC tag or not 
        {
            fwrite(tag_name, 1, 4, temp_fptr);

            break;
        }

        fread(tag_size, 1, 4, org_fptr);
        tag_size[4] = '\0';
        fread(tag_flag, 1, 2, org_fptr);
        tag_flag[2] = '\0';
        
        if (strcmp(tag_name, tag_id) == 0)                  // If tag found 
        {                                                       // read and write the new data
            
            found = 1;
            data_size = big_to_little(tag_size);
            fseek(org_fptr, data_size, SEEK_CUR);
            fwrite(tag_name, 1, 4, temp_fptr);
            new_size = strlen(value);
            little_to_big(new_tag_size, (new_size + 1));
            fwrite(new_tag_size, 1, 4, temp_fptr);
            fwrite(tag_flag, 1, 2, temp_fptr);
            encoded_byte = 0X00;
            fwrite(&encoded_byte, 1, 1, temp_fptr);
            fwrite(value, 1, new_size, temp_fptr);
            size_diff = data_size - (new_size + 1);
            break;
        }
        else                                                // If tag not found
        {                                                       // copy the old data 
            fwrite(tag_name, 1, 4, temp_fptr);
            fwrite(tag_size, 1, 4, temp_fptr);
            fwrite(tag_flag, 1, 2, temp_fptr);

            data_size = big_to_little(tag_size);                
            
            unsigned char ch;
            for (int i = 0; i < data_size; i++)             // Store the data from original to temp file
            {
                fread(&ch, 1, 1, org_fptr);
                fwrite(&ch, 1, 1, temp_fptr);
            }
        }
    }
    if (!found)                                             // If tag is not found ,termianted the program 
    {
        printf(RED"Error message : Tag not found \n"RESET);
        return 1;
    }

    char ch;
    while (fread(&ch, 1, 1, org_fptr) != 0)                 // Copy the remaining data from original to temp file
    {
        fwrite(&ch, 1, 1, temp_fptr);
    }
    
    unsigned char new_header_size[5];                       // To store the new header size              

    fseek(temp_fptr, 0, SEEK_SET);                          
    fseek(temp_fptr, 6, SEEK_CUR);                          // Move the file pointer 
    fread(new_header_size, 1, 4, temp_fptr);                // read the old header size from the file
    new_header_size[4] = '\0';

    if (size_diff > 0)                                      // If it is positive ,reduce the header size
    {
        total_size = total_size - size_diff;
        convert_synchsafe(total_size, new_header_size);
    }
    else if (size_diff < 0)                                 // If it is negative ,add the new size
    {
        size_diff *= -1;
        total_size = total_size + size_diff;
        convert_synchsafe(total_size, new_header_size);
    }
    fseek(temp_fptr, -4, SEEK_CUR);
    fwrite(new_header_size, 1, 4, temp_fptr);

    fclose(org_fptr);                                       // close both file's
    fclose(temp_fptr);                                      
    remove("sample.mp3");                                    // delete the original file 
    rename("temp.mp3", "sample.mp3");                        // rename the temp file into original file
    return 0;
}