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
	int lastPost;
	int reputation;
}*USER_HT;


USER_HT create_myuser(xmlNodePtr ptr);

void set_user_lastPost(USER_HT user, int postId);

int  get_user_nr_posts(USER_HT user);

int get_user_lastPost(USER_HT user);

//#endif