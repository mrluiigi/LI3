#include "tags.h"


/**
 * Inicializa a HashTable da tags
 */
TAGS init_tags(){
	return  g_hash_table_new_full (g_str_hash, g_str_equal, free, NULL);
}
/**
 * Insere uma tag na HashTable
 */
void insert_tag(TAGS t, char * tag_name, int tag_id){
	char * name = mystrdup(tag_name);
	g_hash_table_insert(t, name, GINT_TO_POINTER(tag_id));
}
/**
 * Devolve o ID de uma tag
 */
gpointer convert_tag_name_to_id(TAGS t, char * tag_name){
	gpointer tag_id =  g_hash_table_lookup(t, tag_name);
	return tag_id;
}
/**
* Recebe a string das tags e as coloca numa lista ligada
*/
GSList* getTags(TAGS t,char* tags){
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
		valor = g_hash_table_lookup(t, res);
		l = g_slist_prepend(l, valor);
		j = 0;
		i++;
	}
	return l;
}

/**
* Liberta a memória alocada para a struct tags
*/
void free_tags(TAGS t) {
	g_hash_table_destroy (t);
}