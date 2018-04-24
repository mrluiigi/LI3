#include <glib.h>
#include <string.h>
#include "mydate.h"

//#ifndef HEADER_FILE
//#define HEADER_FILE


GSList* getTags(char* tags);

void init_tags();

gpointer convert_tag_name_to_id(char * tag_name);

void insert_tag(char * tag_name, int tag_id);


//GHashTable* loadTags(GHashTable* tagshash, char *dump_path, char *file);

//#endif
