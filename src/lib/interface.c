#include <interface.h>
#include <glib.h>
#include <stdio.h>
#include "/usr/include/libxml2/libxml/parser.h"
#include "/usr/include/libxml2/libxml/xmlIO.h"
#include <string.h>
#include "date.h"


struct TCD_community{
	GHashTable *postshash;
	GHashTable *usershash;
	GHashTable *tagshash;
	GSList *posts;
	GHashTable *monthsHash;         //guarda o primeiro posts de cada mes
};

typedef struct post{
	int id;
	char *title;
	char *postTypeId;
	char *ownerUserId;
	int parentId;
	int score;
	int nanswers;
	GSList *tags;
	Date creationDate;
}*POST;

typedef struct user_ht{
	char *name;
	char *shortBio;
	unsigned short nr_posts;
}*USER_HT;

typedef struct user_ll{
	int id;
	unsigned short nr_posts;
}*USER_LL;

/*
* Retorna 1 se a primeira data for a mais antiga, -1 se for a mais recente e 0 se forem iguais
*/
int compare_date (Date d1, Date d2){
	int ano1 = get_year(d1);
	int ano2 = get_year(d2);
	int mes1= get_month(d1);
	int mes2 = get_month(d2);
	int dia1 = get_day(d1);
	int dia2 = get_day(d2);

	if(ano1 > ano2) return -1;
	if(ano1 < ano2) return 1;

	if(mes1 > mes2) return -1;
	if(mes1 < mes2) return 1;

	if(dia1 > dia2) return -1;
	if(dia1 < dia2) return 1;

	return 0;
}

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
* Função que converte a CreationDate no XML na struct date
*/
Date xmlCreationDate_to_Date(char* xmlDate) {
	char s_year[2];
	s_year[0] = xmlDate[2];
	s_year[1] = xmlDate[3];
	int year = 2000 + atoi(s_year);

	char s_month[2];
	s_month[0] = xmlDate[5];
	s_month[1] = xmlDate[6];
	int month = atoi(s_month);

	char s_day[2];
	s_day[0] = xmlDate[8];
	s_day[1] = xmlDate[9];
	int day = atoi(s_day);

	Date d = createDate(day, month, year);
	return d;
}
/**
* Função que converte a CreationDate no XML numa key para ser usada na monthshash
*/
int date_to_Key(int year, int month) {
	return (year * 100) + month;
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
		
		g_hash_table_insert(com->usershash, GINT_TO_POINTER(r_users), u);
		ptr = ptr->next->next;

	}

	xmlFreeDoc(doc);
}

