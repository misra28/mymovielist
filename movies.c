#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include "movies.h"

#define DEBUG

movie_t *loaded_movies;

/* movies.h:

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
*/

void save_movie_list() {
	FILE *fp = NULL;
	int status = 0;
	char movie_buffer[100] = { 0 };
	movie_t *nav = loaded_movies;
	actor_t *actornav = NULL;
	int i = 0;

	if (nav == NULL) {
		printf("There are currently no movies in your list to save to a file.\n");	
		return;
	}

	while (status < 1) {
		printf("Enter the name of the file you would like to save to:\n");
		status = scanf("%99[^\n]", movie_buffer);
		while (getchar() != '\n');
#ifdef DEBUG
		printf("status = %d, movie_buffer = %s\n", status, movie_buffer);
#endif
		fp = fopen(movie_buffer, "w");
		if (status < 1 || fp == NULL) {
			printf("Incorrect input. ");
			clearerr(stdin);
			status = 0;
		}
	}
	
	while (nav != NULL) {
		status = fprintf(fp, "%lu|%s\n%lu|%s\n%f|%d|%d/%d/%d|%d\n",
				nav->name_length, nav->name, nav->comments_length,
				nav->comments, nav->rating, nav->runtime, nav->date_watched.month_watched,
				nav->date_watched.day_watched, nav->date_watched.year_watched, nav->actor_count);
		if (status < 10) break;
		
#ifdef DEBUG
		printf("sizeof(nav) == %lu\n", sizeof(nav));
		printf("\n%lu|%s\n%lu|%s\n%f|%d|%d/%d/%d|%d\n\n",
				nav->name_length, nav->name, nav->comments_length,
				nav->comments, nav->rating, nav->runtime, nav->date_watched.month_watched,
				nav->date_watched.day_watched, nav->date_watched.year_watched, nav->actor_count);
#endif

		actornav = nav->actors;
		for (i = 0; i < nav->actor_count; i++) {
			status = fprintf(fp, "%lu:%s\n", actornav->name_length, actornav->name);
			if (status < 1) break;
			actornav = actornav->next;
		}

		nav = nav->next;
		status = fprintf(fp, "\n");
		if (status < 1) break;
	}

	if (status == 1) {
		printf("\nFile successfully saved!\n");
	}
	else {
		printf("\nThere was an error writing to the file.\n\n");
	}

	fclose(fp);
	fp = NULL;
}

