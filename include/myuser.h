#ifndef _MYUSER_H_
#define _MYUSER_H_

#include <glib.h>
#include "common.h"
#include "/usr/include/libxml2/libxml/parser.h"
#include "/usr/include/libxml2/libxml/xmlIO.h"

/**
 * Estrutura que guarda informação sobre um user
 */
typedef struct user_ht * MY_USER;

/**
 *  Cria um user com base nos parâmetros recebidos
 */
MY_USER create_myuser(int id, char * name, char * shortBio, int nr_posts, int lastPost, int reputation);

/** 
 * Devolve o ID do user
 */
int get_user_id(MY_USER user);

/**
 * Devolve o nome do utilizador
 * A string retornada foi alocada dinamicamente por isso deve ser libertada usando a função free()
 */
char * get_user_name(MY_USER user);

/**
 * Devolve a shortBio do utilizador
 * A string retornada foi alocada dinamicamente por isso deve ser libertada usando a função free()
 */
char * get_user_shortBio(MY_USER user);

/**
 * Define o lastPost de um utilizador
 */
void set_user_lastPost(MY_USER user, int postId);

/**
 * Devolve o número de posts de um utilizador
 */
int  get_user_nr_posts(MY_USER user);

/**
 * Devolve o lastPost de um utilizador
 */
int get_user_lastPost(MY_USER user);

/**
 * Devolve a reputação de um utilizador
 */
int get_user_reputation(MY_USER user);

/**
 * Incrementa o número de posts de um utilizador
 */
void increment_user_nr_posts(MY_USER user);

/** 
 * Liberta a memória alocada para a struct user_ht
 */
void free_myuser(MY_USER u);

#endif