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
 * Encontra um determinado user dado o seu ID
 */
USER_HT  find_user(USERS users, int id);
/**
 * Insere um user à estrutura dos users
 */
void add_myuser(USERS users, int id, char * name, char * shortBio, int nr_posts, int lastPost, int reputation);
/**
 * Ordena a lista dos users_by_reputation por ordem decrescente de reputação
 */
void sort_users_by_reputation(USERS users);
/**
 * Ordena a lista dos users_by_nr_posts por ordem decrecente de número de posts
 */
void sort_users_by_nr_posts(USERS users);
/**
 * Devolve a lista com os N users com maior reputação
 */
LONG_list get_N_users_with_most_reputation(USERS users, int N);
/**
 * Incrementa o número de posts de um determinado user
 */
void find_and_increment_user_nr_posts(USERS users, gpointer id_key);
/**
 * Devolve uma lista com os users que tem o maior número de posts
 */
LONG_list get_N_users_with_most_nr_posts(USERS users, int N);
/**
 * Define o lastPost de uma dado user
 */
void find_and_set_user_lastPost(USERS users, gpointer key, int lastPostId);

void free_users(USERS users);
#endif