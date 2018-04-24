#include <glib.h>
#include <string.h>
#include "date.h"
#include "mydate.h"
#include "tags.h"
#include "common.h"
#include "/usr/include/libxml2/libxml/parser.h"
#include "/usr/include/libxml2/libxml/xmlIO.h"


typedef struct post * POST;

POST create_question(char * title, int nanswers, GSList * tags, Date lastActivityDate, char postTypeId, int id, char * ownerUserId, Date creationDate);

POST create_answer(int parentId, int comments, int upVotes, int downVotes, char postTypeId, int id, char * ownerUserId, Date creationDate);

int get_postId(POST p);

gpointer get_post_key(POST p);

gpointer get_owner_key(POST p);

gpointer get_parent_key(POST p);

int get_comments(POST p);

int get_parent(POST p);

char * get_ownerUserId(POST p);

GSList * get_tags(POST p);

int get_nanswers(POST p);

int contains_tag(POST p, gpointer tag_id);

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

