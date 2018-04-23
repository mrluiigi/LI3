#include "myuser.h"

struct user_ht{
	int id;
	char *name;
	char *shortBio;
	unsigned short nr_posts;
	int lastPost;
	int reputation;
};


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

int get_user_id(USER_HT user) {
	if (!user) return 0;
	return user->id;
}

char * get_user_name(USER_HT user) {
	if (!user) return NULL;
	return user->name;
}

char * get_user_shortBio(USER_HT user) {
	if (!user) return NULL;
	return user->shortBio;
}


int get_user_nr_posts(USER_HT user) {
	if (!user) return 0;
	return user->nr_posts;
}

int get_user_lastPost(USER_HT user) {
	if (!user) return 0;
	return user->lastPost;
}

int get_user_reputation(USER_HT user) {
	if (!user) return 0;
	return user->reputation;
}

void increment_user_nr_posts(USER_HT user) {
	user->nr_posts++;
}

void free_myuser(USER_HT u) {
	if(u){
		//free(u->name);
		//free(u->shortBio);
		free(u);
	}
}