#include <interface.h>
#include <glib.h>
#include <stdio.h>
#include "/usr/include/libxml2/libxml/parser.h"
#include "/usr/include/libxml2/libxml/xmlIO.h"
#include <string.h>
#include "date.h"
#include "mydate.h"
#include "posts_list.h"
#include "tags.h"


struct TCD_community{
	GHashTable *postshash;
	GHashTable *usershash;
	GHashTable *tagshash;
	GSList *posts;
	GHashTable *monthsHash;         //guarda o primeiro posts de cada mes
};


typedef struct user_ht{
	char *name;
	char *shortBio;
	unsigned short nr_posts;
	GSList *lastPost;
	int reputation;
}*USER_HT;

typedef struct user_ll{
	int id;
	unsigned short nr_posts;
}*USER_LL;

/*
* Compara as datas do primeiro elemnto de duas listas
*/
int compare_date_list (gconstpointer a, gconstpointer b) {
	POST p1 = (POST) a;
	POST p2 = (POST) b;

	return compare_date(p1->creationDate, p2->creationDate);
}

/**
*	Função que inicializa a estrutura de dados
*/
TAD_community init(){
	TAD_community t;
	t = malloc(sizeof(struct TCD_community));
	t->postshash = g_hash_table_new (g_direct_hash, g_direct_equal);
	t->usershash = g_hash_table_new (g_direct_hash, g_direct_equal);
	t->posts = NULL;
	t->tagshash = g_hash_table_new (g_str_hash, g_str_equal);
	t->monthsHash = g_hash_table_new (g_direct_hash, g_direct_equal);
	return t;
}

/**
 *	Função para concatenar o file ao dump_path	
 */
char* filePath(char *dump_path, char *file){
	size_t lengthDump = strlen(dump_path);
	size_t lengthFile = strlen(file);
	char path[lengthDump + lengthFile];
	strcpy(path, dump_path);
	char *res = strcat(path, file);
	return res;
}

/**
* Função que faz load ao file Users.xml
*/
void loadUsers(TAD_community com, char *dump_path, char *file){
	xmlDocPtr doc;
	xmlNodePtr cur, ptr;

	char file_users[100];
	strcpy(file_users, filePath(dump_path, file));

	doc = xmlParseFile(file_users);

	if(doc == NULL){
		fprintf(stderr, "Document not parsed successfully.\n");
		return;
	}

	cur = xmlDocGetRootElement(doc);

	if(cur == NULL){
		fprintf(stderr, "empty document\n");
		xmlFreeDoc(doc);
	}
	char *key_users;

	ptr = cur->xmlChildrenNode;
	ptr = ptr->next;
	while(ptr != NULL){
		USER_HT u = malloc(sizeof(struct post));
		key_users = (char *) xmlGetProp(ptr, (xmlChar *) "Id");
		int r_users = atoi(key_users);
		
		u->name = (char *) xmlGetProp(ptr, (xmlChar *) "DisplayName");
		u->shortBio = (char *) xmlGetProp(ptr, (xmlChar *) "AboutMe");
		u->nr_posts = 0;
		u->lastPost = NULL;
		u->reputation = atoi((char*) xmlGetProp(ptr, (xmlChar *) "Reputation"));
		
		g_hash_table_insert(com->usershash, GINT_TO_POINTER(r_users), u);
		ptr = ptr->next->next;

	}

	xmlFreeDoc(doc);
}


void set_userht_lastPost(USER_HT user, GSList* l){
	if(user->lastPost == NULL){
		user->lastPost = l;
	}
}


