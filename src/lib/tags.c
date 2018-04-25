#include "tags.h"

static GHashTable * tagshash;

/**
 * Inicializa a HashTable da tags
 */
void init_tags(){
	tagshash = g_hash_table_new (g_str_hash, g_str_equal);
}
/**
 * Insere uma tag na HashTable
 */
void insert_tag(char * tag_name, int tag_id){
	g_hash_table_insert(tagshash, tag_name, GINT_TO_POINTER(tag_id));
}
/**
 * Devolve o ID de uma tag
 */
gpointer convert_tag_name_to_id(char * tag_name){
	gpointer tag_id =  g_hash_table_lookup(tagshash, tag_name);
	return tag_id;
}
/**
* Recebe a string das tags e as coloca numa lista ligada
*/
GSList* getTags(char* tags){
	int i = 0, j=0;
	GSList *l = NULL;
	char res[100];
	gpointer valor;


	while(tags[i] != '\0') {
		i++;
		for (; tags[i] != '>'; i++) {
			res[j] = tags[i];
			j++;
		}
		res[j] = '\0';
		valor = g_hash_table_lookup(tagshash, res);
		l = g_slist_prepend(l, valor);
		j = 0;
		i++;
	}
	return l;
}

void free_tags() {
	g_hash_table_destroy (tagshash);
}