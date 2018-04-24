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
#include "users.h"
//#include "myuser.h"


struct TCD_community{
	//USERS users;
	//GHashTable *postshash;
	GHashTable *tagshash;
	//GSList *posts;
	//GHashTable *monthsHash;         //guarda o primeiro posts de cada mes
};

/**
*	Função que inicializa a estrutura de dados
*/
TAD_community init(){
	TAD_community t;
	t = malloc(sizeof(struct TCD_community));
	init_users();
	init_posts();
	t->tagshash = g_hash_table_new (g_str_hash, g_str_equal);
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

	ptr = cur->xmlChildrenNode;
	ptr = ptr->next;
	while(ptr != NULL){
 		add_myuser(ptr);
		ptr = ptr->next->next;

	}
	sort_users_by_reputation();

	xmlFreeDoc(doc);
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

	ptr = cur->xmlChildrenNode;
	ptr = ptr->next;

	while(ptr != NULL){
		char * postTypeId = (char *) xmlGetProp(ptr, (xmlChar *) "PostTypeId");
		if((strcmp(postTypeId, "1")==0) || (strcmp(postTypeId, "2")==0)){
			add_to_posts(ptr, com->tagshash);
			
		//	com->posts = g_slist_prepend(com->posts, p);
		//	posts_size++;
		}
		ptr = ptr->next->next;
	}

	//GSList* l;

	xmlFreeDoc(doc);
	/*com->posts = g_slist_sort (com->posts, compare_date_list);
	for(l = com->posts; l; l = l->next){
		POST pf = (POST) l->data;
		gpointer k = get_owner_key(pf);
		find_and_set_user_lastPost(k, get_postId(pf));
		g_hash_table_insert(com->postshash, get_post_key(pf), l);
	}
	GSList* li = com->posts;
	for(int i = 0; i < posts_size; i++) {
		Date d = get_creationDate((POST) li->data);
		int months_key = date_to_Key(get_year(d), get_month(d)) ;
		g_hash_table_insert(com->monthsHash, GINT_TO_POINTER(months_key), li);
		li = g_slist_next(li);
	}*/
	finalize();
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
			POST p = find_post(atoi(postId));
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


void set_users_nr_posts_and_last_post() {
	PostsList l = get_posts_list();
	for(; l; l = get_next(l)) {
		POST p = get_post(l);
		gpointer k = get_owner_key(p);
		find_and_set_user_lastPost(k, get_postId(p));
		if(get_ownerUserId(p) != NULL){
			//printf("%d\n", get_user_nr_posts(find_user(atoi(get_ownerUserId(p)))));
			find_and_increment_user_nr_posts(get_owner_key(p));
			//printf("%d\n", get_user_nr_posts(find_user(atoi(get_ownerUserId(p)))));
			//printf("\n");
		}	
	}
	sort_users_by_nr_posts();
}


/**
  Função que faz load dos ficheiros para a estrutura de dados
*/
TAD_community load(TAD_community com, char* dump_path){
	loadTags(com, dump_path, "Tags.xml");
	loadUsers(com, dump_path, "Users.xml");
	loadPosts(com, dump_path, "Posts.xml");
	loadVotes(com, dump_path, "Votes.xml");
	set_users_nr_posts_and_last_post();

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

	p = find_post(id);
	
	//Caso nenhum post tenha o id procurado
	if(p==NULL){
		fprintf(stderr, "Id não válido\n");
		return NULL;
	} 
	if(isQuestion(p)){
		u = find_user( atoi(get_ownerUserId(p)));
		pair = create_str_pair(get_title(p), get_user_name(u));
	}
	else if(isAnswer(p)){
		p = find_post(get_parent(p));

		u = find_user(atoi(get_ownerUserId(p)));
		pair = create_str_pair(get_title(p), get_user_name(u));	

	}
	return pair;
}

/**
  QUERY 2
*/
LONG_list top_most_active(TAD_community com, int N){
	return get_N_users_with_most_nr_posts(N);
}

/**
	QUERY 3
*/
LONG_pair total_posts(TAD_community com, Date begin, Date end){
	long answer = 0, question = 0;
	LONG_pair pair;
	PostsList l = find_by_date(end);

	while(l && compare_date(begin, get_creationDate(get_post(l))) != -1 ){
		if(isQuestion(get_post(l)))
			question++;
		else if(isAnswer(get_post(l)))
			answer++;

		l = get_next(l);
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
	PostsList l = NULL;
	GSList *temp = NULL; //???
	POST p = NULL;

	l = find_by_date(end);
	while(l && compare_date(begin, get_creationDate((POST) l->data)) != -1 ){
		p = get_post(l);
		if(isQuestion(p)) {
			if(contains_tag(p, tag_id)){
				temp = g_slist_append(temp, get_post_key(p));
				size++;
			}
		}

		l = get_next(l);
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
	USER_HT u = find_user( id);
	if (!u )printf("User não existe");
	long post_history[10];
    int nr_posts = get_user_nr_posts(u);
    int i = 0;
    POST p = NULL;
    int lp = get_user_lastPost(u);
	PostsList l = find_post_in_list(lp);
	while(l && i < 10 && i < nr_posts){
		p = get_post(l);
		if( (get_ownerUserId(p) != NULL) && atoi(get_ownerUserId(p)) == id) {
			post_history[i] = get_postId(p);
			i++;
		}
		l = get_next(l);
	}
	for(; i < 10; i++) {
		post_history[i] = 0;
	}
	return create_user(get_user_shortBio(u), post_history);

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
	
	PostsList l;
	GSList *aux = NULL;
	l = find_by_date(end);

	while(l && compare_date(begin, get_creationDate(get_post(l))) != -1 ){
		if(isAnswer(get_post(l))){
			aux = g_slist_prepend(aux, (get_post(l)));
		}
		l = get_next(l);
	}

	//ordena a lista por ordem decrescente de score
	aux = g_slist_sort(aux, compare_score);

	list = create_list(N);

	//Copia apenas os N primeiros posts de aux
	for(int i = 0; i < N; i++){
		set_list(list, i, get_postId((POST) aux->data));
		aux = aux->next;
	}
	return list;
}


/**
	QUERY 7
*/
LONG_list most_answered_questions(TAD_community com, int N, Date begin, Date end){

	LONG_list list;
	
	PostsList l;
	GSList *aux = NULL;

	l = find_by_date(end);

	while(l && compare_date(begin, get_creationDate(get_post(l))) != -1 ){
		if(isQuestion(get_post(l))){
			aux = g_slist_prepend(aux, (get_post(l)));
		}
		l = get_next(l);
	}

	aux = g_slist_sort(aux, compare_nanswers);

	list = create_list(N);

	for(int i = 0; i < N; i++){
		set_list(list, i, get_postId((POST) aux->data));
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
	PostsList l = get_posts_list();
	POST p;
	while(i < N && l != NULL){
		p = get_post(l);
		if(isQuestion(p) && (strstr(get_title(p), word)) != NULL){
			set_list(list, i, get_postId(p));
			i++;
		}
		l = get_next(l);
	}
	for(;i < N; i++){
		set_list(list, i, 0);
	}
	return list;
}




/**
	QUERY 9
*/

LONG_list both_participated(TAD_community com, long id1, long id2, int N){
	USER_HT user1 = find_user(id1);
	USER_HT user2 = find_user(id2);
	if (!user1 || !user2) return NULL;	
	int nposts1 = get_user_nr_posts(user1);
	int nposts2 = get_user_nr_posts(user2);
	int npostst = nposts1 + nposts2;
	int queue1[nposts1];
	int queue2[nposts2];
	int queuef[npostst];
	for (int i = 0; i < nposts1; i++) {
		queue1[i] = 0;
	}
	for (int i = 0; i < nposts2; i++) {
		queue2[i] = 0;
	}
	for (int i = 0; i < npostst; i++) {
		queuef[i] = 0;
	}
	int posts_i1_found = 0;
	int posts_i2_found = 0;

	int queue1_size = 0;
	int queue2_size = 0;
	int queuef_size = 0;
	int res_size = 0;
	int f = 0;
	int i = 0;

	LONG_list res = create_list(N);

	GSList* l1 = find_post_in_list(get_user_lastPost(user1));
	GSList* l2 = find_post_in_list(get_user_lastPost(user2));
	
	PostsList l = find_most_recent_post(l1, l2);
	if (!l)
		return NULL;

	while(l && res_size < N && (posts_i1_found < nposts1 || posts_i2_found < nposts2)) {
		POST p = get_post(l);
		if(get_ownerUserId(p) != NULL){
			if(isAnswer(p)) {
				if(atoi(get_ownerUserId(p)) == id1) {
					if(get_parent_owner(p) != NULL) {
						if(atoi(get_parent_owner(p)) == id2) {
							queuef[queuef_size] =  get_parent(p);
							queuef_size++;
						}
						else {
							i = 0;
							f = 0;
							while (i < queue2_size && f == 0) {
								if (queue2[i] == get_postId(p)) {
									f = 1;
								}
								i++;
							}
							if (f == 1) {
								queuef[queuef_size] =  get_parent(p);
								queuef_size++;
							}
							else {
								queue1[queue1_size] =  get_parent(p);
								queue1_size++;
							}
						}
					}
					posts_i1_found++;
				}
				else if(atoi(get_ownerUserId(p)) == id2) {
					if(get_parent_owner(p) != NULL) {
						if(atoi(get_parent_owner( p)) == id1) {
							queuef[queuef_size] =  get_parent(p);
							queuef_size++;
						}
						else {
							i = 0;
							f = 0;
							while (i < queue1_size && f == 0) {
								if (queue1[i] == get_postId(p)) {
									f = 1;
								}
								i++;
							}
							if (f == 1) {
								queuef[queuef_size] =  get_parent(p);
								queuef_size++;
							}
							else {
								queue2[queue2_size] = get_parent(p);
								queue2_size++;
							}
						}
					}
					posts_i2_found++;
				}
			}
		else if (isQuestion(p)) {
				if(atoi(get_ownerUserId(p)) == id1) {
					posts_i1_found++;
				}
				if(atoi(get_ownerUserId(p)) == id2)
					posts_i2_found++;
				i = 0;
				f = 0;
				while (i < queuef_size && f == 0) {
					if (queuef[i] == get_postId(p)) {
						f = 1;
					}
					i++;
				}
				if (f == 1) {
					set_list(res, res_size ,get_postId(p));
					res_size++;

				}
				f = 0;
			}
		}
		l = get_next(l);
	}
	for (; res_size < N; res_size++ ) {
		set_list(res, res_size ,0);
	}
	return res;

}

/**
	QUERY 10
*/
long better_answer(TAD_community com, long id){
	PostsList l;
	int n =0;
	POST p = find_post(id);
	int nanswers = get_nanswers(p);
	int best, temp; 
	long answer;

	l = find_by_date(get_lastActivityDate(p));

	while(l != NULL && n < 1 && (compare_date_list(p, get_post(l)) != -1)){
		POST ans = get_post(l);
		if(isAnswer(ans) && get_parent(ans) == id){
			nanswers++;
			//gpointer owner_key = get_owner_key(p);
			USER_HT user = find_user(atoi(get_ownerUserId(p)));
			best = get_score(ans)*0.45 + (get_user_reputation(user))*0.25 + 
				  (get_upvotes(ans) + get_downvotes(ans))*0.2 + get_comments(ans)*0.1;
		//	printf("%d\n", best);
			answer = get_postId(ans);
		}
		l = get_next(l);
	} 

	while(l != NULL && n < nanswers && (compare_date_list(p, get_post(l)) != -1)){
		POST ans = get_post(l);
		if(isAnswer(ans) && get_parent(ans) == id){
			nanswers++;
			//gpointer owner_key = get_owner_key(p);
			USER_HT user = find_user(atoi(get_ownerUserId(p)));
			temp = get_score(ans)*0.45 + (get_user_reputation(user))*0.25 + 
				  (get_upvotes(ans) + get_downvotes(ans))*0.2 + get_comments(ans)*0.1;
			//printf("%d\n", temp);
			if(temp > best){
				best = temp;
				answer = get_postId(ans);
			}
		}
		l = get_next(l);
	}
	return answer;
}

typedef struct auxTag{
	int id;
	int num;
}*AUXTAG;

GSList* addTags(GHashTable* h, GSList* res, GSList* tags){
	AUXTAG aux;
	while(tags) {
		aux = g_hash_table_lookup(h, GINT_TO_POINTER(tags->data));
		if(aux) {
			aux->num += 1;
		}
		else {
			AUXTAG a = (AUXTAG) malloc(sizeof(struct auxTag));
			a->id = GPOINTER_TO_INT( (gpointer) tags->data);
			a->num = 1;
			res = g_slist_append(res, a);
			g_hash_table_insert(h,GINT_TO_POINTER(a->id), a);
		}
		
	
	tags = tags->next;
	}
	return res;
}

int compare_numTag(gconstpointer a, gconstpointer b){
	AUXTAG a1 = (AUXTAG) a;
	AUXTAG a2 = (AUXTAG) b;

	if(a1->num > a2->num)
		return -1;							
	else if (a1->num < a2->num) 
		return 1;
	else 
		return 0;
}

LONG_list most_used_best_rep(TAD_community com, int N, Date begin, Date end){
	LONG_list list = get_N_users_with_most_reputation(N);
	GHashTable* h = g_hash_table_new (g_direct_hash, g_direct_equal);
	PostsList l = find_by_date(end);
	GSList *aux = NULL;
	GSList *tags = NULL;

	while(l && (compare_date(begin, get_creationDate(get_post(l))) != -1)){
		POST p = get_post(l);
		int i = 0;
		char *ownerUserId = get_ownerUserId(p);

		if(isQuestion(p) && ownerUserId != NULL){
			for(i = 0; i < N && atoi(ownerUserId) != get_list(list, i); i++);
			
			if(atoi(ownerUserId) == get_list(list, i)){
				tags = get_tags(p);
				aux = addTags(h,aux, tags);
			}
		}
		l = get_next(l);
	}
	LONG_list res = create_list(N);
	aux = g_slist_sort(aux, compare_numTag);
	/*for(GSList* aa = aux; aa; aa = aa->next){
		printf("%d\n", ((AUXTAG)aa->data)->id);
	}*/
	int j = 0;
	GSList* tofree = aux;
	while(aux && j < N){
		set_list(res, j, ((AUXTAG) aux->data)->id);
		free(aux->data);
		j++;
		aux = aux->next;
	}
	g_hash_table_destroy (h);
	g_slist_free (tofree);
	return res;
}

TAD_community clean(TAD_community com) {
	free_users();
	return com;
}