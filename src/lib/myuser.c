#include "myuser.h"
// GHashTable* usershash, GSList* users_by_nr_posts, GSList* users_by_reputation
USER_HT create_myuser(xmlNodePtr ptr) {
	USER_HT u = malloc(sizeof(struct user_ht));
	u->id = atoi((char *) xmlGetProp(ptr, (xmlChar *) "Id"));
	u->name = (char *) xmlGetProp(ptr, (xmlChar *) "DisplayName");
	u->shortBio = (char *) xmlGetProp(ptr, (xmlChar *) "AboutMe");
	u->nr_posts = 0;
	u->lastPost = 0;
	u->reputation = atoi((char*) xmlGetProp(ptr, (xmlChar *) "Reputation"));
	return u;
}

void set_user_lastPost(USER_HT user, int postId){
	if(user->lastPost == 0){
		user->lastPost = postId;
	}
}


int get_user_nr_posts(USER_HT user) {
	if (!user) return 0;
	return user->nr_posts;
}

int get_user_lastPost(USER_HT user) {
	if (!user) return 0;
	return user->lastPost;
}


/*
void free_myuser(USER_HT u) {
	if(u){
		free(u->id);
		free(u->name);
		free(u->id);
		free(u->nr_posts);
		free(u->id);
	}
}*/