#ifndef _POSTS_LIST_H_
#define _POSTS_LIST_H_

#include <glib.h>
#include <string.h>
#include "date.h"
#include "mydate.h"
#include "tags.h"
#include "post.h"
#include "/usr/include/libxml2/libxml/parser.h"
#include "/usr/include/libxml2/libxml/xmlIO.h"


typedef struct TCD_posts * TAD_posts;

typedef GSList * PostsList;

void init_posts();

void add_question_to_posts(char * title, int nanswers, GSList * tags, Date lastActivityDate, char postTypeId, int id, char * ownerUserId, Date creationDate);

void add_answer_to_posts(int parentId, int comments, int score, char postTypeId, int id, char * ownerUserId, Date creationDate);

void finalize();

char * get_parent_owner(POST p);

GSList* find_by_date(Date date);

POST find_post(int id);

GSList * find_post_in_list(int id);

int compare_nanswers(gconstpointer a, gconstpointer b);

int compare_date_list (gconstpointer a, gconstpointer b);

PostsList get_posts_list();

PostsList get_next();

POST get_post(PostsList pl);

PostsList find_most_recent_post(PostsList l1, PostsList l2);

//int compare_score(gconstpointer a, gconstpointer b);
#endif