void load_movie_list() {
	FILE *fp = NULL;
	int status = 0;
	char movie_buffer[100] = { 0 };
	movie_t *nav = loaded_movies;
	movie_t *created_movie = NULL;
	actor_t *actornav = NULL;
	actor_t *created_actor = NULL;

	if (loaded_movies != NULL && status < 1) {
		printf("WARNING: You will lose access to the movies you have loaded up.\n");
		printf("Please type 'Yes' to confirm that you have saved your loaded movies to a file:\n");
		status = scanf("%99[^\n]", movie_buffer);
		while (getchar() != '\n');
#ifdef DEBUG
		printf("status = %d, movie_buffer = %s\n", status, movie_buffer);
#endif

		if (status < 1) {
			printf("Incorrect input. ");
			clearerr(stdin);
			status = 0;
		} else if (strcmp(movie_buffer, "Yes") != 0) {
			printf("You did not enter 'Yes' to confirm.\n");
			return;
		}
	}
	
	status = 0;
	while (status < 1) {
		printf("Enter the name of the existing file you would like to load from:\n");
		printf("Alternatively, enter 'exit' to quit.\n");
		status = scanf("%99[^\n]", movie_buffer);
		while (getchar() != '\n');
#ifdef DEBUG
		printf("status = %d, movie_buffer = %s\n", status, movie_buffer);
#endif
		if (strcmp(movie_buffer, "exit") == 0) {
			printf("Canceling...\n");
			return;
		}

		fp = fopen(movie_buffer, "r");

		if (status < 1 || fp == NULL) {
			printf("Incorrect input. ");
			clearerr(stdin);
			status = 0;
		}
	}

	// Remove all movies
	while (loaded_movies != NULL) {
		remove_movie(&loaded_movies);
	}

	while (feof(fp) == 0) {
		created_movie = (movie_t *) malloc(sizeof(movie_t));
		if (loaded_movies == NULL) {
			loaded_movies = created_movie;
		}
		else {
			created_movie->prev = nav;
			nav->next = created_movie;
		}

		nav = created_movie;
		status = fscanf(fp, "%lu|", &(nav->name_length));
		if (status < 1) break;

		nav->name = malloc(nav->name_length + 1);
		status = fscanf(fp, "%[^\n]\n", nav->name);
				
		status = fscanf(fp, "%lu|", &(nav->comments_length));
		if (status < 1) break;

		nav->comments = (char *) malloc(nav->comments_length + 1);
		status = fscanf(fp, "%[^\n]\n%f|%d|%d/%d/%d|%d\n",
				nav->comments, &(nav->rating), &(nav->runtime), &(nav->date_watched.month_watched),
				&(nav->date_watched.day_watched), &(nav->date_watched.year_watched), &(nav->actor_count));
		if (status < 7) break;

		nav->actors = NULL;
		actornav = nav->actors;
		for (int i = 0; i < nav->actor_count; i++) {
			created_actor = (actor_t *) malloc(sizeof(actor_t));
			
			if (nav->actors == NULL) {
				nav->actors = created_actor;
			}
			else {
				actornav->next = created_actor;
			}
			
			actornav = created_actor;
			status = fscanf(fp, "%lu:", &(actornav->name_length));
			if (status < 1) break;
			
			actornav->name = malloc(actornav->name_length + 1);
			status = fscanf(fp, "%[^\n]\n", actornav->name);
			if (status < 1) break;

#ifdef DEBUG
			printf("sizeof(actornav) == %lu\n", sizeof(actornav));
			printf("actornav->name_length = %lu\n", actornav->name_length);
			printf("actornav->name = %s\n", actornav->name);
#endif
		}
		actornav->next = NULL;
		fscanf(fp, "\n");
#ifdef DEBUG
		printf("sizeof(nav) == %lu\n", sizeof(nav));
		printf("\n%lu|%s\n%lu|%s\n%f|%d|%d/%d/%d|%d\n\n",
				nav->name_length, nav->name, nav->comments_length,
				nav->comments, nav->rating, nav->runtime, nav->date_watched.month_watched,
				nav->date_watched.day_watched, nav->date_watched.year_watched, nav->actor_count);
#endif	
	}

	if (feof(fp) != 0) {
		printf("\nList successfully loaded!\n\n");
	}
	else {
		printf("\nThere was an error loading from the file.\n\n");
	}

	fclose(fp);
	fp = NULL;
}

void remove_movie(movie_t **remove_this) {
	movie_t *remove = *remove_this;

	if (remove_this == NULL || remove == NULL) {
		return;
	}

	if (remove->prev == NULL) {
		loaded_movies = remove->next;
	}

	if (remove->prev != NULL) {
		remove->prev->next = remove->next;
	}

	if (remove->next != NULL) {
		remove->next->prev = remove->prev;
	}

	remove->prev = NULL;
	remove->next = NULL;
	free(remove->name);
	remove->name = NULL;
	free(remove->comments);
	remove->comments = NULL;
	
	while (remove->actors != NULL) {
		remove_actor(remove, remove->actors->name);
	}

	free(*remove_this);
	*remove_this = NULL;
	remove_this = NULL;
}

void set_name(movie_t *created_movie, char *movie_buffer) {
	if (created_movie->name != NULL) {
		free(created_movie->name);
		created_movie->name = NULL;
	}

	created_movie->name = malloc(strlen(movie_buffer) + 1);
	strncpy(created_movie->name, movie_buffer, strlen(movie_buffer));
	created_movie->name[strlen(movie_buffer)] = '\0';
	created_movie->name_length = (size_t) strlen(movie_buffer);
}

void set_comments(movie_t *created_movie, char *movie_buffer) {
	if (created_movie->comments != NULL) {
		free(created_movie->comments);
		created_movie->comments = NULL;
	}

	created_movie->comments = malloc(strlen(movie_buffer) + 1);
	strncpy(created_movie->comments, movie_buffer, strlen(movie_buffer));
	created_movie->comments[strlen(movie_buffer)] = '\0';
	created_movie->comments_length = (size_t) strlen(movie_buffer);
}

void set_rating(movie_t *created_movie, float rating_buffer) {
	if (rating_buffer > 10) {
		rating_buffer = 10;
	}
	else if (rating_buffer < 1) {
		rating_buffer = 1;
	}
	created_movie->rating = rating_buffer;
}

void set_runtime(movie_t *created_movie, int runtime_buffer) {
	created_movie->runtime = runtime_buffer;
}

