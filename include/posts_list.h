#include <glib.h>
#include <string.h>
#include "date.h"
#include "mydate.h"
#include "tags.h"
#include "/usr/include/libxml2/libxml/parser.h"
#include "/usr/include/libxml2/libxml/xmlIO.h"


//#ifndef HEADER_FILE
//#define HEADER_FILE


typedef struct question{
	char *title;
	int nanswers;
	GSList *tags;
}*QUESTION;

typedef struct answer{
	int parentId;
	int comments;
}*ANSWER;


typedef struct post {
	char *postTypeId;
	QUESTION q; 
	ANSWER a; 
	int id;
	char *ownerUserId;
	int score;
	Date creationDate;
}*POST;

POST create_post(xmlNodePtr ptr, GHashTable* tags);

char * get_userId (POST p);

gpointer get_post_key(POST p);

GSList* find_by_date(GSList* posts_list, GHashTable *monthsHash, Date begin, Date end);

int isQuestion(POST p);

int isAnswer(POST p);

char * get_title(POST p);
//#endif