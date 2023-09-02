#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "movies.h"

//#define DEBUG

void add_new_movie(movie_t **);
void print_movie_list();
void remove_movie(movie_t **);

/*
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
*/

void save_movie_list() {
	FILE *fp = NULL;
	int status = 0;

	fp = fopen();
}

void remove_movie(movie_t **remove_this) {
	movie_t *remove = *remove_this;

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
	free(remove->comments);
	free(remove->actors);
	free(remove);
	remove = NULL;
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
		printf("\tRating: %.1f/10.0\n", nav->rating);
		printf("\tDate watched: %02d/%02d/%04d\n",
				nav->date_watched.month_watched, nav->date_watched.day_watched, nav->date_watched.year_watched);
		printf("\tActors in this movie:\n");
		
		if (actnav == NULL) {
			printf("\t\t(none)\n");
		}

		while (actnav != NULL) {
			printf("\t\t%s\n", actnav->name);
			actnav = actnav->next;
		}

		printf("\tComments about this movie: '%s'\n\n", nav->comments);
		nav = nav->next;
		counter++;
	}
}

void add_new_movie(movie_t **entered_movie) {
	int status = 0;
	char movie_buffer[300] = { 0 };
	float rating_buffer = 0.0;
	int month_buffer = 0;
	int day_buffer = 0;
	int year_buffer = 0;
	int actor_count = 0;
	
	*entered_movie = malloc(sizeof(movie_t));
	movie_t *created_movie = *entered_movie;
	actor_t *created_actor = NULL;

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
	created_movie->name = malloc(strlen(movie_buffer) + 1);
	strncpy(created_movie->name, movie_buffer, strlen(movie_buffer));
#ifdef DEBUG
	printf("Successfully identified '%s' as the movie name\n", created_movie->name);
#endif

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
	created_movie->comments = malloc(strlen(movie_buffer) + 1);
	strncpy(created_movie->comments, movie_buffer, strlen(movie_buffer));
#ifdef DEBUG
	printf("Successfully identified the comments: '%s'\n", created_movie->comments);
#endif

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

	if (rating_buffer > 10) {
		rating_buffer = 10;
	}
	else if (rating_buffer < 1) {
		rating_buffer = 1;
	}
	created_movie->rating = rating_buffer;
#ifdef DEBUG
	printf("Successfully identified the rating: %f\n", created_movie->rating);
#endif

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
	created_movie->date_watched.month_watched = month_buffer;
	created_movie->date_watched.day_watched = day_buffer;
	created_movie->date_watched.year_watched = year_buffer;
#ifdef DEBUG
	printf("Successfully identified the date watched: %d/%d/%d\n", created_movie->date_watched.month_watched,
			created_movie->date_watched.day_watched, created_movie->date_watched.year_watched);
#endif
	
	// Ask for movie's actors
	status = 0;
	while (status < 1) {
		printf("Enter the number of main/supporting actors in this movie you would like to add (for a maximum of 10):\n");
		status = scanf("%d", &actor_count);
		while (getchar() != '\n');
#ifdef DEBUG
		printf("status = %d\n", status);
#endif
		if (status < 1 || actor_count < 0 || actor_count > 10) {
			printf("Incorrect input. ");
			clearerr(stdin);
		}
	}

	for (int i = 0; i < actor_count; i++) {
		status = 0;
		while (status < 1) {
			printf("Enter the name of actor #%d:\n", (i + 1));
			status = scanf("%299[^\n]", movie_buffer);
			while (getchar() != '\n');
#ifdef DEBUG
			printf("status = %d\n", status);
#endif
			if (status < 1) {
				printf("Incorrect input. ");
				clearerr(stdin);
			}
		}

		created_actor = malloc(sizeof(actor_t));
		created_actor->name = malloc(strlen(movie_buffer) + 1);
		strcpy(created_actor->name, movie_buffer);

		created_actor->next = created_movie->actors;
		created_movie->actors = created_actor;
		created_actor = NULL;
	}

	if (loaded_movies == NULL) {
		loaded_movies = created_movie;
	}
	else {
		created_movie->next = loaded_movies;
		loaded_movies->prev = created_movie;
		loaded_movies = created_movie;
	}
}

int main() {
	int continue_program = 0;
	int initial_input = 0;
	int status = 0;
	const int OPTION_COUNT = 6;
	movie_t *new_movie = NULL;	
	char movie_buffer[300] = { 0 };
	movie_t *nav = NULL;
	int found_desired_movie = 0;

	printf("Welcome to MyMovieList!\nSelect an option by entering the corresponding number.\n\n");
	
	while (continue_program == 0) {
		while (initial_input < 1 || initial_input > OPTION_COUNT) {
			printf("1. Add movie to my list\n");
			printf("2. Remove movie from my list\n");
			printf("3. View my list\n");
			printf("4. Save my list to a file\n");
			printf("5. Load my list from a file\n");
			printf("6. Exit\n\n");

			status = scanf("%d", &initial_input);
			while (getchar() != '\n');
			if (status < 1 || initial_input < 1 || initial_input > OPTION_COUNT) {
				printf("Incorrect input. Please enter a number from 1 to %d.\n\n", OPTION_COUNT);
				printf("initial_input is %d\n", initial_input);
				clearerr(stdin);	
			}
		}
		
		// Proceed using user input
		if (initial_input == 1) {
			add_new_movie(&new_movie);
			printf("Added a movie called '%s'!\n", loaded_movies->name);
		}
		else if (initial_input == 2) {
			status = 0;
			while (status < 1) {
				printf("Enter the name of the movie you wish to remove:\n");
				status = scanf("%299[^\n]", movie_buffer);
				while (getchar() != '\n');
#ifdef DEBUG
				printf("status = %d\n", status);
#endif
				if (status < 1) {
					printf("Incorrect input. ");
					clearerr(stdin);
				}
			}
			
			nav = loaded_movies;

			if (loaded_movies != NULL) {
				while (nav != NULL) {
					if (strcmp(nav->name, movie_buffer) == 0) {
						remove_movie(&nav);
						found_desired_movie = 1;
						break;
					}
					nav = nav->next;
				}
			}

			if (found_desired_movie == 0) {
				printf("The movie name you entered is not in your list. Please make sure it is entered correctly.\n\n");
			}
			else {
				printf("'%s' was successfully removed from your list!", movie_buffer);
			}
		}
		else if (initial_input == 3) {
			print_movie_list();
		}
		else if (initial_input == 6) {
			continue_program = -1;
			while (loaded_movies != NULL) {
				remove_movie(&loaded_movies);
			}
		}
		
		if (initial_input != 6) {
			printf("\nWhat would you like to do next?\n");
			continue_program = 0;
			initial_input = 0;	
		}
	}
	printf("Thank you so much for using MyMovieList!\n\n");
	return 1;
}
