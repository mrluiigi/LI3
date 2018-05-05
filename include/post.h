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
 * O POST retornado foi alocado dinamicamente por isso deve ser libertado usando a função free_post()
 */
POST create_question(char * title, int nanswers, GSList * tags, Date lastActivityDate, char postTypeId, int id, char * ownerUserId, Date creationDate);

/**
 * Cria uma resposta com base nos parâmetros recebidos
* O POST retornado foi alocado dinamicamente por isso deve ser libertado usando a função free_post()
 */
POST create_answer(int parentId, int comments, int score, char postTypeId, int id, char * ownerUserId, Date creationDate);

/**
 * Devolve o ID de um post
 */
int get_postId(POST p);

/**
 * Devolve o ownerUserId
 * A string retornada foi alocada dinamicamente por isso deve ser libertada usando a função free()
 */
char * get_ownerUserId(POST p);

/**
 * Devolve o ownerUserId em forma de int
 */
int get_ownerUserId_as_int(POST p);

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
 * A string retornada foi alocada dinamicamente por isso deve ser libertada usando a função free()
 */
char * get_title(POST p);

/**
 * Devolve o número de respostas que uma pergunta tem
 */
int get_nanswers(POST p);

/**
 * Devolve as tags de uma pergunta
 * A GSList* retornada foi alocada dinamicamente por isso deve ser libertada usando a função g_slist_free()
 */
GSList * get_tags(POST p);

/**
 * Verifica se um certo post contém uma dada tag
 */
int contains_tag(POST p, gpointer tag_id);

/**
 * Verifica se o post tem um owner definido
 */
int hasOwner(POST p);

/**
 * Verifica se o id dado corresponde ao owner do post
 */
int isOwner(POST p, int id);

/**
 * Verifica se um post é uma pergunta
 */
int isQuestion(POST p);

/**
 * Verifica se um post é uma resposta
 */
int isAnswer(POST p);

/**
 * Devolve o score de uma resposta
 */
int get_score(POST p);

/**
 * Devolve o creationDate de um post
 * A Date retornada foi alocada dinamicamente por isso deve ser libertada usando a função free_date()
 */
Date get_creationDate(POST p);

/**
 * Devolve o lastActivityDate de uma pergunta
 * A Date retornada foi alocada dinamicamente por isso deve ser libertada usando a função free_date()
 */
Date get_lastActivityDate(POST p);

/**
 * Liberta a memória alocada para um post
 */
void free_post(POST p);

#endif