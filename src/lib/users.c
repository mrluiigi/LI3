#include "users.h"

 USERS init_users() {
 	USERS users = malloc(sizeof(struct users));
	users->hash = g_hash_table_new (g_direct_hash, g_direct_equal);
	users->users_by_nr_posts = NULL;
	users->users_by_reputation = NULL;
	return users;
 }


int compare_users_by_reputation (gconstpointer a, gconstpointer b) {
	USER_HT u1 = (USER_HT) a;
	USER_HT u2 = (USER_HT) b;
	if(u1->reputation > u2->reputation) return -1;
	else if(u1->reputation == u2->reputation) return 0;
	else return 1;
}


void add_myuser(USERS users, xmlNodePtr ptr) {
	USER_HT u = create_myuser(ptr);
	g_hash_table_insert(users->hash, GINT_TO_POINTER(u->id), u);
	users->users_by_reputation = g_slist_prepend (users->users_by_reputation, u);
}



void sort_users_by_reputation(USERS users) {
	users->users_by_reputation = g_slist_sort (users->users_by_reputation, compare_users_by_reputation);
}

LONG_list get_N_users_with_most_reputation(USERS users, int N) {
	int i = 0;
	GSList* l = users->users_by_reputation;
	LONG_list res = create_list(N);
	USER_HT u = NULL;
	while (l && i < N) {
		u = (USER_HT) l->data;
		set_list(res, i, u->id);
		l = l->next;
		i++;
	}
	return res;
}



