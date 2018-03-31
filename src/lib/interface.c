#include <interface.h>
#include <glib.h>
#include <stdio.h>
#include "/usr/include/libxml2/libxml/parser.h"
#include "/usr/include/libxml2/libxml/xmlIO.h"
#include <string.h>

struct TCD_community{
	GHashTable *postshash;
	GHashTable *usershash;
};

typedef struct post{
	char *title;
	char *postTypeId;
	char *ownerUserId;
	int parentId;
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

/**
*	Função que inicializa a estrutura de dados
*/
TAD_community init(){
	TAD_community t;
	t = malloc(sizeof(struct TCD_community));
	t->postshash = g_hash_table_new (g_direct_hash, g_direct_equal);
	t->usershash = g_hash_table_new (g_direct_hash, g_direct_equal);
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
		g_hash_table_insert(com->usershash, GINT_TO_POINTER(r_users), u);
		ptr = ptr->next->next;

	}

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
	
	char *key_posts;


	ptr = cur->xmlChildrenNode;
	ptr = ptr->next;
	while(ptr != NULL){
		POST p = malloc(sizeof(struct post));
		key_posts = (char *) xmlGetProp(ptr, (xmlChar *) "Id");
		int r_posts = atoi(key_posts);
		p->postTypeId =  (char *) xmlGetProp(ptr, (xmlChar *) "PostTypeId");

		char *userId = (char *) xmlGetProp(ptr, (xmlChar *) "OwnerUserId");
		//Caso o post possua informação sobre o OwnerUserId
		if(userId != NULL){
			int ownerUserId = atoi(userId);
			USER_HT user = g_hash_table_lookup(com->usershash, GINT_TO_POINTER(ownerUserId));
			user->nr_posts += 1;
			g_hash_table_insert(com->usershash, GINT_TO_POINTER(ownerUserId), user);
		}
		if (!strcmp(p->postTypeId, "1")){
			p->title = (char *) xmlGetProp(ptr, (xmlChar *) "Title");
			p->ownerUserId =  (char *) xmlGetProp(ptr, (xmlChar *) "OwnerUserId");
		}
		else if(!strcmp(p->postTypeId, "2")){
			p->parentId = atoi ((char *) xmlGetProp(ptr, (xmlChar *) "ParentId"));
		}
		g_hash_table_insert(com->postshash, GINT_TO_POINTER(r_posts), p);
		ptr = ptr->next->next;
	}
	
	xmlFreeDoc(doc);

}

/**
  Função que faz load dos ficheiros para a estrutura de dados
*/
TAD_community load(TAD_community com, char* dump_path){
	
	loadUsers(com, dump_path, "Users.xml");

	loadPosts(com, dump_path, "Posts.xml");

	return com;
}

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
	if(u1->nr_posts > u2->nr_posts)
		return 1;
	if(u1->nr_posts < u2->nr_posts)
		return (-1);
	else return 0;
}


//Falta corrigir sort e adicionar na LONG_LIST
LONG_list top_most_active(TAD_community com, int N){
	GSList * ll;
	USER_LL cur;
	LONG_list lista;
	lista = create_list(N);
	g_hash_table_foreach(com->usershash, (GHFunc)slist_append, (gpointer)&ll);
	//ll = g_slist_sort(ll, (GCompareFunc)compare_user_ll);
	for(int i = 0; i < N; i++){
		cur = g_slist_nth_data(ll, i);
		//set_list(lista, i, )
		printf("posts %d\n", cur->nr_posts);
		printf("id %d\n", cur->id);
	}
	return lista;
}
