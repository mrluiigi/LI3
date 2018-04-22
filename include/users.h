#include <glib.h>
#include "/usr/include/libxml2/libxml/parser.h"
#include "/usr/include/libxml2/libxml/xmlIO.h"
#include "myuser.h"
#include "list.h"



typedef struct users{
	GHashTable* hash;
	GSList* users_by_nr_posts;
	GSList* users_by_reputation;
}*USERS;


 USERS init_users();

USER_HT  find_user(USERS users, int id);

void add_myuser(USERS users, xmlNodePtr ptr);

void sort_users_by_reputation(USERS users);

void sort_users_by_nr_posts(USERS users);

LONG_list get_N_users_with_most_reputation(USERS users, int N);

void increment_user_nr_posts(USERS users, gpointer id_key);

LONG_list get_N_users_with_most_nr_posts(USERS users, int N);

void find_and_set_user_lastPost(USERS users, gpointer key, int lastPostId);