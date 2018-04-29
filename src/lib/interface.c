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
	Posts p;
	USERS users;
	TAGS t;
};

/**
 * Estrutura auxiliar para query 11 que guarda o id de uma tag e o número de vezes que ela foi utilizada
 */
typedef struct auxTag{
	int id;
	int num;
}*AUXTAG;

/**
*	Função que inicializa a estrutura de dados
*/
TAD_community init(){
	TAD_community com;
	com = malloc(sizeof(struct TCD_community));
	com->users = init_users();
	com->p = init_posts(com->p);
	com->t = init_tags();
	return com;
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
		return;
	}

	ptr = cur->xmlChildrenNode;
	ptr = ptr->next;

	while(ptr != NULL){
		int id = atoi((char *) xmlGetProp(ptr, (xmlChar *) "Id"));
		char * name = (char *) xmlGetProp(ptr, (xmlChar *) "DisplayName");
		char * shortBio = (char *) xmlGetProp(ptr, (xmlChar *) "AboutMe");
		int nr_posts = 0;
		int lastPost = 0;
		int reputation = atoi((char*) xmlGetProp(ptr, (xmlChar *) "Reputation"));
 		add_myuser(com->users, id, name, shortBio, nr_posts, lastPost, reputation);
 		xmlFree(name);
 		xmlFree(shortBio);
		ptr = ptr->next->next;

	}
	sort_users_by_reputation(com->users);
	xmlFreeDoc(doc);
}
/**
	Função que faz load ao file Posts.xml
*/
void loadPosts(TAD_community com, char *dump_path, char *file){
	xmlDocPtr doc;
	xmlNodePtr cur, ptr;
	char file_posts[100];
	strcpy(file_posts, filePath(dump_path, file));

	doc = xmlParseFile(file_posts);
	if(doc == NULL){
		fprintf(stderr, "Document not parsed successfully.\n");
		return;
	}
	cur = xmlDocGetRootElement(doc);

	if(cur == NULL){
		fprintf(stderr, "empty document\n");
		xmlFreeDoc(doc);
		return;
	}

	ptr = cur->xmlChildrenNode;
	ptr = ptr->next;

	while(ptr != NULL){
		char postTypeId = xmlGetProp(ptr, (xmlChar *) "PostTypeId")[0];
		if(postTypeId == '1' || postTypeId == '2'){
			int id = atoi((char *) xmlGetProp(ptr, (xmlChar *) "Id"));
			char * ownerUserId = (char *) xmlGetProp(ptr, (xmlChar *) "OwnerUserId");
			Date creationDate = xmlCreationDate_to_Date((char*) xmlGetProp(ptr, (xmlChar *) "CreationDate"));
			if (postTypeId == '1') {
				char * title = (char *) xmlGetProp(ptr, (xmlChar *) "Title");
				char * nanswers = (char *) xmlGetProp(ptr, (xmlChar *) "AnswerCount");
				GSList * tags = getTags(com->t,(char *) xmlGetProp(ptr, (xmlChar *) "Tags"));
				Date lastActivityDate = xmlCreationDate_to_Date((char*) xmlGetProp(ptr, (xmlChar *) "LastActivityDate"));
				add_question_to_posts(com->p,title, atoi(nanswers), tags, lastActivityDate, postTypeId, id, ownerUserId, creationDate);
				xmlFree(title);
				xmlFree(nanswers);
				xmlFree(ownerUserId);
				free_date(lastActivityDate);
				g_slist_free(tags);
			}
			else if (postTypeId == '2') {
				int parentId = atoi ((char *) xmlGetProp(ptr, (xmlChar *) "ParentId"));
				int comments = atoi ((char *) xmlGetProp(ptr, (xmlChar *) "CommentCount"));
				int score = atoi((char *) xmlGetProp(ptr, (xmlChar *) "Score"));
				add_answer_to_posts(com->p, parentId, comments, score, postTypeId, id, ownerUserId, creationDate);
				xmlFree(ownerUserId);
			}
			free_date(creationDate);
		}
		ptr = ptr->next->next;
	}
	xmlFreeDoc(doc);
	finalize(com->p);
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

	doc = xmlParseFile(file_tags);

	if(doc == NULL){
		fprintf(stderr, "Document not parsed successfully.\n");
		return;
	}

	cur = xmlDocGetRootElement(doc);

	if(cur == NULL){
		fprintf(stderr, "empty document\n");
		xmlFreeDoc(doc);
		return;
	}
	
	ptr = cur->xmlChildrenNode;
	ptr = ptr->next;

	while(ptr != NULL){
		char * tag_name =  (char *) xmlGetProp(ptr, (xmlChar *) "TagName");
		int tag_id = (atoi((char *) xmlGetProp(ptr, (xmlChar *) "Id")));
		insert_tag(com->t, tag_name, tag_id);
		xmlFree(tag_name);
		ptr = ptr->next->next;
	}

	xmlFreeDoc(doc);
}
/**
 *	Função que para cada user define o número de posts que este tem e qual o último post feito
 */
