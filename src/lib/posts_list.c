#include "posts_list.h"


/**
 * Estrutura que guarda informação sobre um post
 */
struct TCD_posts {
	GHashTable *hash;
	GSList *list;
	GHashTable *months_hash; 
};

/**
 * Inicializa a estrutura dos posts
 */
Posts init_posts(Posts posts) {
	posts = malloc(sizeof(struct TCD_posts));
	posts->hash = g_hash_table_new (g_direct_hash, g_direct_equal);
	posts->list = NULL;
	posts->months_hash = g_hash_table_new (g_direct_hash, g_direct_equal);
	return posts;
}

/**
 * Adiciona uma pergunta aos posts
 */
void add_question_to_posts(Posts posts, char * title, int nanswers, GSList * tags, Date lastActivityDate, char postTypeId, int id, char * ownerUserId, Date creationDate) {
	POST p = create_question(title, nanswers, tags, lastActivityDate, postTypeId, id, ownerUserId, creationDate);
	posts->list = g_slist_prepend(posts->list, p);
}

/**
 * Adiciona uma resposta aos posts
 */
void add_answer_to_posts(Posts posts, int parentId, int comments, int score, char postTypeId, int id, char * ownerUserId, Date creationDate) {
	POST p = create_answer(parentId, comments, score, postTypeId, id, ownerUserId, creationDate);
	posts->list = g_slist_prepend(posts->list, p);
}

/**
 * 
 */
void finalize(Posts posts) {
	posts->list = g_slist_sort (posts->list, compare_date_list);
	for(GSList* l = posts->list; l; l = l->next){
		POST p = (POST) l->data;
		g_hash_table_insert(posts->hash, GINT_TO_POINTER(get_postId(p)), l);	
		Date d = get_creationDate(p);
		int months_key = date_to_Key(get_year(d), get_month(d));
		g_hash_table_insert(posts->months_hash, GINT_TO_POINTER(months_key), l);
	}
}

/**
 * Devolve o ID do utilizador que publicou o post ao qual a resposta atual respondeu
 */
char * get_parent_owner(Posts posts, POST p) {
	POST parent = find_post(posts, get_parent(p));
	if (!parent) return 0;
	return get_ownerUserId(parent);
}

/**
 * Função que procura numa lista de datas uma data específica
 * A função devolve essa mesma lista a partir da data que lhe foi passada
 */
PostsList find_by_date(Posts posts, Date end) {
	GSList* l = posts->list;
	int year, month;
	year = get_year(end);
	month = get_month(end);
	if(month < 12)
		month++;
	else{
		year++;
		month = 1;
	}
	if(get_year(get_creationDate((POST) l->data)) == get_year(end)){
		if(get_month(get_creationDate((POST) l->data)) > get_month(end)){
			int key = date_to_Key(year, month);
			l = g_hash_table_lookup(posts->months_hash, GINT_TO_POINTER(key));
		}
		while(l && compare_date(end, get_creationDate((POST) l->data)) == 1){
			l=l->next;
		}
	}
	else if(get_year(get_creationDate((POST) l->data)) > get_year(end)){
		int key = date_to_Key(year, month);
		l = g_hash_table_lookup(posts->months_hash, GINT_TO_POINTER(key));
		while(l && compare_date(end, get_creationDate((POST) l->data)) == 1){ 
			l=l->next;
		}
	}
	return l;
}

/**
 * Função que procura numa lista de posts aquele que corresponde ao ID que lhe é passado como argumento
 * Devolve o post com o ID correspondente
 */
POST find_post(Posts posts, int id) {
	GSList* l = g_hash_table_lookup(posts->hash, GINT_TO_POINTER(id));
	if(l) return ((POST)l->data);
	else return NULL;
}

/**
 * Função que procura numa lista de posts aquele que corresponde ao ID que lhe é passado como argumento
 * Devolve todos os posts a seguir ao post encontrado
 */
PostsList find_post_in_list(Posts posts, int id) {
	PostsList l = g_hash_table_lookup(posts->hash, GINT_TO_POINTER(id));
	if(l) return l;
	else return NULL;
}

/**
 * Retorna -1 se o primeiro post tiver mais respostas, 1 se tiver menos respostas 0 se tiverem o mesmo número de respostas
 */
int compare_nanswers(gconstpointer a, gconstpointer b){
	POST p1 = ((POST) a);
	POST p2 = ((POST) b);

	if(get_nanswers(p1) > get_nanswers(p2))
		return -1;							
	else if (get_nanswers(p1) < get_nanswers(p2)) 
		return 1;
	else 
		return 0;
}

/**
* Compara as datas do primeiro elemnto de duas listas
*/
int compare_date_list (gconstpointer a, gconstpointer b) {
	POST p1 = (POST) a;
	POST p2 = (POST) b;

	return compare_date(get_creationDate(p1), get_creationDate(p2));
}

/**
* Devolve a lista com os posts
*/
PostsList get_posts_list(Posts posts) {
	return posts->list;
}

/**
* Avança para o post seguinte
*/
PostsList get_next(PostsList pl) {
	return pl->next;
}

/**
* Devolve a data de um certo post
*/
POST get_post(PostsList pl) {
	return (POST)pl->data;
}


/**
	Função para comparar os scores de dois POSTS
	Se a > b retorna -1 e se a < b retorna 1. Desta maneira a lista é ordenada por ordem decrescente
*/
/*
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
}*/

/**
* Dadas duas listas com posts a função retorna a lista com o post mais recente
*/
PostsList find_most_recent_post(PostsList l1, PostsList l2){
	if(!l1 || !l2) return NULL;
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
* Liberta a memória alocada para a struct posts_list
*/
void free_posts_list(gpointer data) {
	free_post((POST) data);
} 

/**
* Liberta a memória alocada para a struct posts
*/
void free_posts(Posts posts) {
	if (posts) {
		g_slist_free_full (posts->list,free_posts_list);
		g_hash_table_destroy (posts->hash);
		g_hash_table_destroy (posts->months_hash);
		free(posts);
	}
}