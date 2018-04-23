#include <glib.h>
#include "/usr/include/libxml2/libxml/parser.h"
#include "/usr/include/libxml2/libxml/xmlIO.h"
#include "myuser.h"
#include "list.h"



typedef struct users * USERS;


void init_users();

USER_HT  find_user(int id);

void add_myuser(xmlNodePtr ptr);

void sort_users_by_reputation();

void sort_users_by_nr_posts();

LONG_list get_N_users_with_most_reputation(int N);

void find_and_increment_user_nr_posts(gpointer id_key);

LONG_list get_N_users_with_most_nr_posts(int N);

void find_and_set_user_lastPost(gpointer key, int lastPostId);

void free_users();