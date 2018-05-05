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
	int nr_posts;
	/** Último post do utilizador */
	int lastPost;
	/** Reputação do utilizador */
	int reputation;
};


// GHashTable* usershash, GSList* users_by_nr_posts, GSList* users_by_reputation
/**
 *  Cria um user com base nos parâmetros recebidos
 */
MY_USER create_myuser(int id, char * name, char * shortBio, int nr_posts, int lastPost, int reputation) {
	MY_USER u = malloc(sizeof(struct user_ht));
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
int get_user_id(MY_USER user) {
	if (!user) return 0;
	return user->id;
}

/**
 * Devolve o nome do utilizador
 * A string retornada foi alocada dinamicamente por isso deve ser libertada usando a função free()
 */
char * get_user_name(MY_USER user) {
	if (!user) return NULL;
	return mystrdup(user->name);
}

/**
 * Devolve a shortBio do utilizador
 * A string retornada foi alocada dinamicamente por isso deve ser libertada usando a função free()
 */
char * get_user_shortBio(MY_USER user) {
	if (!user) return NULL;
	return mystrdup(user->shortBio);
}

/**
 * Define o lastPost de um utilizador
 */
void set_user_lastPost(MY_USER user, int postId){
	if(user && user->lastPost == 0) user->lastPost = postId;
}

/**
 * Devolve o número de posts de um utilizador
 */
int get_user_nr_posts(MY_USER user) {
	if (!user) return 0;
	return user->nr_posts;
}

/**
 * Devolve o lastPost de um utilizador
 */
int get_user_lastPost(MY_USER user) {
	if (!user) return 0;
	return user->lastPost;
}

/**
 * Devolve a reputação de um utilizador
 */
int get_user_reputation(MY_USER user) {
	if (!user) return 0;
	return user->reputation;
}

/**
 * Incrementa o número de posts de um utilizador
 */
void increment_user_nr_posts(MY_USER user) {
	user->nr_posts++;
}

/** 
 * Liberta a memória alocada para a struct user_ht
 */
void free_myuser(MY_USER u) {
	if(u){
		free(u->name);
		free(u->shortBio);
		free(u);
	}
}