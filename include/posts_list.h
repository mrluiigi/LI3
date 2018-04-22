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
	Date lastActivityDate;
}*QUESTION;

typedef struct answer{
	int parentId;
	int comments;
	int upVotes;
	int downVotes;
}*ANSWER;

typedef struct post {
	char postTypeId;
	QUESTION q; 
	ANSWER a; 
	int id;
	char * ownerUserId;
	Date creationDate;
}*POST;

POST create_post(xmlNodePtr ptr, GHashTable* tags);

int get_postId(POST p);

gpointer get_post_key(POST p);

gpointer get_owner_key(POST p);

gpointer get_parent_key(POST p);

int get_comments(POST p);

int get_parent(POST p);

char * get_parent_owner( GHashTable* posts, POST p);

char * get_ownerUserId(POST p);

GSList * get_tags(POST p);

int get_nanswers(POST p);

int contains_tag(POST p, gpointer tag_id);

GSList* find_by_date(GSList* posts_list, GHashTable *monthsHash, Date date);

int isQuestion(POST p);

int isAnswer(POST p);

char * get_title(POST p);

void addUpVote(POST p);

void addDownVote(POST p);

int get_upvotes(POST p);

int get_downvotes(POST p);

int get_score(POST p);

Date get_creationDate(POST p);

Date get_lastActivityDate(POST p);

POST find_post(GHashTable* h, int id);

GSList * find_post_in_list(GHashTable* h, int id);
//#endif