/**
	Função que faz load ao file Posts.xml
*/
void loadPosts(TAD_community com, char *dump_path, char *file){
	xmlDocPtr doc;
	xmlNodePtr cur, ptr;
	//Concatena o file ao dump_path
	char file_posts[100];
	strcpy(file_posts, filePath(dump_path, file));
	//-------------------------
	doc = xmlParseFile(file_posts);
	//returns!
	if(doc == NULL){
		fprintf(stderr, "Document not parsed successfully.\n");
		return;
	}
	cur = xmlDocGetRootElement(doc);

	//returns!
	if(cur == NULL){
		fprintf(stderr, "empty document\n");
		xmlFreeDoc(doc);
	}
	
	int posts_size = 0;

	ptr = cur->xmlChildrenNode;
	ptr = ptr->next;

	while(ptr != NULL){
		char * postTypeId = (char *) xmlGetProp(ptr, (xmlChar *) "PostTypeId");
		if((strcmp(postTypeId, "1")==0) || (strcmp(postTypeId, "2")==0)){
			POST p = create_post(ptr, com->tagshash);
			if(get_ownerUserId(p) != NULL){
				gpointer owner_key = get_owner_key(p);
				USER_HT user = g_hash_table_lookup(com->usershash, owner_key);
				user->nr_posts += 1;
				g_hash_table_insert(com->usershash, owner_key, user);
			}
			
			com->posts = g_slist_prepend(com->posts, p);
			posts_size++;
			
			g_hash_table_insert(com->postshash, get_post_key(p), p);
		}
		ptr = ptr->next->next;
	}

	GSList* l;

	xmlFreeDoc(doc);
	com->posts = g_slist_sort (com->posts, compare_date_list);
	for(l = com->posts; l; l = l->next){
		USER_HT u = g_hash_table_lookup(com->usershash, get_owner_key((POST) l->data));
		set_userht_lastPost(u, l);
	}
	GSList* li = com->posts;
	for(int i = 0; i < posts_size; i++) {
		Date d = ( (POST) li->data)->creationDate;
		int months_key = date_to_Key(get_year(d), get_month(d)) ;
		g_hash_table_insert(com->monthsHash, GINT_TO_POINTER(months_key), li);
		li = g_slist_next(li);
	}
}

/**
* Função que faz load ao file Tags.xml
*/
void loadTags(TAD_community com, char *dump_path, char *file){
	xmlDocPtr doc;
	xmlNodePtr cur, ptr;

	//Concatena o file ao dump_path
	char file_tags[100];
	strcpy(file_tags, filePath(dump_path, file));
	//-------------------------
	doc = xmlParseFile(file_tags);

	//returns!
	if(doc == NULL){
		fprintf(stderr, "Document not parsed successfully.\n");
		return;
	}

	cur = xmlDocGetRootElement(doc);

	//returns!
	if(cur == NULL){
		fprintf(stderr, "empty document\n");
		xmlFreeDoc(doc);
	}
	
	ptr = cur->xmlChildrenNode;
	ptr = ptr->next;

	while(ptr != NULL){
		g_hash_table_insert(com->tagshash, (char *) xmlGetProp(ptr, (xmlChar *) "TagName"), GINT_TO_POINTER(atoi((char *) xmlGetProp(ptr, (xmlChar *) "Id"))));
		ptr = ptr->next->next;
	}
	xmlFreeDoc(doc);
}

void loadVotes(TAD_community com, char *dump_path, char *file){
	xmlDocPtr doc;
	xmlNodePtr cur, ptr;

	char file_tags[100];
	strcpy(file_tags, filePath(dump_path, file));
	//-------------------------
	doc = xmlParseFile(file_tags);


	if(doc == NULL){
		fprintf(stderr, "Document not parsed successfully.\n");
		return;
	}

	cur = xmlDocGetRootElement(doc);

	if(cur == NULL){
		fprintf(stderr, "empty document\n");
		xmlFreeDoc(doc);
	}
	
	ptr = cur->xmlChildrenNode;
	ptr = ptr->next;
	
	while(ptr != NULL){
		char* voteTypeId = (char *) xmlGetProp(ptr, (xmlChar *) "VoteTypeId");
		if((strcmp(voteTypeId, "2") == 0) || (strcmp(voteTypeId, "3") == 0)){
			char* postId = (char *) xmlGetProp(ptr, (xmlChar *) "PostId");
			POST p = g_hash_table_lookup(com->postshash, GINT_TO_POINTER(atoi(postId)));
			if(p != NULL && isAnswer(p)){
				if(strcmp(voteTypeId, "2") == 0){
					addUpVote(p);				
				}
				else{
					addDownVote(p);
				}
			}
		}
		ptr = ptr->next->next;
	}
	xmlFreeDoc(doc);
}

/**
  Função que faz load dos ficheiros para a estrutura de dados
*/
TAD_community load(TAD_community com, char* dump_path){
	
	loadTags(com, dump_path, "Tags.xml");

	loadUsers(com, dump_path, "Users.xml");

	loadPosts(com, dump_path, "Posts.xml");

	loadVotes(com, dump_path, "Votes.xml");

	return com;
}

////////////////////

/**
  QUERY 1
*/
STR_pair info_from_post(TAD_community com, long id){
	POST p;
	USER_HT u;

	STR_pair pair;

	p = g_hash_table_lookup(com->postshash, GINT_TO_POINTER(id));
	
	//Caso nenhum post tenha o id procurado
	if(p==NULL){
		fprintf(stderr, "Id não válido\n");
		return NULL;
	} 
	if(isQuestion(p)){
		u = g_hash_table_lookup(com->usershash, get_owner_key(p));
		pair = create_str_pair(get_title(p), u->name);
	}
	else if(isAnswer(p)){
		p = g_hash_table_lookup(com->postshash, get_parent_key(p));

		u = g_hash_table_lookup(com->usershash, get_owner_key(p));
		pair = create_str_pair(get_title(p), u->name);	

	}
	return pair;
}

