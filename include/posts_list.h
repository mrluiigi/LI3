#ifndef _POSTS_LIST_H_
#define _POSTS_LIST_H_

#include <glib.h>
#include <string.h>
#include "date.h"
#include "mydate.h"
#include "tags.h"
#include "post.h"
#include "/usr/include/libxml2/libxml/parser.h"
#include "/usr/include/libxml2/libxml/xmlIO.h"

/**
 * Estrutura que guarda informação sobre um post
 */
typedef struct TCD_posts * Posts;

typedef GSList * PostsList;
/**
 * Inicializa a estrutura dos posts
 */
Posts init_posts(Posts posts);
/**
 * Adiciona uma pergunta aos posts
 */
void add_question_to_posts(Posts posts, char * title, int nanswers, GSList * tags, Date lastActivityDate, char postTypeId, int id, char * ownerUserId, Date creationDate);
/**
 * Adiciona uma resposta aos posts
 */
void add_answer_to_posts(Posts posts, int parentId, int comments, int score, char postTypeId, int id, char * ownerUserId, Date creationDate);

void finalize(Posts posts);

char * get_parent_owner(Posts posts, POST p);

PostsList find_by_date(Posts posts, Date date);

POST find_post(Posts posts, int id);

PostsList  find_post_in_list(Posts posts, int id);

int compare_nanswers(gconstpointer a, gconstpointer b);

int compare_date_list (gconstpointer a, gconstpointer b);

PostsList get_posts_list(Posts posts);

PostsList get_next(PostsList pl);

POST get_post(PostsList pl);

PostsList find_most_recent_post(PostsList l1, PostsList l2);

//int compare_score(gconstpointer a, gconstpointer b);

void free_posts(Posts posts);
#endif