/* Ahvan Misra, 9/30/2023
 *
 * This file contains the functions that involve changing the properties of movies.
 *
 */

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include "movies.h"

extern movie_t *loaded_movies;


/* void select_movie()
 *
 * Ask the user which movie they want to change the properties of
 *
 */
void select_movie() {
	if (loaded_movies == NULL) {
		printf("You currently do not have any movies loaded.\n\n");
		return;
	}

	int status = 0;
	char movie_buffer[300] = { 0 };
	movie_t *nav = loaded_movies;

	// Ask for movie name
	while (status < 1) {
		printf("Enter the name of the movie whose properties you would like to edit:\n");
		status = scanf("%299[^\n]", movie_buffer);
		while (getchar() != '\n');
#ifdef DEBUG
		printf("status = %d, movie_buffer = '%s'\n", status, movie_buffer);
#endif
		while (nav != NULL && strcmp(nav->name, movie_buffer) != 0) {
			nav = nav->next;
		}

		if (status < 1 || nav == NULL) {
			printf("Your input was not recognized as a movie name. Try again. ");
			clearerr(stdin);
			nav = loaded_movies;
			status = 0;
		}
	}
	printf("\n");
	editing_menu(nav);
} /* select_movie() */


/* void editing_menu()
 *
 * Ask the user which property they want to change
 *
 */
void editing_menu(movie_t *movie_to_edit) {
	int initial_input = 0;
	const int OPTION_COUNT = 8;
	int status = 0;

	printf("What properties for this movie would you like to change?\n\n");
	while (initial_input != OPTION_COUNT) {
		printf("1. Edit movie name\n");
		printf("2. Edit comments\n");
		printf("3. Edit rating\n");
		printf("4. Edit runtime\n");
		printf("5. Edit date\n");
		printf("6. Add actor\n");
		printf("7. Remove actor\n");
		printf("8. Exit\n\n");

		status = scanf("%d", &initial_input);
		while (getchar() != '\n');
		if (status < 1 || initial_input < 1 || initial_input > OPTION_COUNT) {
			printf("Incorrect input. Please enter a number from 1 to %d.\n\n", OPTION_COUNT);
			printf("initial_input is %d\n", initial_input);
			clearerr(stdin);
		}

		if (initial_input == 1) {
			edit_movie_name(movie_to_edit);
			printf("This movie's name was changed to '%s'!\n\n", movie_to_edit->name);
		}
		else if (initial_input == 2) {
			edit_comments(movie_to_edit);
			printf("This movie's comments were changed to '%s'!\n\n", movie_to_edit->comments);
		}
		else if (initial_input == 3) {
			edit_rating(movie_to_edit);
			printf("Rating successfully edited!\n\n");
		}
		else if (initial_input == 4) {
			edit_runtime(movie_to_edit);
			printf("Runtime successfully edited!\n\n");
		}
		else if (initial_input == 5) {
			edit_date(movie_to_edit);
			printf("Date successfully edited!\n\n");
		}
		else if (initial_input == 6) {
			new_actor(movie_to_edit);
			printf("Added a new actor!\n\n");
		}
		else if (initial_input == 7) {
			delete_actor(movie_to_edit);
			printf("Deleted an actor!\n\n");
		}
		else if (initial_input == 8) {
			return;
		}
		printf("What else would you like to change?\n");
	}
} /* editing_menu() */


/* void delete_actor()
 *
 * Prompt the user to remove an actor of their choice
 *
 */
void delete_actor(movie_t *movie_to_edit) {
	int status = 0;
	char movie_buffer[300] = { 0 };

	while (status < 1) {
		printf("Enter the name of the actor to remove:\n");
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
	printf("\n");
	remove_actor(movie_to_edit, movie_buffer);
} /* delete_actor() */


/* void new_actor()
 *
 * Prompt the user to add an actor of their choice
 *
 */
void new_actor(movie_t *movie_to_edit) {
	int status = 0;
	char movie_buffer[300] = { 0 };

	while (status < 1) {
		printf("Enter the name of the actor to add:\n");
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
	printf("\n");
	add_actor(movie_to_edit, movie_buffer);
} /* new_actor() */


/* void edit_date()
 *
 * Prompt the user to change the date they watched the movie
 *
 */
void edit_date(movie_t *movie_to_edit) {
	int status = 0;
	int month_buffer = 0;
	int day_buffer = 0;
	int year_buffer = 0;

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

	set_date(movie_to_edit, month_buffer, day_buffer, year_buffer);
#ifdef DEBUG
	printf("Successfully identified the date watched: %d/%d/%d\n", movie_to-edit->date_watched.month_watched,
			movie_to_edit->date_watched.day_watched, movie_to_edit->date_watched.year_watched);
#endif
	printf("\n");
} /* edit_date() */


/* void edit_runtime()
 *
 * Prompt the user to change the runtime of the movie
 *
 */
void edit_runtime(movie_t *movie_to_edit) {
	int status = 0;
	int runtime_buffer = 0;

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

	set_runtime(movie_to_edit, runtime_buffer);
#ifdef DEBUG
	printf("Successfully identified the runtime: %d\n", movie_to_edit->runtime);
#endif
	printf("\n");
} /* edit_runtime() */


/* void edit_rating()
 *
 * Prompt the user to change their rating of the movie
 *
 */
void edit_rating(movie_t *movie_to_edit) {
	int status = 0;
	float rating_buffer = 0.0;

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

	set_rating(movie_to_edit, rating_buffer);
#ifdef DEBUG
	printf("Successfully identified the rating: %f\n", movie_to_edit->rating);
#endif
	printf("\n");
} /* edit_rating() */


/* void edit_comments()
 *
 * Prompt the user to change their comments for the movie
 *
 */
void edit_comments(movie_t *movie_to_edit) {
	int status = 0;
	char movie_buffer[300] = { 0 };

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
	printf("\n");

	set_comments(movie_to_edit, movie_buffer);
} /* edit_comments() */


/* void edit_movie_name()
 *
 * Prompt the user to change their movie's name
 *
 */
void edit_movie_name(movie_t *movie_to_edit) {
	int status = 0;
	char movie_buffer[300] = { 0 };

	while (status < 1) {
		printf("Enter the name of your movie:\n");
		status = scanf("%299[^\n]", movie_buffer);
		while (getchar() != '\n');
#ifdef DEBUG
		printf("status = %d, movie_buffer = '%s'\n", status, movie_buffer);
#endif

		if (status < 1) {
			printf("Invalid input. ");
			clearerr(stdin);
		}
	}
	printf("\n");

	set_name(movie_to_edit, movie_buffer);
} /* edit_movie_name() */
