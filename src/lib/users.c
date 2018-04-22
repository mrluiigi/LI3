#include "users.h"

 USERS init_users() {
 	USERS users = malloc(sizeof(struct users));
	users->hash = g_hash_table_new (g_direct_hash, g_direct_equal);
	users->users_by_nr_posts = NULL;
	users->users_by_reputation = NULL;
	return users;
 }

USER_HT  find_user(USERS users, int id) {
	USER_HT u = g_hash_table_lookup(users->hash, GINT_TO_POINTER(id));
	return u;
}

int compare_users_by_reputation (gconstpointer a, gconstpointer b) {
	USER_HT u1 = (USER_HT) a;
	USER_HT u2 = (USER_HT) b;
	if(u1->reputation > u2->reputation) return -1;
	else if(u1->reputation == u2->reputation) return 0;
	else return 1;
}

int compare_users_by_nr_posts (gconstpointer a, gconstpointer b) {
	USER_HT u1 = (USER_HT) a;
	USER_HT u2 = (USER_HT) b;
	if(u1->nr_posts > u2->nr_posts) return -1;
	else if(u1->nr_posts == u2->nr_posts) return 0;
	else return 1;
}


void add_myuser(USERS users, xmlNodePtr ptr) {
	USER_HT u = create_myuser(ptr);
	g_hash_table_insert(users->hash, GINT_TO_POINTER(u->id), u);
	users->users_by_reputation = g_slist_prepend (users->users_by_reputation, u);
	users->users_by_nr_posts = g_slist_prepend(users->users_by_nr_posts, u);
}



void sort_users_by_reputation(USERS users) {
	users->users_by_reputation = g_slist_sort (users->users_by_reputation, compare_users_by_reputation);
}

void sort_users_by_nr_posts(USERS users) {
	users->users_by_nr_posts = g_slist_sort (users->users_by_nr_posts, compare_users_by_nr_posts);
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

void increment_user_nr_posts(USERS users, gpointer id_key) {
	USER_HT user = g_hash_table_lookup(users->hash, id_key);
	user->nr_posts += 1;
	g_hash_table_insert(users->hash, id_key, user);

}

LONG_list get_N_users_with_most_nr_posts(USERS users, int N) {
	int i = 0;
	GSList* l = users->users_by_nr_posts;
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

void find_and_set_user_lastPost(USERS users, gpointer key, GSList * lastpost) {
	if (key) {
		USER_HT u = g_hash_table_lookup(users->hash, key);
		if(u)
			set_user_lastPost(u, lastpost);

	}
}