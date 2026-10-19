#include "main.h"
#include "id3_utils.h"

/**
 * Create a memory dynamically for each tag and initializing with NULL  
 */
TagData *create_tag_data()
{
    TagData *data = (TagData *)malloc(sizeof(TagData));
    if (data)
    {
        data->version = NULL;
        data->title = NULL;
        data->artist = NULL;
        data->album = NULL;
        data->year = NULL;
        data->genre = NULL;
        data->comment = NULL;
    }
    return data;
}

/**
 * free each and every tag 
 */
void free_tag_data(TagData *data)
{
    if (data)
    {
        free(data->version);
        free(data->title);
        free(data->artist);
        free(data->album);
        free(data->year);
        free(data->genre);
        free(data->comment);
        free(data);
    }
}
