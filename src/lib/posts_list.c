#include "posts_list.h"

static TAD_posts posts;

//static hash = posts->hash;

struct TCD_posts {
	GHashTable *hash;
	//????GHashTable *tagshash;
	GSList *list;
	GHashTable *months_hash; 
};


void init_posts() {
	posts = malloc(sizeof(struct TCD_posts));
	posts->hash = g_hash_table_new (g_direct_hash, g_direct_equal);
	posts->list = NULL;
	posts->months_hash = g_hash_table_new (g_direct_hash, g_direct_equal);
}

void add_question_to_posts(char * title, int nanswers, GSList * tags, Date lastActivityDate, char postTypeId, int id, char * ownerUserId, Date creationDate) {
	POST p = create_question(title, nanswers, tags, lastActivityDate, postTypeId, id, ownerUserId, creationDate);
	posts->list = g_slist_prepend(posts->list, p);
}

void add_answer_to_posts(int parentId, int comments, int upVotes, int downVotes, char postTypeId, int id, char * ownerUserId, Date creationDate) {
	POST p = create_answer(parentId, comments, upVotes, downVotes, postTypeId, id, ownerUserId, creationDate);
	posts->list = g_slist_prepend(posts->list, p);
}

void finalize() {
	posts->list = g_slist_sort (posts->list, compare_date_list);
	for(GSList* l = posts->list; l; l = l->next){
		POST p = (POST) l->data;
		gpointer k = get_owner_key(p);
		//find_and_set_user_lastPost(k, get_postId(p));
		g_hash_table_insert(posts->hash, get_post_key(p), l);
		
		Date d = get_creationDate(p);
		int months_key = date_to_Key(get_year(d), get_month(d));
		g_hash_table_insert(posts->months_hash, GINT_TO_POINTER(months_key), l);
	}
}

char * get_parent_owner(POST p) {
	gpointer parent_key = get_parent_key(p);
	if (!parent_key) return 0;
	POST parent = find_post(get_parent(p));
	if (!parent) return 0;
	return get_ownerUserId(parent);
}

GSList* find_by_date(Date end) {
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


POST find_post(int id) {
	GSList* l = g_hash_table_lookup(posts->hash, GINT_TO_POINTER(id));
	if(l) return ((POST)l->data);
	else return NULL;
}

GSList* find_post_in_list(int id) {
	GSList* l = g_hash_table_lookup(posts->hash, GINT_TO_POINTER(id));
	if(l) return l;
	else return NULL;
}

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


/*
* Compara as datas do primeiro elemnto de duas listas
*/
int compare_date_list (gconstpointer a, gconstpointer b) {
	POST p1 = (POST) a;
	POST p2 = (POST) b;

	return compare_date(get_creationDate(p1), get_creationDate(p2));
}
//temporaria
PostsList get_posts_list() {
	return posts->list;
}

PostsList get_next(PostsList pl) {
	return pl->next;
}

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

//????????????
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
