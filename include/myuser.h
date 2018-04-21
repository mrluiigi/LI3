#include <glib.h>
#include "/usr/include/libxml2/libxml/parser.h"
#include "/usr/include/libxml2/libxml/xmlIO.h"


//#ifndef HEADER_FILE
//#define HEADER_FILE


typedef struct user_ht{
	int id;
	char *name;
	char *shortBio;
	unsigned short nr_posts;
	GSList *lastPost;
	int reputation;
}*USER_HT;

typedef struct user_ll{
	int id;
	unsigned short nr_posts;
}*USER_LL;



USER_HT create_myuser(xmlNodePtr ptr);


//#endif