void set_date(movie_t *created_movie, int month_buffer, int day_buffer, int year_buffer) {
	if (month_buffer > 12) {
		month_buffer = 12;
		printf("The month number was reduced to %d.\n", month_buffer);
	}

	if (day_buffer > 31) {
		day_buffer = 31;
		printf("The day number was reduced to %d.\n", day_buffer);
	}

	created_movie->date_watched.month_watched = month_buffer;
	created_movie->date_watched.day_watched = day_buffer;
	created_movie->date_watched.year_watched = year_buffer;
}

void add_actor(movie_t *created_movie, char *movie_buffer) {
	actor_t *created_actor = malloc(sizeof(actor_t));
	created_actor->name = malloc(strlen(movie_buffer) + 1);
	strncpy(created_actor->name, movie_buffer, strlen(movie_buffer));
	created_actor->name[strlen(movie_buffer)] = '\0';
	created_actor->name_length = (size_t) strlen(movie_buffer);

	created_actor->next = created_movie->actors;
	created_movie->actors = created_actor;
	created_movie->actor_count += 1;
	created_actor = NULL;
}

void remove_actor(movie_t *movie, char *name) {
	int found_actor = 0;
	actor_t *actornav = movie->actors;
	if (actornav == NULL) return;

	actor_t *actornav2 = actornav->next;
	if (strcmp(actornav->name, name) == 0) {
		movie->actors = actornav2;
		free(actornav->name);
		actornav->name = NULL;
		free(actornav);
		actornav = NULL;
		return;
	}

	while (actornav2 != NULL && found_actor == 0) {
		if (strcmp(actornav2->name, name) == 0) {
			found_actor = 1;
			break;
		}
		actornav = actornav->next;
		actornav2 = actornav2->next;
	}

	if (found_actor == 1) {
		actornav->next = actornav2->next;
		actornav2->next = NULL;
		free(actornav2->name);
		actornav2->name = NULL;
		free(actornav2);
		actornav2 = NULL;
	}

	movie->actor_count -= 1;
}

