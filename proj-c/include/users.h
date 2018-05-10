#ifndef _USERS_H_
#define _USERS_H_

#include <glib.h>
#include "/usr/include/libxml2/libxml/parser.h"
#include "/usr/include/libxml2/libxml/xmlIO.h"
#include "myuser.h"
#include "common.h"
#include "list.h"

/**
 * Estrutura que guarda informação sobre os users
 */
typedef struct users * USERS;

/**
 * Inicializa a estrutura dos users
 */
USERS init_users();

/**
 *Função a chamar depois de inseridos todos os users para garantir o ordenamento correto destes de acordo com o número de posts e de acordo com a reputação
 */
void finalize_users(USERS users);
/**
 * Encontra um determinado user dado o seu ID
 */
MY_USER  find_user(USERS users, int id);

/**
 * Insere um user à estrutura dos users
 */
void add_myuser(USERS users, int id, char * name, char * shortBio, int nr_posts, int lastPost, int reputation);

/**
 * Remove um user da estrutura dos users
 */
void remove_myuser(USERS users, int id);

/**
 * Devolve a lista com os N users com maior reputação
 */
LONG_list get_N_users_with_most_reputation(USERS users, int N);

/**
 * Devolve uma lista com os users que tem o maior número de posts
 */
LONG_list get_N_users_with_most_nr_posts(USERS users, int N);

/**
 * Incrementa o número de posts de um determinado user
 */
void find_and_increment_user_nr_posts(USERS users, int id_key);


/**
 * Define o lastPost de uma dado user
 */
void find_and_set_user_lastPost(USERS users, int id, int lastPostId);

/**
 * Liberta a memória alocada para os users
 */
void free_users(USERS users);

#endif