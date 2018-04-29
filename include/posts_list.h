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

/**
* Estrutura que guarda informação sobre um post
*/
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

/**
 *	Função auxiliar para a query 9 que adiciona uma resposta à queue
 */
char * get_parent_owner(Posts posts, POST p);

/**
 * Função que procura numa lista de datas uma data específica
 * A função devolve essa mesma lista a partir da data que lhe foi passada
 */
PostsList find_by_date(Posts posts, Date date);

/**
 * Função que procura numa lista de posts aquele que corresponde ao ID que lhe é passado como argumento
 * Devolve o post com o ID correspondente
 */
POST find_post(Posts posts, int id);

/**
 * Função que procura numa lista de posts aquele que corresponde ao ID que lhe é passado como argumento
 * Devolve todos os posts a seguir ao post encontrado
 */
PostsList  find_post_in_list(Posts posts, int id);

/**
 * Retorna -1 se o primeiro post tiver mais respostas, 1 se tiver menos respostas 0 se tiverem o mesmo número de respostas
 */
int compare_nanswers(gconstpointer a, gconstpointer b);

/**
* Compara as datas do primeiro elemnto de duas listas
*/
int compare_date_list (gconstpointer a, gconstpointer b);

/**
* Devolve a lista com os posts
*/
PostsList get_posts_list(Posts posts);

/**
* Avança para o post seguinte
*/
PostsList get_next(PostsList pl);

/**
* Devolve a data de um certo post
*/
POST get_post(PostsList pl);

/**
* Dadas duas listas com posts a função retorna a lista com o post mais recente
*/
PostsList find_most_recent_post(PostsList l1, PostsList l2);


//int compare_score(gconstpointer a, gconstpointer b);

/**
* Liberta a memória alocada para a struct posts
*/
void free_posts(Posts posts);
#endif