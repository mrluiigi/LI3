#include "../../include/interface.h"
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
	char *ownerUserId;
	char *postTypeId;
	char *parentId;
}*POST;

typedef struct user_ht{
	char *name;
	char *shortBio;
}*USER_HT;


TAD_community init(){
	TAD_community t;
	t = malloc(sizeof(struct TCD_community));
	t->postshash = g_hash_table_new (g_direct_hash, g_direct_equal);
	t->usershash = g_hash_table_new (g_direct_hash, g_direct_equal);
	return t;
}

/*
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

		if (!strcmp(p->postTypeId, "1")){
			p->title = (char *) xmlGetProp(ptr, (xmlChar *) "Title");
			p->ownerUserId =  (char *) xmlGetProp(ptr, (xmlChar *) "OwnerUserId");

		}
		else if(!strcmp(p->postTypeId, "2"))
			p->parentId = (char *) xmlGetProp(ptr, (xmlChar *) "ParentId");

		g_hash_table_insert(com->postshash, GINT_TO_POINTER(r_posts), p);
		ptr = ptr->next->next;

	}

	xmlFreeDoc(doc);


}


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
		g_hash_table_insert(com->usershash, GINT_TO_POINTER(r_users), u);
		ptr = ptr->next->next;

	}

	xmlFreeDoc(doc);
}


TAD_community load(TAD_community com, char* dump_path){

	loadPosts(com, dump_path, "Posts.xml");

	loadUsers(com, dump_path, "Users.xml");
	
	return com;
}


//Função de teste
STR_pair info_from_post(TAD_community com, long id){
	POST p, z1;
	USER_HT q, z2;

	STR_pair pair;

	p = g_hash_table_lookup(com->postshash, GINT_TO_POINTER(id));
	
	if(p==NULL){
		fprintf(stderr, "Id não válido\n");
		return NULL;
	} 

	if(!strcmp(p->postTypeId, "1")){
		char * key;
		key = (char *) p->ownerUserId;
		int r = atoi(key);
		q = g_hash_table_lookup(com->usershash, GINT_TO_POINTER(r));
		pair = create_str_pair(p->title, q->name);
	}
	else if(!strcmp(p->postTypeId, "2")){
		char *key1;
		key1 = (char *) p->parentId;
		int r1 = atoi(key1);
		z1 = g_hash_table_lookup(com->postshash, GINT_TO_POINTER(r1));

		char *key2;
		key2 = (char *) z1->ownerUserId;
		int r2 = atoi(key2);
		z2 = g_hash_table_lookup(com->usershash, GINT_TO_POINTER(r2));
		pair = create_str_pair(z1->title, z2->name);	

	}
	return pair;
}