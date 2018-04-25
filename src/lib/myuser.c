#include "myuser.h"

/**
 * Estrutura que guarda informação sobre um utilizador
 */
struct user_ht{
	/** ID do user */
	int id;
	/** Nome do utilizador */
	char *name;
	/** ShortBio do utilizador */
	char *shortBio;
	/** Número de posts do utilizador */
	unsigned short nr_posts;
	/** Último post do utilizador */
	int lastPost;
	/** Reputação do utilizador */
	int reputation;
};


// GHashTable* usershash, GSList* users_by_nr_posts, GSList* users_by_reputation
/**
 *  Cria um user com base nos parâmetros recebidos
 */
USER_HT create_myuser(int id, char * name, char * shortBio, int nr_posts, int lastPost, int reputation) {
	USER_HT u = malloc(sizeof(struct user_ht));
	u->id = id;
	u->name = mystrdup(name);
	u->shortBio = mystrdup(shortBio);
	u->nr_posts = nr_posts;
	u->lastPost = lastPost;
	u->reputation = reputation;
	return u;
}
/** 
 * Devolve o ID do utilizador
 */
int get_user_id(USER_HT user) {
	if (!user) return 0;
	return user->id;
}
/**
 * Devolve o nome do utilizador
 */
char * get_user_name(USER_HT user) {
	if (!user) return NULL;
	return mystrdup(user->name);
}
/**
 * Devolve a shortBio do utilizador
 */
char * get_user_shortBio(USER_HT user) {
	if (!user) return NULL;
	return mystrdup(user->shortBio);
}
/**
 * Define o lastPost de um utilizador
 */
void set_user_lastPost(USER_HT user, int postId){
	if(user && user->lastPost == 0) user->lastPost = postId;
}
/**
 * Devolve o número de posts de um utilizador
 */
int get_user_nr_posts(USER_HT user) {
	if (!user) return 0;
	return user->nr_posts;
}
/**
 * Devolve o lastPost de um utilizador
 */
int get_user_lastPost(USER_HT user) {
	if (!user) return 0;
	return user->lastPost;
}
/**
 * Devolve a reputação de um utilizador
 */
int get_user_reputation(USER_HT user) {
	if (!user) return 0;
	return user->reputation;
}
/**
 * Incrementa o número de posts de um utilizador
 */
void increment_user_nr_posts(USER_HT user) {
	user->nr_posts++;
}

void free_myuser(USER_HT u) {
	if(u){
		free(u->name);
		free(u->shortBio);
		free(u);
	}
}