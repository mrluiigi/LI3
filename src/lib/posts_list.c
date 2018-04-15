#include "posts_list.h"

 
POST create_post(xmlNodePtr ptr, GHashTable* tags) {
	POST p = malloc(sizeof(struct post));
	p->postTypeId = ((char *) xmlGetProp(ptr, (xmlChar *) "PostTypeId"))[0];
	p->id = atoi((char *) xmlGetProp(ptr, (xmlChar *) "Id"));
	p->ownerUserId = (char *) xmlGetProp(ptr, (xmlChar *) "OwnerUserId");
	p->score = atoi((char *) xmlGetProp(ptr, (xmlChar *) "Score"));
	p->creationDate = xmlCreationDate_to_Date((char*) xmlGetProp(ptr, (xmlChar *) "CreationDate"));
	if (p->postTypeId == '1') {
		QUESTION q = malloc(sizeof(struct question));
		q->title = (char *) xmlGetProp(ptr, (xmlChar *) "Title");
		q->nanswers = atoi((char *) xmlGetProp(ptr, (xmlChar *) "AnswerCount"));
		q->tags = getTags(tags, (char *) xmlGetProp(ptr, (xmlChar *) "Tags"));
		p->q = q;
	}
	else if (p->postTypeId == '2') {
		ANSWER a = malloc(sizeof(struct answer));
		a->parentId = atoi ((char *) xmlGetProp(ptr, (xmlChar *) "ParentId"));
		a->comments = atoi ((char *) xmlGetProp(ptr, (xmlChar *) "CommentCount"));
		p->a = a;
	}
	return p;
}

int get_postId(POST p) {
	return (p->id);
}

gpointer get_post_key(POST p) {
	return GINT_TO_POINTER(p->id);
}

gpointer get_owner_key(POST p) {
	return GINT_TO_POINTER(atoi(p->ownerUserId));
}

gpointer get_parent_key(POST p) {
	return GINT_TO_POINTER(p->a->parentId);
}

char * get_title(POST p) {  
	return p->q->title;
}

char * get_ownerUserId(POST p) {  
	return p->ownerUserId;
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


GSList* find_by_date(GSList* l, GHashTable *monthsHash, Date end) {
	int year, month;
	year = get_year(end);
	month = get_month(end);
	if(month < 12)
		month++;
	else{
		year++;
		month = 1;
	}
	if(get_year(((POST) l->data)->creationDate) == get_year(end)){
		if(get_month(((POST) l->data)->creationDate) > get_month(end)){
			int key = date_to_Key(year, month);
			l = g_hash_table_lookup(monthsHash, GINT_TO_POINTER(key));
		}
		while(l && compare_date(end, ((POST) l->data)->creationDate) == 1){
			l=l->next;
		}
	}
	else if(get_year(((POST) l->data)->creationDate) > get_year(end)){
		int key = date_to_Key(year, month);
		l = g_hash_table_lookup(monthsHash, GINT_TO_POINTER(key));
		while(l && compare_date(end, ((POST) l->data)->creationDate) == 1){ 
			l=l->next;
		}
	}
	return l;
}


int isQuestion(POST p){
	return (p->postTypeId == '1');
}

int isAnswer(POST p){
	return (p->postTypeId == '2');
}