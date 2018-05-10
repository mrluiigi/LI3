#ifndef _TAGS_H_
#define _TAGS_H_

#include <glib.h>
#include <string.h>
#include "mydate.h"
#include "common.h"

typedef GHashTable * TAGS;

/**
 * Inicializa as TAGS
 */
TAGS init_tags();

/**
 * Insere uma tag
 */
void insert_tag(TAGS t, char * tag_name, int tag_id);

/**
 * Devolve o ID de uma tag
 */
gpointer convert_tag_name_to_id(TAGS t, char * tag_name);

/**
* Recebe a string das tags e as coloca numa lista ligada
*/
GSList* getTags(TAGS t, char* tags);

/**
 * Liberta a memória alocada para as tags
 */
void free_tags(TAGS t);
#endif