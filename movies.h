#ifndef MOVIE
#define MOVIE

typedef struct actor {
	char *name;
	struct actor *next;
} actor_t;

typedef struct date {
	int month_watched;
	int day_watched;
	int year_watched;
} date_t;

typedef struct movie {
	char *name;
	char *comments;
	float rating;
	date_t date_watched;
	actor_t *actors;	
	struct movie *prev;
	struct movie *next;
} movie_t;

movie_t *loaded_movies;

#endif
