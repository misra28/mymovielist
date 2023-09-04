#ifndef MOVIE
#define MOVIE

typedef struct actor {
	char *name;
	struct actor *next;
	size_t name_length;
} actor_t;

typedef struct date {
	int month_watched;
	int day_watched;
	int year_watched;
} date_t;

typedef struct movie {
	date_t date_watched;
	float rating;
	char *name;
	char *comments;
	actor_t *actors;	
	struct movie *prev;
	struct movie *next;
	size_t name_length;
	size_t comments_length;
	int runtime;
	int actor_count;
} movie_t;

movie_t *loaded_movies;

void add_new_movie(movie_t **);
void print_movie_list();
void remove_movie(movie_t **);
void save_movie_list();
void load_movie_list();

#endif
