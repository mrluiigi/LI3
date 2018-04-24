#include "post.h"

typedef struct question{
	char *title;
	int nanswers;
	GSList *tags;
	Date lastActivityDate;
}*QUESTION;

typedef struct answer{
	int parentId;
	int comments;
	int upVotes;
	int downVotes;
}*ANSWER;

struct post {
	char postTypeId;
	QUESTION q; 
	ANSWER a; 
	int id;
	char * ownerUserId;
	Date creationDate;
};

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

POST create_answer(int parentId, int comments, int upVotes, int downVotes, char postTypeId, int id, char * ownerUserId, Date creationDate){
	POST p = malloc(sizeof(struct post));
	p->a = malloc(sizeof(struct answer));
	p->a->parentId = parentId;
	p->a->comments = comments;
	p->a->upVotes = upVotes;
	p->a->downVotes = downVotes;
	p->postTypeId = postTypeId;
	p->id = id;
	p->ownerUserId = mystrdup(ownerUserId);
	p->creationDate = creationDate;
	return p;
}

int get_postId(POST p) {
	return (p->id);
}

int get_comments(POST p){
	return p->a->comments;
}

gpointer get_post_key(POST p) {
	return GINT_TO_POINTER(p->id);
}

gpointer get_owner_key(POST p) {
	if(!p || !p->ownerUserId) return NULL;
	return GINT_TO_POINTER(atoi(p->ownerUserId));
}

gpointer get_parent_key(POST p) {
	if(!p || !p->a || !p->a->parentId) return NULL;
	return GINT_TO_POINTER(p->a->parentId);
}

int get_parent(POST p) {
	return p->a->parentId;
}

char * get_title(POST p) {  
	return mystrdup(p->q->title);
}

char * get_ownerUserId(POST p) {  
	return mystrdup(p->ownerUserId);
}

int get_nanswers(POST p){
	return p->q->nanswers;
}

GSList * get_tags(POST p) {
	return p->q->tags;
}

int contains_tag(POST p, gpointer tag_id) {
	if (p->q->tags == NULL) 
		return 0;
	else if (g_slist_find(p->q->tags, tag_id) != NULL) 
		return 1;
	else 
		return 0;
}


int isQuestion(POST p){
	return (p->postTypeId == '1');
}

int isAnswer(POST p){
	return (p->postTypeId == '2');
}

void addUpVote(POST p){
	p->a->upVotes += 1;
}

void addDownVote(POST p){
	p->a->downVotes += 1;
}

int get_upvotes(POST p){
	return p->a->upVotes;
}

int get_downvotes(POST p){
	return p->a->downVotes;
}

int get_score(POST p){
	return p->a->upVotes - p->a->downVotes;
}

Date get_creationDate(POST p){
	return p->creationDate;
}

Date get_lastActivityDate(POST p){
	return p->q->lastActivityDate;
}

