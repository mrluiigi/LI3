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
	t->hash = g_hash_table_new (g_int_hash, g_str_equal);
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

	int r;

	POST p = malloc(sizeof(struct post));
	ptr = cur->xmlChildrenNode;
	ptr = ptr->next;
	while(ptr!=NULL){
		key = (char *) xmlGetProp(ptr, (xmlChar *) "Id");
		r = atoi(key);
		p->title = (char *) xmlGetProp(ptr, (xmlChar *) "Title");
		p->ownerUserId =  (char *) xmlGetProp(ptr, (xmlChar *) "OwnerUserId");
		p->postTypeId =  (char *) xmlGetProp(ptr, (xmlChar *) "PostTypeID");
		p->parentId = (char *) xmlGetProp(ptr, (xmlChar *) "ParentId");
		g_hash_table_insert(com->hash, &r, p);
		ptr = ptr->next->next;
	}
	printf("%d\n", g_hash_table_size(com->hash));
	
	return com;
}