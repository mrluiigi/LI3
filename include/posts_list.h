#include <glib.h>
#include <string.h>
#include "date.h"
#include "mydate.h"
#include "tags.h"
#include "post.h"
#include "/usr/include/libxml2/libxml/parser.h"
#include "/usr/include/libxml2/libxml/xmlIO.h"


//#ifndef HEADER_FILE
//#define HEADER_FILE

char * get_parent_owner( GHashTable* posts, POST p);

GSList* find_by_date(GSList* posts_list, GHashTable *monthsHash, Date date);

POST find_post(GHashTable* h, int id);

GSList * find_post_in_list(GHashTable* h, int id);

int compare_nanswers(gconstpointer a, gconstpointer b);

//#endif