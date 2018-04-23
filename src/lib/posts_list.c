#include "posts_list.h"



char * get_parent_owner( GHashTable* posts, POST p) {
	gpointer parent_key = get_parent_key(p);
	if (!parent_key) return 0;
	POST parent = find_post(posts, get_parent(p));
	if (!parent) return 0;
	return get_ownerUserId(parent);
}

GSList* find_by_date(GSList* l, GHashTable *monthsHash, Date end) {
	int year, month;
	year = get_year(end);
	month = get_month(end);
	if(month < 12)
		month++;
	else{
		year++;
		month = 1;
	}
	if(get_year(get_creationDate((POST) l->data)) == get_year(end)){
		if(get_month(get_creationDate((POST) l->data)) > get_month(end)){
			int key = date_to_Key(year, month);
			l = g_hash_table_lookup(monthsHash, GINT_TO_POINTER(key));
		}
		while(l && compare_date(end, get_creationDate((POST) l->data)) == 1){
			l=l->next;
		}
	}
	else if(get_year(get_creationDate((POST) l->data)) > get_year(end)){
		int key = date_to_Key(year, month);
		l = g_hash_table_lookup(monthsHash, GINT_TO_POINTER(key));
		while(l && compare_date(end, get_creationDate((POST) l->data)) == 1){ 
			l=l->next;
		}
	}
	return l;
}


POST find_post(GHashTable* h, int id) {
	GSList* l = g_hash_table_lookup(h, GINT_TO_POINTER(id));
	if(l) return ((POST)l->data);
	else return NULL;
}

GSList* find_post_in_list(GHashTable* h, int id) {
	GSList* l = g_hash_table_lookup(h, GINT_TO_POINTER(id));
	if(l) return l;
	else return NULL;
}

int compare_nanswers(gconstpointer a, gconstpointer b){
	POST p1 = ((POST) a);
	POST p2 = ((POST) b);

	if(get_nanswers(p1) > get_nanswers(p2))
		return -1;							
	else if (get_nanswers(p1) < get_nanswers(p2)) 
		return 1;
	else 
		return 0;
}