/*
DOCUMENTATION

NAME         :Rajkamal V
ID           :26012_111
START DATE   :01/10/2026
END DATE     :18/10/2026

SAMPLE INPUT :
./a.out -v sample.mp3

SAMPLE OUTPUT:
---------------------------------------------
               MP3 TAG DETAILS 
---------------------------------------------
                               VERSION : 2.3
TITLE   : Rolex_theme
ARTIST  : Anirudh_Ravichandran
ALBUM   : Vikram
YEAR    : 2022
GENRE   : Mass
COMMENT : engtamil
---------------------------------------------
*/
#include "main.h"
#include "id3_reader.h"
#include "id3_writer.h"

/**
 * Displays the help message for the MP3 Tag Reader application.
 */
void display_help()
{
    printf("--------------------------------------------------\n");
    printf("           MP3 TAG READER - HELP MENU\n");
    printf("--------------------------------------------------\n");

    printf("Usage   : mp3tagreader [options] filename\n");
    printf("Options :\n");
    printf("  -h               Display help\n");
    printf("  -v               View tags\n\n");
    printf("  -e -t/-a/-A/-y/-g/-c <value>  Edit tags\n");
    printf("--------------------------------------------------\n");
    printf("eg: ./a.out -e -t new_title filename\n");
    printf("--------------------------------------------------\n");

}
/* 
 * Validate filename 
 * return 0 on success , non zero on failure
 */
int validate_filename(char *argv)
{
    char *ext = strstr(argv, ".");
    if (ext != NULL && (strcmp(ext, ".mp3") == 0))
        return 0;

    else
        return 1;
}

/**
 * Main function to handle command-line arguments and execute appropriate actions.
 * 
 * argc Argument count.
 * argv Argument vector.
 * return 0 on success, non-zero on failure.
 */
int main(int argc, char *argv[])
{

    if (argc < 2)                                   // If arg count is less than 2 
    {                                                   // call display help function
        display_help();
        return 1;
    }

    if (strcmp(argv[1], "-h") == 0)                 
    {
        display_help();
        return 0;
    }
    else if (strcmp(argv[1], "-v") == 0 && argc == 3)   
    {
        if(validate_filename(argv[2])!=0)
        {
            printf(RED"Error message : File ext should be .mp3 only\n"RESET);
            return 1;
        }
        view_tags(argv[2]);
    }
    else if (strcmp(argv[1], "-e") == 0 && argc == 5)
    {
        char *tag = argv[2];                            // Store which tag wants to edit
        char *value = argv[3];                          // Store the edit information
        char *filename = argv[4];                       // Store the file name
        if(validate_filename(argv[4])!=0)
        {
            printf(RED"Error message : File ext should be .mp3 only\n"RESET);
            return 1;
        }
        if (edit_tag(filename, tag, value) != 0)
        {
            printf(RED"Error message : Failed to edit tag data\n"RESET);
            printf("------------------------------------------------------\n");
            return 1;
        }
        printf(GREEN"Tag information edited successfully\n"RESET);
        printf("------------------------------------------------------\n");
    }
    else
    {
        display_help();
        return 1;
    }

    return 0;
}