void slist_append(gpointer key, gpointer value, gpointer *ll_pt){
	USER_HT uht = value;
	USER_LL user = malloc(sizeof(struct user_ll));
	user->id = GPOINTER_TO_INT(key);
	user->nr_posts = uht->nr_posts;
	*ll_pt = g_slist_prepend(*ll_pt, user);
}

gint compare_user_ll(gconstpointer g1, gconstpointer g2){
	USER_LL u1 = ((USER_LL) g1);
	USER_LL u2 = ((USER_LL) g2);
	return u2->nr_posts - u1->nr_posts;
}

/**
  QUERY 2
*/
LONG_list top_most_active(TAD_community com, int N){
	GSList * ll = NULL;
	USER_LL cur;
	LONG_list lista;
	lista = create_list(N);
	g_hash_table_foreach(com->usershash, (GHFunc)slist_append, (gpointer)&ll);
	ll = g_slist_sort(ll, (GCompareFunc)compare_user_ll);
	for(int i = 0; i < N; i++){
		cur = g_slist_nth_data(ll, i);
		set_list(lista, i, (long) cur->id);
	}
	return lista;
}

/**
	QUERY 3
*/
LONG_pair total_posts(TAD_community com, Date begin, Date end){
	long answer = 0, question = 0;
	LONG_pair pair;
	GSList *l = find_by_date(com->posts, com->monthsHash, end);

	while(l && compare_date(begin, ((POST) l->data)->creationDate) != -1 ){
		if(isQuestion( (POST) l->data))
			question++;
		else if(isAnswer((POST) l->data))
			answer++;

		l = l->next;
	}
	pair = create_long_pair(question, answer);
	return pair;
}

 /**
	QUERY 4
 */
LONG_list questions_with_tag(TAD_community com, char* tag, Date begin, Date end){
	gpointer tag_id = g_hash_table_lookup(com->tagshash, tag);

	int size=0;
	LONG_list list = NULL;
	GSList *l = NULL;
	GSList *temp = NULL;
	POST p = NULL;

	l = find_by_date(com->posts, com->monthsHash, end);
	while(l && compare_date(begin, ((POST) l->data)->creationDate) != -1 ){
		p = (POST)l->data;
		if(isQuestion(p)) {
			if(contains_tag(p, tag_id)){
				temp = g_slist_append(temp, get_post_key(p));
				size++;
			}
		}

		l = l->next;
	}
	list = create_list(size);
	for(int i=0; i<size; i++){
		set_list(list, i, GPOINTER_TO_INT(temp->data));
		temp = temp->next;
	}
	return list;
}

 /**
	QUERY 5
 */

USER get_user_info(TAD_community com, long id) {
	USER_HT u = g_hash_table_lookup(com->usershash, GINT_TO_POINTER(id));
	if (!u )printf("User não existe");
	long post_history[10];
    int nr_posts = u->nr_posts;
    int i = 0;
    POST p = NULL;
	GSList* l = u->lastPost;
	while(l && i < 10 && i < nr_posts){
		p = (POST)l->data;
		if( (get_ownerUserId(p) != NULL) &&atoi(get_ownerUserId(p)) == id) {
			post_history[i] = get_postId(p);
			i++;
		}
		l = l->next;
	}
	for(; i < 10; i++) {
		post_history[i] = 0;
	}
	return create_user(u->shortBio, post_history);

}


/**
	Função para comparar os scores de dois POSTS
	Se a > b retorna -1 e se a < b retorna 1. Desta maneira a lista é ordenada por ordem decrescente
*/
int compare_score(gconstpointer a, gconstpointer b){
	POST p1 = (POST) a;
	POST p2 = (POST) b;
	int s1 = get_score(p1);
	int s2 = get_score(p2);

	if(s1 > s2)
		return -1;							
	else if (s1 < s2) 
		return 1;
	else 
		return 0;
}

/**
	QUERY 6
*/
LONG_list most_voted_answers(TAD_community com, int N, Date begin, Date end){
	LONG_list list;
	
	GSList *l;
	GSList *aux = NULL;
	l = find_by_date(com->posts, com->monthsHash, end);

	while(l && compare_date(begin, ((POST) l->data)->creationDate) != -1 ){
		if(isAnswer((POST) l->data)){
			aux = g_slist_prepend(aux, ((POST) l->data) );
		}
		l = l->next;
	}

	//ordena a lista por ordem decrescente de score
	aux = g_slist_sort(aux, compare_score);

	list = create_list(N);

	//Copia apenas os N primeiros posts de aux
	for(int i = 0; i < N; i++){
		set_list(list, i, ((POST) aux->data)->id);
		aux = aux->next;
	}
	return list;
}