void add_new_movie(movie_t **entered_movie) {
	int status = 0;
	char movie_buffer[300] = { 0 };
	float rating_buffer = 0.0;
	int month_buffer = 0;
	int day_buffer = 0;
	int year_buffer = 0;
	int runtime_buffer = 0;
	int actor_count = 0;
	
	*entered_movie = malloc(sizeof(movie_t));
	movie_t *created_movie = *entered_movie;

	// Ask for movie name
	while (status < 1) {
		printf("Enter the name of the movie:\n");
		status = scanf("%299[^\n]", movie_buffer);
		while (getchar() != '\n');
#ifdef DEBUG
		printf("status = %d, movie_buffer = '%s'\n", status, movie_buffer);
#endif
		if (status < 1) {
			printf("Incorrect input. ");
			clearerr(stdin);
		}
	}
	created_movie->name = NULL;
	set_name(created_movie, movie_buffer);
#ifdef DEBUG
	printf("Successfully identified '%s' as the movie name with a length of %d\n", created_movie->name, (int) created_movie->name_length);
#endif
	printf("\n");

	// Ask for comments
	status = 0;
	while (status < 1) {
		printf("Enter your comments for the movie:\n");
		status = scanf("%299[^\n]", movie_buffer);
		while (getchar() != '\n');
#ifdef DEBUG
		printf("status = %d, movie_buffer = '%s'\n", status, movie_buffer);
#endif
		if (status < 1) {
			printf("Incorrect input. ");
			clearerr(stdin);
		}
	}
	created_movie->comments = NULL;
	set_comments(created_movie, movie_buffer);
#ifdef DEBUG
	printf("Successfully identified the comments with a length of %d: '%s'\n", (int) created_movie->comments_length, created_movie->comments);
#endif
	printf("\n");

	// Ask for rating
	status = 0;
	while (status < 1) {
		printf("Enter your rating for the movie (from 1 to 10):\n");
		status = scanf("%f", &rating_buffer);
		while (getchar() != '\n');
#ifdef DEBUG
		printf("status = %d, rating_buffer = %f\n", status, rating_buffer);
#endif
		if (status < 1) {
			printf("Incorrect input. ");
			clearerr(stdin);
		}
	}

	set_rating(created_movie, rating_buffer);
#ifdef DEBUG
	printf("Successfully identified the rating: %f\n", created_movie->rating);
#endif
	printf("\n");

	// Ask for runtime
	status = 0;
	while (status < 1) {
		printf("Enter the movie's runtime in minutes. If unknown, put 0:\n");
		status = scanf("%d", &runtime_buffer);
		while (getchar() != '\n');
#ifdef DEBUG
		printf("status = %d, runtime_buffer = %d\n", status, runtime_buffer);
#endif
		if (status < 1 || runtime_buffer < 0) {
			printf("Incorrect input. ");
			clearerr(stdin);
			status = 0;
		}
	}

	set_runtime(created_movie, runtime_buffer);
#ifdef DEBUG
	printf("Successfully identified the runtime: %d\n", created_movie->runtime);
#endif
	printf("\n");

	// Ask for date watched
	status = 0;
	while (status < 3) {
		printf("Enter the date you watched the movie in the format MM/DD/YYYY:\n");
		status = scanf("%d/%d/%d", &month_buffer, &day_buffer, &year_buffer);
		while (getchar() != '\n');
#ifdef DEBUG
		printf("status = %d\n", status);
#endif
		if (status < 3) {
			printf("Incorrect input. ");
			clearerr(stdin);
		}
	}

	set_date(created_movie, month_buffer, day_buffer, year_buffer);
#ifdef DEBUG
	printf("Successfully identified the date watched: %d/%d/%d\n", created_movie->date_watched.month_watched,
			created_movie->date_watched.day_watched, created_movie->date_watched.year_watched);
#endif
	printf("\n");

	// Ask for movie's actors
	status = 0;
	while (status < 1) {
		printf("Enter the number of main/supporting actors in this movie you would like to add (for a maximum of 10):\n");
		status = scanf("%d", &actor_count);
		while (getchar() != '\n');
#ifdef DEBUG
		printf("status = %d\n", status);
#endif
		printf("\n");
		if (status < 1 || actor_count < 0 || actor_count > 10) {
			printf("Incorrect input. ");
			clearerr(stdin);
			status = 0;
		}
	}
	
	created_movie->actor_count = 0;
	for (int i = 0; i < actor_count; i++) {
		status = 0;
		while (status < 1) {
			printf("Enter the name of actor #%d:\n", (i + 1));
			status = scanf("%299[^\n]", movie_buffer);
			while (getchar() != '\n');
#ifdef DEBUG
			printf("status = %d\n", status);
#endif
			printf("\n");
			if (status < 1) {
				printf("Incorrect input. ");
				clearerr(stdin);
			}
		}

		add_actor(created_movie, movie_buffer);
	}

	if (loaded_movies == NULL) {
		loaded_movies = created_movie;
	}
	else {
		created_movie->next = loaded_movies;
		loaded_movies->prev = created_movie;
		loaded_movies = created_movie;
	}

	loaded_movies->prev = NULL;
}

void print_movie_list() {
	movie_t *nav = loaded_movies;
	int counter = 1;
	actor_t *actnav = NULL;
	
	if (nav == NULL) {
		printf("You have not added or loaded any movies yet.\n");
		return;
	}

	printf("\n\tDisplaying My Movie List\n");

	while (nav != NULL) {
		actnav = nav->actors;
		printf("\n\tMOVIE %d: '%s'\n", counter, nav->name);
		
		if (nav->runtime > 0) {
			printf("\tRuntime: %d minute", nav->runtime);
			if (nav->runtime != 1) printf("s");
			printf("\n");
		} else {
			printf("\tRuntime: Unknown\n");
		}

		printf("\tRating: %.1f/10.0\n", nav->rating);
		printf("\tDate watched: %02d/%02d/%04d\n",
				nav->date_watched.month_watched, nav->date_watched.day_watched, nav->date_watched.year_watched);
		printf("\tActor count in this movie: %d\n", nav->actor_count);
		
		if (actnav == NULL) {
			printf("\t\t(none)\n");
		}

		for (int i = 0; i < nav->actor_count; i++) {
			printf("\t\t%s\n", actnav->name);
			actnav = actnav->next;
		}

		printf("\tComments about this movie: '%s'\n", nav->comments);
#ifdef DEBUG
		if (counter != 1 || nav->prev != NULL)
			printf("\tPrevious movie: %p, '%s'\n", nav->prev, nav->prev->name);
		if (nav->next != NULL)
			printf("\tNext movie: %p, '%s'\n", nav->next, nav->next->name);
#endif
		nav = nav->next;
		counter++;
		printf("\n");
	}
}
