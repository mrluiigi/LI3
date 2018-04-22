#include "myuser.h"
// GHashTable* usershash, GSList* users_by_nr_posts, GSList* users_by_reputation
USER_HT create_myuser(xmlNodePtr ptr) {
	USER_HT u = malloc(sizeof(struct user_ht));
	u->id = atoi((char *) xmlGetProp(ptr, (xmlChar *) "Id"));
	u->name = (char *) xmlGetProp(ptr, (xmlChar *) "DisplayName");
	u->shortBio = (char *) xmlGetProp(ptr, (xmlChar *) "AboutMe");
	u->nr_posts = 0;
	u->lastPost = NULL;
	u->reputation = atoi((char*) xmlGetProp(ptr, (xmlChar *) "Reputation"));
	u->lastPost = NULL;
	return u;
}

void set_user_lastPost(USER_HT user, GSList* l){
	if(user->lastPost == NULL){
		user->lastPost = l;
	}
}


int get_user_nr_posts(USER_HT user) {
	if (!user) return 0;
	return user->nr_posts;
}

GSList* get_user_lastPost(USER_HT user) {
	if (!user) return NULL;
	return user->lastPost;
}