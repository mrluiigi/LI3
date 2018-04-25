#ifndef _TAGS_H_
#define _TAGS_H_

#include <glib.h>
#include <string.h>
#include "mydate.h"

/**
 * Inicializa a HashTable da tags
 */
void init_tags();
/**
 * Insere uma tag na HashTable
 */
void insert_tag(char * tag_name, int tag_id);
/**
 * Devolve o ID de uma tag
 */
gpointer convert_tag_name_to_id(char * tag_name);
/**
* Recebe a string das tags e as coloca numa lista ligada
*/
GSList* getTags(char* tags);
//GHashTable* loadTags(GHashTable* tagshash, char *dump_path, char *file);
#endif