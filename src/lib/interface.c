#include "../../include/interface.h"
#include <glib.h>
#include <stdio.h>
#include "/usr/include/libxml2/libxml/parser.h"
#include "/usr/include/libxml2/libxml/xmlIO.h"
#include <string.h>

struct TCD_community{
	GHashTable *hash;
};

typedef struct post{
	char *title;
	char *ownerUserId;
	char *postTypeId;
	char *parentId;
}*POST;


TAD_community init(){
	TAD_community t;
	t = malloc(sizeof(struct TCD_community));
	t->hash = g_hash_table_new (g_direct_hash, g_direct_equal);
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
	xmlDocPtr doc;

	xmlNodePtr cur, ptr;


	char *key;

	//Concatena o file ao dump_path
	char file[100];
	strcpy(file, filePath(dump_path, "Posts.xml"));
	//-------------------------
	doc = xmlParseFile(file);

	if(doc == NULL){
		fprintf(stderr, "Document not parsed successfully.\n");
		return com;
	}

	cur = xmlDocGetRootElement(doc);

	if(cur == NULL){
		fprintf(stderr, "empty document\n");
		xmlFreeDoc(doc);
		return com;
	}


	ptr = cur->xmlChildrenNode;
	ptr = ptr->next;
	while(ptr!=NULL){
	POST p = malloc(sizeof(struct post));
		key = (char *) xmlGetProp(ptr, (xmlChar *) "Id");
		int r = atoi(key);
		p->postTypeId =  (char *) xmlGetProp(ptr, (xmlChar *) "PostTypeId");

		if (!strcmp(p->postTypeId, "1")){
			p->title = (char *) xmlGetProp(ptr, (xmlChar *) "Title");
			p->ownerUserId =  (char *) xmlGetProp(ptr, (xmlChar *) "OwnerUserId");

		}
		else if(!strcmp(p->postTypeId, "2"))
			p->parentId = (char *) xmlGetProp(ptr, (xmlChar *) "ParentId");
		g_hash_table_insert(com->hash, GINT_TO_POINTER(r), p);
		//xmlFree(key);
		ptr = ptr->next->next;

	}
	return com;
}


//Função de teste
void myInfo_from_post(TAD_community com, int id){
	POST p;


	p = g_hash_table_lookup(com->hash, GINT_TO_POINTER(id));
	if(p->title!=NULL)
		printf("%s\n", p->title);
	else 
		printf("erro\n");
}