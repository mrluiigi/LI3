#include "tags.h"
/**
* Função que recebe a string das tags e as coloca numa lista ligada
*/
GSList* getTags(GHashTable *h, char* tags){
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
		valor = g_hash_table_lookup(h, res);
		l = g_slist_prepend(l, valor);
		j = 0;
		i++;
	}
	return l;
}