void set_users_nr_posts_and_last_post(TAD_community com) {
	PostsList l = get_posts_list(com->p);
	for(; l; l = get_next(l)) {
		POST p = get_post(l);
		gpointer k = get_owner_key(p);
		find_and_set_user_lastPost(com->users, k, get_postId(p));
		if(get_ownerUserId(p) != NULL){
			find_and_increment_user_nr_posts(com->users, get_owner_key(p));
		}	
	}
	sort_users_by_nr_posts(com->users);
}
/**
  Função que faz load dos ficheiros para a estrutura de dados
*/
TAD_community load(TAD_community com, char* dump_path){
	loadTags(com, dump_path, "Tags.xml");
	loadUsers(com, dump_path, "Users.xml");
	loadPosts(com, dump_path, "Posts.xml");
	set_users_nr_posts_and_last_post(com);

	return com;
}
/**
 * QUERY 1
 * Dado um ID de um post retorna o título do post e o nome do autor
 */
STR_pair info_from_post(TAD_community com, long id){
	POST p;
	USER_HT u;
	STR_pair pair;

	p = find_post(com->p, id);
	
	//Caso nenhum post tenha o id procurado
	if(p==NULL){
		fprintf(stderr, "Id não válido\n");
		return create_str_pair("", "");
	} 
	if(isQuestion(p)){
		u = find_user(com->users, atoi(get_ownerUserId(p)));
		pair = create_str_pair(get_title(p), get_user_name(u));
	}
	else if(isAnswer(p)){
		p = find_post(com->p, get_parent(p));
		u = find_user(com->users, atoi(get_ownerUserId(p)));
		pair = create_str_pair(get_title(p), get_user_name(u));	
	}
	return pair;
}
/**
 * QUERY 2
 * Retorna os N utilizadores com maior número de posts de sempre
 */
