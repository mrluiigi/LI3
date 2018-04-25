#include <glib.h>
#include "common.h"
#include "/usr/include/libxml2/libxml/parser.h"
#include "/usr/include/libxml2/libxml/xmlIO.h"


//#ifndef HEADER_FILE
//#define HEADER_FILE

/**
 * Estrutura que guarda informação sobre um user
 */
typedef struct user_ht * USER_HT;

/**
 *  Cria um user com base nos parâmetros recebidos
 */
USER_HT create_myuser(int id, char * name, char * shortBio, int nr_posts, int lastPost, int reputation);
/** 
 * Devolve o ID do user
 */
int get_user_id(USER_HT user);
/**
 * Devolve o nome do utilizador
 */
char * get_user_name(USER_HT user);
/**
 * Devolve a shortBio do utilizador
 */
char * get_user_shortBio(USER_HT user);
/**
 * Define o lastPost de um utilizador
 */
void set_user_lastPost(USER_HT user, int postId);
/**
 * Devolve o número de posts de um utilizador
 */
int  get_user_nr_posts(USER_HT user);
/**
 * Devolve o lastPost de um utilizador
 */
int get_user_lastPost(USER_HT user);
/**
 * Devolve a reputação de um utilizador
 */
int get_user_reputation(USER_HT user);
/**
 * Incrementa o número de posts de um utilizador
 */
void increment_user_nr_posts(USER_HT user);

void free_myuser(USER_HT u);

//#endif