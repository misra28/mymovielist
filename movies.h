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

extern movie_t *loaded_movies;

void add_new_movie(movie_t **);
void print_movie_list();
void remove_movie(movie_t **);
void save_movie_list();
void load_movie_list();

void set_name(movie_t *, char *);
void set_comments(movie_t *, char *);
void set_rating(movie_t *, float);
void set_runtime(movie_t *, int);
void set_date(movie_t *, int, int, int);
void add_actor(movie_t *, char *);
void remove_actor(movie_t *, char *);

void sorting_menu();
void sort_movies(int, int);
movie_t *insert_movie(movie_t *, movie_t *, int, int);

void select_movie();
void editing_menu(movie_t *);
void edit_movie_name(movie_t *);
void edit_comments(movie_t *);

#endif