int compare_nanswers(gconstpointer a, gconstpointer b){
	QUESTION q1 = ((POST) a)->q;
	QUESTION q2 = ((POST) b)->q;

	if(q1->nanswers > q2->nanswers)
		return -1;							
	else if (q1->nanswers < q2->nanswers) 
		return 1;
	else 
		return 0;
}

/**
	QUERY 7
*/
LONG_list most_answered_questions(TAD_community com, int N, Date begin, Date end){

	LONG_list list;
	
	GSList *l;
	GSList *aux = NULL;

	l = find_by_date(com->posts, com->monthsHash, end);

	while(l && compare_date(begin, ((POST) l->data)->creationDate) != -1 ){
		if(isQuestion( (POST) l->data)){
			aux = g_slist_prepend(aux, ((POST) l->data) );
		}
		l = l->next;
	}

	aux = g_slist_sort(aux, compare_nanswers);

	list = create_list(N);

	for(int i = 0; i < N; i++){
		set_list(list, i, ((POST) aux->data)->id);
		aux = aux->next;
	}
	return list;
}

/**
	QUERY 8
*/
LONG_list contains_word(TAD_community com, char* word, int N){
	LONG_list list = create_list(N);
	int i = 0;
	GSList *l = com->posts;
	POST p;
	while(i < N && l != NULL){
		p = ((POST) l->data);
		if(isQuestion(p) && (strstr(get_title(p), word)) != NULL){
			set_list(list, i, ((POST) l->data)->id);
			i++;
		}
		l = l->next;
	}
	for(;i < N; i++){
		set_list(list, i, 0);
	}
	return list;
}


GSList* find_most_recent_post(GHashTable* users, long id1, long id2){
	USER_HT u1 = g_hash_table_lookup(users, GINT_TO_POINTER (id1));
	USER_HT u2 = g_hash_table_lookup(users, GINT_TO_POINTER (id2));
	GSList* l1 = u1->lastPost;
	GSList* l2 = u2->lastPost;
	int b = compare_date_list (l1->data, l2->data);
	if(b == -1){
		return l1;
	}
	else if(b == 0){
		GSList* aux = l1;
		int f = 0;
		while(aux && compare_date_list (aux->data, l2->data) == 0 && f != 0){
			if(aux == l2){
				f = 1;
			}
		}
		if(f == 1) return l1;
		else return l2;
	}
	else return l2;
}


/**
	QUERY 9
*/
/*LONG_list both_participated(TAD_community com, long id1, long id2, int N){

}*/









/**
	QUERY 10
*/
long better_answer(TAD_community com, long id){
	GSList *l;
	int n =0;
	POST p = g_hash_table_lookup(com->postshash, GINT_TO_POINTER(id));
	int nanswers = get_nanswers(p);
	int best, temp; 
	long answer;

	l = find_by_date(com->posts, com->monthsHash, get_lastActivityDate(p));

	while(l != NULL && n < 1 && (compare_date_list(p, (POST) l->data) != -1)){
		if(isAnswer((POST) l->data) && get_parent((POST) l->data) == id){
			nanswers++;
			gpointer owner_key = get_owner_key(p);
			USER_HT user = g_hash_table_lookup(com->usershash, owner_key);
			best = get_score((POST) l->data)*0.45 + (user->reputation)*0.25 + 
				  (get_upvotes((POST) l->data) + get_downvotes((POST) l->data))*0.2 + get_comments((POST) l->data)*0.1;
			printf("%d\n", best);
			answer = get_postId((POST) l->data);
		}
		l = l->next;
	} 

	while(l != NULL && n < nanswers && (compare_date_list(p, (POST) l->data) != -1)){
		if(isAnswer((POST) l->data) && get_parent((POST) l->data) == id){
			nanswers++;
			gpointer owner_key = get_owner_key(p);
			USER_HT user = g_hash_table_lookup(com->usershash, owner_key);
			temp = get_score((POST) l->data)*0.45 + (user->reputation)*0.25 + 
				  (get_upvotes((POST) l->data) + get_downvotes((POST) l->data))*0.2 + get_comments((POST) l->data)*0.1;
			printf("%d\n", temp);
			if(temp > best){
				best = temp;
				answer = get_postId((POST) l->data);
			}
		}
		l = l->next;
	}
	return answer;
}
