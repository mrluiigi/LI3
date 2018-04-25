#ifndef _POST_H_
#define _POST_H_

#include <glib.h>
#include <string.h>
#include "date.h"
#include "mydate.h"
#include "tags.h"
#include "common.h"
#include "/usr/include/libxml2/libxml/parser.h"
#include "/usr/include/libxml2/libxml/xmlIO.h"

/**
 * Estrutura que guarda informação sobre um post
 */
typedef struct post * POST;
/**
 * Cria uma pergunta com base nos parâmetros recebidos
 */
POST create_question(char * title, int nanswers, GSList * tags, Date lastActivityDate, char postTypeId, int id, char * ownerUserId, Date creationDate);
/**
 * Cria uma resposta com base nos parâmetros recebidos
 */
POST create_answer(int parentId, int comments, int score, char postTypeId, int id, char * ownerUserId, Date creationDate);
/**
 * Devolve o ID de um post
 */
int get_postId(POST p);
/**
 * Devolve um ID de um post como um pointer
 */
gpointer get_post_key(POST p);
/**
 * Devolve o ownerUserId como um pointer
 */
gpointer get_owner_key(POST p);
/**
 * Devolve o ownerUserId
 */
char * get_ownerUserId(POST p);
/**
 * Devolve o parentId de uma resposta como um pointer
 */
gpointer get_parent_key(POST p);
/**
 * Devolve o parentId de uma resposta
 */
int get_parent(POST p);
/**
 * Devolve o número de comentários que uma resposta teve
 */
int get_comments(POST p);
/**
 * Devolve o título de uma pergunta
 */
char * get_title(POST p);
/**
 * Devolve o número de respostas que uma pergunta tem
 */
int get_nanswers(POST p);
/**
 * Devolve as tags de uma pergunta
 */
GSList * get_tags(POST p);
/**
 * Verifica se um certo post contém uma dada tag
 */
int contains_tag(POST p, gpointer tag_id);
/**
 * Verifica se um post é uma pergunta
 */
int isQuestion(POST p);
/**
 * Verifica se um post é uma resposta
 */
int isAnswer(POST p);
/**
 * Incrementa o número de upVotes de uma resposta
 */
void addUpVote(POST p);
/**
 * Incrementa o número de downVotes de uma resposta
 */
void addDownVote(POST p);
/**
 * Devolve o número de upVotes de uma resposta
 */
int get_upvotes(POST p);
/**
 * Devolve o número de downVotes de uma resposta
 */
int get_downvotes(POST p);
/**
 * Devolve o score de uma resposta
 */
int get_score(POST p);
/**
 * Devolve o creationDate de um post
 */
Date get_creationDate(POST p);
/**
 * Devolve o lastActivityDate de uma pergunta
 */
Date get_lastActivityDate(POST p);
#endif