#include <glib.h>
#include "/usr/include/libxml2/libxml/parser.h"
#include "/usr/include/libxml2/libxml/xmlIO.h"


//#ifndef HEADER_FILE
//#define HEADER_FILE

typedef struct user_ht * USER_HT;


USER_HT create_myuser(xmlNodePtr ptr);

int get_user_id(USER_HT user);

char * get_user_name(USER_HT user);

char * get_user_shortBio(USER_HT user);

void set_user_lastPost(USER_HT user, int postId);

int  get_user_nr_posts(USER_HT user);

int get_user_lastPost(USER_HT user);

int get_user_reputation(USER_HT user);

void increment_user_nr_posts(USER_HT user);

void free_myuser(USER_HT u);

//#endif