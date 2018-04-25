#include "users.h"

static USERS users;

/**
 * Estrutura que guarda informação sobre os users
 */
struct users {
	GHashTable* hash;
	GSList* users_by_nr_posts;
	GSList* users_by_reputation;
};
/**
 * Liberta a informação de um user
 */
void free_user_hash(gpointer data) {
	free_myuser((USER_HT) data);
} 
/**
 * Inicializa a estrutura dos users
 */
void init_users() {
	users = malloc(sizeof(struct users));
	users->hash = g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, free_user_hash);
	users->users_by_nr_posts = NULL;
	users->users_by_reputation = NULL;
}
/**
 * Encontra um determinado user dado o seu ID
 */
USER_HT  find_user(int id) {
	USER_HT u = g_hash_table_lookup(users->hash, GINT_TO_POINTER(id));
	return u;
}
/**
 * Insere um user à estrutura dos users
 */
void add_myuser(int id, char * name, char * shortBio, int nr_posts, int lastPost, int reputation) {
	USER_HT u = create_myuser(id, name, shortBio, nr_posts, lastPost, reputation);
	g_hash_table_insert(users->hash, GINT_TO_POINTER(get_user_id(u)), u);
	users->users_by_reputation = g_slist_prepend (users->users_by_reputation, u);
	users->users_by_nr_posts = g_slist_prepend(users->users_by_nr_posts, u);
}
/**
 * Função para comparar a reputação de dois utilizadores
 */
int compare_users_by_reputation (gconstpointer a, gconstpointer b) {
	USER_HT u1 = (USER_HT) a;
	USER_HT u2 = (USER_HT) b;
	if(get_user_reputation(u1) > get_user_reputation(u2)) return -1;
	else if(get_user_reputation(u1) == get_user_reputation(u2)) return 0;
	else return 1;
}
/**
 * Função para comparar o número de posts de dois utilizadores
 */
int compare_users_by_nr_posts (gconstpointer a, gconstpointer b) {
	USER_HT u1 = (USER_HT) a;
	USER_HT u2 = (USER_HT) b;
	if(get_user_nr_posts(u1) > get_user_nr_posts(u2)) return -1;
	else if(get_user_nr_posts(u1) == get_user_nr_posts(u2)) return 0;
	else return 1;
}
/**
 * Ordena a lista dos users_by_reputation por ordem decrescente de reputação
 */
void sort_users_by_reputation() {
	users->users_by_reputation = g_slist_sort (users->users_by_reputation, compare_users_by_reputation);
}
/**
 * Ordena a lista dos users_by_nr_posts por ordem decrecente de número de posts
 */
void sort_users_by_nr_posts() {
	users->users_by_nr_posts = g_slist_sort (users->users_by_nr_posts, compare_users_by_nr_posts);
}
/**
 * Devolve uma lista com os N users que tem maior reputação
 */
LONG_list get_N_users_with_most_reputation(int N) {
	int i = 0;
	GSList* l = users->users_by_reputation;
	LONG_list res = create_list(N);
	USER_HT u = NULL;
	while (l && i < N) {
		u = (USER_HT) l->data;
		set_list(res, i, get_user_id(u));
		l = l->next;
		i++;
	}
	return res;
}
/**
 * Incrementa o número de posts de um determinado user
 */
void find_and_increment_user_nr_posts(gpointer id_key) {
	USER_HT user = g_hash_table_lookup(users->hash, id_key);
	increment_user_nr_posts(user);
}
/**
 * Devolve uma lista com os users que tem o maior número de posts
 */
LONG_list get_N_users_with_most_nr_posts(int N) {
	int i = 0;
	GSList* l = users->users_by_nr_posts;
	LONG_list res = create_list(N);
	USER_HT u = NULL;
	while (l && i < N) {
		u = (USER_HT) l->data;
		set_list(res, i, get_user_id(u));
		l = l->next;
		i++;
	}
	return res;
}
/**
 * Define o lastPost de uma dado user
 */
void find_and_set_user_lastPost(gpointer key, int lastPostId) {
	if (key) {
		USER_HT u = g_hash_table_lookup(users->hash, key);
		if(u)
			set_user_lastPost(u, lastPostId);

	}
}

void free_users() {
	if (users) {
		g_hash_table_destroy (users->hash);
		g_slist_free (users->users_by_reputation);
		g_slist_free (users->users_by_nr_posts);
	}
}