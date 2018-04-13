#include "posts_list.h"

 
POST create_post(xmlNodePtr ptr, GHashTable* tags) {
	POST p = malloc(sizeof(struct post));
	p->postTypeId = (char *) xmlGetProp(ptr, (xmlChar *) "PostTypeId");
	p->id = atoi((char *) xmlGetProp(ptr, (xmlChar *) "Id"));
	p->ownerUserId = (char *) xmlGetProp(ptr, (xmlChar *) "OwnerUserId");
	p->score = atoi((char *) xmlGetProp(ptr, (xmlChar *) "Score"));
	p->creationDate = xmlCreationDate_to_Date((char*) xmlGetProp(ptr, (xmlChar *) "CreationDate"));
	if (!strcmp(p->postTypeId, "1")) {
		QUESTION q = malloc(sizeof(struct question));
		q->title = (char *) xmlGetProp(ptr, (xmlChar *) "Title");
		q->nanswers = atoi((char *) xmlGetProp(ptr, (xmlChar *) "AnswerCount"));
		q->tags = getTags(tags, (char *) xmlGetProp(ptr, (xmlChar *) "Tags"));
		p->q = q;
	}
	else if (!strcmp(p->postTypeId, "2")) {
		ANSWER a = malloc(sizeof(struct answer));
		a->parentId = atoi ((char *) xmlGetProp(ptr, (xmlChar *) "ParentId"));
		a->comments = atoi ((char *) xmlGetProp(ptr, (xmlChar *) "CommentCount"));
		p->a = a;
	}
	return p;
}


char * get_userId (POST p) {
	return p->ownerUserId;
}

gpointer get_post_key(POST p) {
	return GINT_TO_POINTER(p->id);
}

char * get_title(POST p) {  
	return p->q->title;
}


GSList* find_by_date(GSList* l, GHashTable *monthsHash, Date begin, Date end) {
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
	return (!strcmp(p->postTypeId, "1"));
}

int isAnswer(POST p){
	return (!strcmp(p->postTypeId, "2"));
}