#include "post.h"
/**
 * Estrutura que guarda informação sobre um post que é pergunta
 */
typedef struct question{
	/** Título */
	char *title;
	/** Número de respostas */
	int nanswers;
	/** Tags */
	GSList *tags;
	/** Última atividade do post */
	Date lastActivityDate;
}*QUESTION;
/**
 * Estrutura que guarda informação sobre um post que é resposta
 */
typedef struct answer{
	/** ParentID (ID do post a que a resposta pertence) */
	int parentId;
	/** Número de comentários de uma resposta */
	int comments;
	/** Score de uma resposta */
	int score;
}*ANSWER;
/**
 * Estrutura que guarda informação sobre um post
 */
struct post {
	/** Identifica o tipo de post (Pergunta ou Resposta) */
	char postTypeId;
	QUESTION q; 
	ANSWER a; 
	/** ID do post */
	int id;
	/** ID do utilizador que fez o post */
	char * ownerUserId;
	/** Data da criação do post */
	Date creationDate;
};
/**
 * Cria uma pergunta com base nos parâmetros recebidos
 */
POST create_question(char * title, int nanswers, GSList * tags, Date lastActivityDate, char postTypeId, int id, char * ownerUserId, Date creationDate){
	POST p = malloc(sizeof(struct post));
	p->q = malloc(sizeof(struct question));
	p->q->title = mystrdup(title);
	p->q->nanswers = nanswers;
	p->q->tags = tags;
	p->q->lastActivityDate = lastActivityDate;
	p->postTypeId = postTypeId;
	p->id = id;
	p->ownerUserId = mystrdup(ownerUserId);
	p->creationDate = creationDate;
	return p;
}
/**
 * Cria uma resposta com base nos parâmetros recebidos
 */
POST create_answer(int parentId, int comments, int score, char postTypeId, int id, char * ownerUserId, Date creationDate){
	POST p = malloc(sizeof(struct post));
	p->a = malloc(sizeof(struct answer));
	p->a->parentId = parentId;
	p->a->comments = comments;
	p->a->score = score;
	p->postTypeId = postTypeId;
	p->id = id;
	p->ownerUserId = mystrdup(ownerUserId);
	p->creationDate = creationDate;
	return p;
}
/**
 * Devolve o ID de um post
 */
int get_postId(POST p) {
	return (p->id);
}
/**
 * Devolve um ID de um post como um pointer
 */
gpointer get_post_key(POST p) {
	return GINT_TO_POINTER(p->id);
}
/**
 * Devolve o ownerUserId como um pointer
 */
gpointer get_owner_key(POST p) {
	if(!p || !p->ownerUserId) return NULL;
	return GINT_TO_POINTER(atoi(p->ownerUserId));
}
/**
 * Devolve o ownerUserId
 */
char * get_ownerUserId(POST p) {  
	return mystrdup(p->ownerUserId);
}
/**
 * Devolve o parentId de uma resposta como um pointer
 */
gpointer get_parent_key(POST p) {
	if(!p || !p->a || !p->a->parentId) return NULL;
	return GINT_TO_POINTER(p->a->parentId);
}
/**
 * Devolve o parentId de uma resposta
 */
int get_parent(POST p) {
	return p->a->parentId;
}
/**
 * Devolve o número de comentários que uma resposta teve
 */
int get_comments(POST p){
	return p->a->comments;
}
/**
 * Devolve o título de uma pergunta
 */
char * get_title(POST p) {  
	return mystrdup(p->q->title);
}
/**
 * Devolve o número de respostas que uma pergunta tem
 */
int get_nanswers(POST p){
	return p->q->nanswers;
}
/**
 * Devolve as tags de uma pergunta
 */
GSList * get_tags(POST p) {
	return p->q->tags;
}
/**
 * Verifica se um certo post contém uma dada tag
 */
int contains_tag(POST p, gpointer tag_id) {
	if (p->q->tags == NULL) 
		return 0;
	else if (g_slist_find(p->q->tags, tag_id) != NULL) 
		return 1;
	else 
		return 0;
}
/**
 * Verifica se um post é uma pergunta
 */
int isQuestion(POST p){
	return (p->postTypeId == '1');
}
/**
 * Verifica se um post é uma resposta
 */
int isAnswer(POST p){
	return (p->postTypeId == '2');
}

/**
 * Devolve o score de uma resposta
 */
int get_score(POST p){
	return p->a->score;
}
/**
 * Devolve o creationDate de um post
 */
Date get_creationDate(POST p){
	return p->creationDate;
}
/**
 * Devolve o lastActivityDate de uma pergunta
 */
Date get_lastActivityDate(POST p){
	return p->q->lastActivityDate;
}