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
	char *name;
	char *shortBio;
}*POST;


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


TAD_community load(TAD_community com, char* dump_path){
	xmlDocPtr doc_posts, doc_users;
	xmlNodePtr cur_posts, ptr_posts, cur_users, ptr_users;


	//Concatena o file ao dump_path
	char file_posts[100];
	strcpy(file_posts, filePath(dump_path, "Posts.xml"));
	//-------------------------
	doc_posts = xmlParseFile(file_posts);



	//returns!
	if(doc_posts == NULL){
		fprintf(stderr, "Document not parsed successfully.\n");
		return com;
	}


	cur_posts = xmlDocGetRootElement(doc_posts);

	//returns!
	if(cur_posts == NULL){
		fprintf(stderr, "empty document\n");
		xmlFreeDoc(doc_posts);
	}
	
	char *key_posts;

	ptr_posts = cur_posts->xmlChildrenNode;
	ptr_posts = ptr_posts->next;
	while(ptr_posts != NULL){
		POST p = malloc(sizeof(struct post));
		key_posts = (char *) xmlGetProp(ptr_posts, (xmlChar *) "Id");
		int r_posts = atoi(key_posts);
		p->postTypeId =  (char *) xmlGetProp(ptr_posts, (xmlChar *) "PostTypeId");

		if (!strcmp(p->postTypeId, "1")){
			p->title = (char *) xmlGetProp(ptr_posts, (xmlChar *) "Title");
			p->ownerUserId =  (char *) xmlGetProp(ptr_posts, (xmlChar *) "OwnerUserId");

		}
		else if(!strcmp(p->postTypeId, "2"))
			p->parentId = (char *) xmlGetProp(ptr_posts, (xmlChar *) "ParentId");

		g_hash_table_insert(com->postshash, GINT_TO_POINTER(r_posts), p);
		ptr_posts = ptr_posts->next->next;

	}

	xmlFreeDoc(doc_posts);


	char file_users[100];
	strcpy(file_users, filePath(dump_path, "Users.xml"));

	doc_users = xmlParseFile(file_users);

	if(doc_users == NULL){
		fprintf(stderr, "Document not parsed successfully.\n");
		return com;
	}

	cur_users = xmlDocGetRootElement(doc_users);

	if(cur_users == NULL){
		fprintf(stderr, "empty document\n");
		xmlFreeDoc(doc_users);
	}

	char *key_users;

	ptr_users = cur_users->xmlChildrenNode;
	ptr_users = ptr_users->next;
	while(ptr_users != NULL){
		POST q = malloc(sizeof(struct post));
		key_users = (char *) xmlGetProp(ptr_users, (xmlChar *) "Id");
		int r_users = atoi(key_users);
		q->name = (char *) xmlGetProp(ptr_users, (xmlChar *) "DisplayName");
		q->shortBio = (char *) xmlGetProp(ptr_users, (xmlChar *) "AboutMe");
		g_hash_table_insert(com->usershash, GINT_TO_POINTER(r_users), q);
		ptr_users = ptr_users->next->next;

	}

	xmlFreeDoc(doc_users);

	return com;
}


//Função de teste
void myInfo_from_post(TAD_community com, int id){
	POST p, q, z1, z2;
	p = g_hash_table_lookup(com->postshash, GINT_TO_POINTER(id));
	if(!strcmp(p->postTypeId, "1")){
		printf("%s\n", p->title);
		char * key;
		key = (char *) p->ownerUserId;
		int r = atoi(key);
		q = g_hash_table_lookup(com->usershash, GINT_TO_POINTER(r));
		printf("%s\n", q->name);
	}
	else if(!strcmp(p->postTypeId, "2")){
		char *key1;
		key1 = (char *) p->parentId;
		int r1 = atoi(key1);
		z1 = g_hash_table_lookup(com->postshash, GINT_TO_POINTER(r1));
		printf("%s\n", z1->title);

		char *key2;
		key2 = (char *) z1->ownerUserId;
		int r2 = atoi(key2);
		z2 = g_hash_table_lookup(com->usershash, GINT_TO_POINTER(r2));
		printf("%s\n", z2->name);		

	}
}
//teste