/**
* Função que recebe a string das tags e as coloca numa lista ligada
*/
GSList* getTags(GHashTable *h, char* tags){
	int i = 0, j=0;
	GSList *l = NULL;
	char res[100];
	gpointer valor;


	while(tags[i] != '\0') {
		i++;
		for (; tags[i] != '>'; i++) {
			res[j] = tags[i];
			j++;
		}
		res[j] = '\0';
		valor = g_hash_table_lookup(h, res);
		l = g_slist_prepend(l, valor);
		j = 0;
		i++;
	}
	return l;
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
	
	char *key_posts;
	int posts_size = 0;

	ptr = cur->xmlChildrenNode;
	ptr = ptr->next;

	while(ptr != NULL){
		char * postTypeId = (char *) xmlGetProp(ptr, (xmlChar *) "PostTypeId");
		if((strcmp(postTypeId, "1")==0) || (strcmp(postTypeId, "2")==0)){
			POST p = malloc(sizeof(struct post));
			key_posts = (char *) xmlGetProp(ptr, (xmlChar *) "Id");
			int r_posts = atoi(key_posts);
			
			p->creationDate = xmlCreationDate_to_Date((char*) xmlGetProp(ptr, (xmlChar *) "CreationDate"));

			p->postTypeId =  postTypeId;

			p->score = atoi((char *) xmlGetProp(ptr, (xmlChar *) "Score"));

			char *userId = (char *) xmlGetProp(ptr, (xmlChar *) "OwnerUserId");
			//Caso o post possua informação sobre o OwnerUserId
			p->id = atoi((char *) xmlGetProp(ptr, (xmlChar *) "Id"));
			if(userId != NULL){
				int ownerUserId = atoi(userId);
				USER_HT user = g_hash_table_lookup(com->usershash, GINT_TO_POINTER(ownerUserId));
				user->nr_posts += 1;
				g_hash_table_insert(com->usershash, GINT_TO_POINTER(ownerUserId), user);
			}
			if (!strcmp(p->postTypeId, "1")){
				p->title = (char *) xmlGetProp(ptr, (xmlChar *) "Title");
				p->ownerUserId =  (char *) xmlGetProp(ptr, (xmlChar *) "OwnerUserId");
				p->tags = getTags(com->tagshash, (char *) xmlGetProp(ptr, (xmlChar *) "Tags"));	
				p->nanswers = atoi((char *) xmlGetProp(ptr, (xmlChar *) "AnswerCount"));
			}
			else if(!strcmp(p->postTypeId, "2")){
				p->parentId = atoi ((char *) xmlGetProp(ptr, (xmlChar *) "ParentId"));
			}

			com->posts = g_slist_prepend(com->posts, p);
			posts_size++;
			
			g_hash_table_insert(com->postshash, GINT_TO_POINTER(r_posts), p);
		}
		ptr = ptr->next->next;
	}

	xmlFreeDoc(doc);
	com->posts = g_slist_sort (com->posts, compare_date_list);
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

/**
  Função que faz load dos ficheiros para a estrutura de dados
*/
TAD_community load(TAD_community com, char* dump_path){
	
	loadTags(com, dump_path, "Tags.xml");

	loadUsers(com, dump_path, "Users.xml");

	loadPosts(com, dump_path, "Posts.xml");

	return com;
}
/**
  QUERY 1
*/
STR_pair info_from_post(TAD_community com, long id){
	POST p, z1;
	USER_HT q, z2;

	STR_pair pair;

	p = g_hash_table_lookup(com->postshash, GINT_TO_POINTER(id));
	
	//Caso nenhum post tenha o id procurado
	if(p==NULL){
		fprintf(stderr, "Id não válido\n");
		return NULL;
	} 

	if(!strcmp(p->postTypeId, "1")){
		q = g_hash_table_lookup(com->usershash, GINT_TO_POINTER(atoi(p->ownerUserId)));
		pair = create_str_pair(p->title, q->name);
	}
	else if(!strcmp(p->postTypeId, "2")){
		z1 = g_hash_table_lookup(com->postshash, GINT_TO_POINTER(p->parentId));

		int key;
		key = atoi(z1->ownerUserId);
		z2 = g_hash_table_lookup(com->usershash, GINT_TO_POINTER(key));
		pair = create_str_pair(z1->title, z2->name);	

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

int isQuestion(POST p){
	return (!strcmp(p->postTypeId, "1"));
}

int isAnswer(POST p){
	return (!strcmp(p->postTypeId, "2"));
}
/**
	QUERY 3
*/
LONG_pair total_posts(TAD_community com, Date begin, Date end){
	int year, month;
	long answer = 0, question = 0;
	year = get_year(end);
	month = get_month(end);
	LONG_pair pair;
	GSList *l = com->posts;

	if(month < 12)
		month++;
	else{
		year++;
		month = 1;
	}


	if(get_year(((POST) l->data)->creationDate) == get_year(end)){
		if(get_month(((POST) l->data)->creationDate) > get_month(end)){
			int key = date_to_Key(year, month);
			l = g_hash_table_lookup(com->monthsHash, GINT_TO_POINTER(key));
		}
		while(l && compare_date(end, ((POST) l->data)->creationDate) == 1){
			l=l->next;
		}
	}
	else if(get_year(((POST) l->data)->creationDate) > get_year(end)){
		int key = date_to_Key(year, month);
		l = g_hash_table_lookup(com->monthsHash, GINT_TO_POINTER(key));
		while(l && compare_date(end, ((POST) l->data)->creationDate) == 1){ //problema!!!
			l=l->next;
		}
	}


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
	gpointer valor = g_hash_table_lookup(com->tagshash, tag);

	int year, month;
	int size=0;
	year = get_year(end);
	month = get_month(end);
	LONG_list list;
	GSList *l = com->posts;
	GSList *temp = NULL;
	GSList *aux = NULL;
	POST p = NULL;

	if(month < 12)
		month++;
	else{
		year++;
		month = 1;
	}


	if(get_year(((POST) l->data)->creationDate) == get_year(end)){
		if(get_month(((POST) l->data)->creationDate) > get_month(end)){
			int key = date_to_Key(year, month);
			l = g_hash_table_lookup(com->monthsHash, GINT_TO_POINTER(key));
		}
		while(l && compare_date(end, ((POST) l->data)->creationDate) == 1){
			l=l->next;
		}
	}
	else if(get_year(((POST) l->data)->creationDate) > get_year(end)){
		int key = date_to_Key(year, month);
		l = g_hash_table_lookup(com->monthsHash, GINT_TO_POINTER(key));
		while(l && compare_date(end, ((POST) l->data)->creationDate) == 1){ 
			l=l->next;
		}
	}

	while(l && compare_date(begin, ((POST) l->data)->creationDate) != -1 ){
		p = (POST)l->data;
		if(!strcmp (p->postTypeId, "1")) {
			if(p->tags && (aux = g_slist_find(p->tags, valor)) != NULL){
				temp = g_slist_append(temp, GINT_TO_POINTER (p->id));
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
	Função para comparar os scores de dois POSTS
	Se a > b retorna -1 e se a < b retorna 1. Desta maneira a lista é ordenada por ordem decrescente
*/
int compare_score(gconstpointer a, gconstpointer b){
	POST p1 = (POST) a;
	POST p2 = (POST) b;

	if(p1->score > p2->score)
		return -1;							
	else if (p1->score < p2->score) 
		return 1;
	else 
		return 0;
}

/**
	QUERY 6
*/
LONG_list most_voted_answers(TAD_community com, int N, Date begin, Date end){
	int year = get_year(end);
	int month = get_month(end);
	
	LONG_list list;
	
	GSList *l = com->posts;
	GSList *aux = NULL;

	if(month < 12)
		month++;
	else{
		year++;
		month = 1;
	}

	if(get_year(((POST) l->data)->creationDate) == get_year(end)){
		if(get_month(((POST) l->data)->creationDate) > get_month(end)){
			int key = date_to_Key(year, month);
			l = g_hash_table_lookup(com->monthsHash, GINT_TO_POINTER(key));
		}
		while(l && compare_date(end, ((POST) l->data)->creationDate) == 1){
			l=l->next;
		}
	}
	else if(get_year(((POST) l->data)->creationDate) > get_year(end)){
		int key = date_to_Key(year, month);
		l = g_hash_table_lookup(com->monthsHash, GINT_TO_POINTER(key));
		while(l && compare_date(end, ((POST) l->data)->creationDate) == 1){ 
			l=l->next;
		}
	}

	while(l && compare_date(begin, ((POST) l->data)->creationDate) != -1 ){
		if(strcmp(( (POST) l->data)->postTypeId, "2") == 0){
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
	POST p1 = (POST) a;
	POST p2 = (POST) b;

	if(p1->nanswers > p2->nanswers)
		return -1;							
	else if (p1->nanswers < p2->nanswers) 
		return 1;
	else 
		return 0;
}


LONG_list most_answered_questions(TAD_community com, int N, Date begin, Date end){
	int year = get_year(end);
	int month = get_month(end);
	
	LONG_list list;
	
	GSList *l = com->posts;
	GSList *aux = NULL;

	if(month < 12)
		month++;
	else{
		year++;
		month = 1;
	}

	if(get_year(((POST) l->data)->creationDate) == get_year(end)){
		if(get_month(((POST) l->data)->creationDate) > get_month(end)){
			int key = date_to_Key(year, month);
			l = g_hash_table_lookup(com->monthsHash, GINT_TO_POINTER(key));
		}
		while(l && compare_date(end, ((POST) l->data)->creationDate) == 1){
			l=l->next;
		}
	}
	else if(get_year(((POST) l->data)->creationDate) > get_year(end)){
		int key = date_to_Key(year, month);
		l = g_hash_table_lookup(com->monthsHash, GINT_TO_POINTER(key));
		while(l && compare_date(end, ((POST) l->data)->creationDate) == 1){ 
			l=l->next;
		}
	}

	while(l && compare_date(begin, ((POST) l->data)->creationDate) != -1 ){
		if(strcmp(( (POST) l->data)->postTypeId, "1") == 0){
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



LONG_list contains_word(TAD_community com, char* word, int N){
	LONG_list list = create_list(N);
	int i = 0;
	GSList *l = com->posts;
	POST p;
	while(i < N && l != NULL){
		p = ((POST) l->data);
		if(strcmp(p->postTypeId, "1") == 0 && (strstr(p->title, word)) != NULL){
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