LONG_list top_most_active(TAD_community com, int N){
	return get_N_users_with_most_nr_posts(com->users, N);
}
/**
 * QUERY 3
 * Retorna o número total de posts num certo intervalo de tempo
*/
LONG_pair total_posts(TAD_community com, Date begin, Date end){
	long answer = 0, question = 0;
	LONG_pair pair;
	PostsList l = find_by_date(com->p, end);

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
 * QUERY 4
 * Retorna todas as perguntas num certo intervalo de tempo contendo uma determinada tag 
 */
LONG_list questions_with_tag(TAD_community com, char* tag, Date begin, Date end){
	gpointer tag_id = convert_tag_name_to_id(com->t, tag);
	if (!tag_id) return NULL;
	int size=0;
	LONG_list list = NULL;
	PostsList l = NULL;
	GSList *temp = NULL;
	POST p = NULL;

	l = find_by_date(com->p, end);
	while(l && compare_date(begin, get_creationDate((POST) l->data)) != -1 ){
		p = get_post(l);
		if(isQuestion(p)) {
			if(contains_tag(p, tag_id)){
				temp = g_slist_append(temp, GINT_TO_POINTER(get_postId(p)));
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
	g_slist_free(temp);
	return list;
}
/**
 * QUERY 5
 * Devolve a informação do perfil e os IDs dos últimos 10 posts de um certo utilizador
 */
USER get_user_info(TAD_community com, long id) {
	USER_HT u = find_user(com->users, id);
	if (!u ) {
		printf("User não existe");
		return NULL;
	}
	long post_history[10];
    int nr_posts = get_user_nr_posts(u);
    int i = 0;
    POST p = NULL;
    int lp = get_user_lastPost(u);
	PostsList l = find_post_in_list(com->p, lp);
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
	if (!get_user_shortBio(u)) 
		return create_user("", post_history);
	return create_user(get_user_shortBio(u), post_history);
}
/**
 * Função para comparar os scores de dois POSTS
 * Se a > b retorna -1 e se a < b retorna 1. Desta maneira a lista é ordenada por ordem decrescente
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
 * QUERY 6
 * Devolve os IDs das N respostas com mais votos num dado intervalo de tempo
*/
LONG_list most_voted_answers(TAD_community com, int N, Date begin, Date end){
	LONG_list list;
	PostsList l;
	GSList *aux = NULL;
	int i = 0;
	l = find_by_date(com->p, end);

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
	for(i = 0; i < N && aux; i++){
		set_list(list, i, get_postId((POST) aux->data));
		aux = aux->next;
	}
	for(; i < N; i++){
		set_list(list, i, 0);
	}
	g_slist_free(aux);
	return list;
}
/**
 * QUERY 7
 * Devolve os IDs das N perguntas com mais respostas
*/
LONG_list most_answered_questions(TAD_community com, int N, Date begin, Date end){
	LONG_list list;
	PostsList l;
	GSList *aux = NULL;
	int i = 0;

	l = find_by_date(com->p, end);

	while(l && compare_date(begin, get_creationDate(get_post(l))) != -1 ){
		if(isQuestion(get_post(l))){
			aux = g_slist_prepend(aux, (get_post(l)));
		}
		l = get_next(l);
	}

	aux = g_slist_sort(aux, compare_nanswers);

	list = create_list(N);

	for(i = 0; i < N && aux; i++){
		set_list(list, i, get_postId((POST) aux->data));
		aux = aux->next;
	}
	for(; i < N; i++){
		set_list(list, i, 0);
	}
	g_slist_free(aux);
	return list;
}
/**
 * QUERY 8
 * Devolve os IDs de N perguntas cujos títulos contenham uma dada palavra
*/
LONG_list contains_word(TAD_community com, char* word, int N){
	LONG_list list = create_list(N);
	int i = 0;
	PostsList l = get_posts_list(com->p);
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
 *	Função auxiliar para a query 9 que adiciona uma resposta à queue
 */
void addQueue_Answer(TAD_community com, POST p, long * id2, int * q1, int * s1, int * q2, int * s2, int * qf, int * sf, int * i, int * f){
	if(atoi(get_parent_owner(com->p, p)) == *id2) {
		(*(qf+*sf)) = get_parent(p);
		*sf += 1;
	}
	else {
		*i = 0;
		*f = 0;
		while ((*i) < (*s2) && (*f) == 0) {
			if ((*(q2+(*i))) == get_postId(p)) {
				(*f) = 1;
			}
			(*i)+=1;
		}
		if (*f == 1) {
			(*(qf+*sf)) = get_parent(p);
			*sf += 1;
		}
		else {
			(*(q1+*s1)) = get_parent(p);
			*s1 += 1;
		}
	}
}
/**
 *	Função auxiliar para a query 9 que adiciona uma pergunta à queue
 */
void addQueue_Question(POST p, long * id1, int * posts_i1_found, long * id2, int * posts_i2_found, int * i, int * f, int * qf, int * queuef_size){
	if(atoi(get_ownerUserId(p)) == *id1) {
		*posts_i1_found+=1;
	}
	if(atoi(get_ownerUserId(p)) == *id2){
		*posts_i2_found+=1;
	}
	*i = 0;
	*f = 0;
	while (*i < *queuef_size && *f == 0) {
		if ((*(qf+*i)) == get_postId(p)) {
			*f = 1;
		}
		*i+=1;
	}			
}
/**
 * QUERY 9
 * Devolve as últimas N perguntas em que dois utilizadores participaram
*/
LONG_list both_participated(TAD_community com, long id1, long id2, int N){
	USER_HT user1 = find_user(com->users, id1);
	USER_HT user2 = find_user(com->users, id2);
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

	PostsList l1 = find_post_in_list(com->p, get_user_lastPost(user1));
	PostsList l2 = find_post_in_list(com->p, get_user_lastPost(user2));
	
	PostsList l = find_most_recent_post(l1, l2);
	if (!l)
		return NULL;

	while(l && res_size < N && (posts_i1_found < nposts1 || posts_i2_found < nposts2)) {
		POST p = get_post(l);
		if(get_ownerUserId(p) != NULL){
			if(isAnswer(p)) {
				if(atoi(get_ownerUserId(p)) == id1 && get_parent_owner(com->p, p) != NULL){
					addQueue_Answer(com, p, &id2, queue1, &queue1_size, queue2, &queue2_size, queuef, &queuef_size, &i, &f);
					posts_i1_found++;
				}
				else if(atoi(get_ownerUserId(p)) == id2 && get_parent_owner(com->p, p) != NULL) {
					addQueue_Answer(com, p, &id1, queue2, &queue2_size, queue1, &queue1_size, queuef, &queuef_size, &i, &f);
					posts_i2_found++;
				}
			}
		else if (isQuestion(p)) {
				addQueue_Question(p, &id1, &posts_i1_found, &id2, &posts_i2_found, &i, &f, queuef, &queuef_size);
				if (f == 1) {
					set_list(res, res_size ,get_postId(p));
					res_size++;
				}
				f = 0;
			}
		}
		l = get_next(l);
	}

	for (; res_size < N; res_size++ )
		set_list(res, res_size ,0);

	return res;
}

/**
 *	Função auxiliar para a query 10 que calcula a pontuação de um post
 */
int calculates_score(POST ans, USER_HT user){
	return get_score(ans)*0.45 + (get_user_reputation(user))*0.25 + get_score(ans)*0.2 + get_comments(ans)*0.1;
}


/**
 * QUERY 10
 * Obtém a melhor resposta dado o ID de uma pergunta
*/
long better_answer(TAD_community com, long id){
	PostsList l;
	int n =0;
	POST p = find_post(com->p, id);
	if(!p) return 0;
	int nanswers = get_nanswers(p);
	if (nanswers == 0) return 0;
	int best, temp; 
	long answer;

	l = find_by_date(com->p, get_lastActivityDate(p));

	while(l != NULL && n < 1 && (compare_date_list(p, get_post(l)) != -1)){
		POST ans = get_post(l);
		if(isAnswer(ans) && get_parent(ans) == id){
			n++;
			USER_HT user = find_user(com->users, atoi(get_ownerUserId(ans)));
			best = calculates_score(ans, user);
			answer = get_postId(ans);
		}
		l = get_next(l);
	} 

	while(l != NULL && n < nanswers && (compare_date_list(p, get_post(l)) != -1)){
		POST ans = get_post(l);
		if(isAnswer(ans) && get_parent(ans) == id){
			n++;
			USER_HT user = find_user(com->users, atoi(get_ownerUserId(ans)));
			temp = calculates_score(ans, user);
			if(temp > best){
				best = temp;
				answer = get_postId(ans);
			}
		}
		l = get_next(l);
	}
	return answer;
}
/**
 * Função auxiliar para a query 11 que atualiza o número de vezes que uma certa tag é utilizada
 */
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
/**
 * Função auxiliar para a query 11 que compara o número de vezes que uma tag é utilizada
*/
int compare_numTag(gconstpointer a, gconstpointer b){
	AUXTAG a1 = (AUXTAG) a;
	AUXTAG a2 = (AUXTAG) b;

	if(a1->num > a2->num)
		return -1;							
	else if (a1->num < a2->num) 
		return 1;
	else if (a1->id < a2->id) 
		return -1;
	else if (a1->id > a2->id) 
		return 1;
	else return 0;
}
/**
 * QUERY 11
 * Devolve os IDs das N tags mais utilizadas pelos N utilizadores com mais reputação
 */
LONG_list most_used_best_rep(TAD_community com, int N, Date begin, Date end){
	LONG_list list = get_N_users_with_most_reputation(com->users, N);
	GHashTable* h = g_hash_table_new (g_direct_hash, g_direct_equal);
	PostsList l = find_by_date(com->p, end);
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
				g_slist_free(tags);
				tags = NULL;
			}
		}
		l = get_next(l);
	}
	LONG_list res = create_list(N);
	aux = g_slist_sort(aux, compare_numTag);
	
	int j = 0;
	GSList* tofree = aux;
	while(aux && j < N){
		set_list(res, j, ((AUXTAG) aux->data)->id);
		free(aux->data);
		j++;
		aux = aux->next;
	}
	while(j < N){
		set_list(res, j, 0);
		j++;
	}
	g_hash_table_destroy (h);
	g_slist_free (tofree);
	return res;
}

TAD_community clean(TAD_community com) {
	free_users(com->users);
	free_posts(com->p);
	free_tags(com->t);
	return com;
}