#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include "movies.h"

extern movie_t *loaded_movies;

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
	
	editing_menu(nav);
}

void editing_menu(movie_t *movie_to_edit) {
	int initial_input = 0;
	const int OPTION_COUNT = 8;
	int status = 0;

	printf("What properties for this movie would you like to change?\n\n");
	while (initial_input < 1 || initial_input > OPTION_COUNT) {
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
	}
	
	if (initial_input == 1) {
		edit_movie_name(movie_to_edit);
	}
	else if (initial_input == 2) {
		edit_comments(movie_to_edit);
	}
}

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
	printf("This movie's comments were changed to '%s'!\n\n", movie_to_edit->comments);
}

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
	printf("This movie's name was changed to '%s'!\n\n", movie_to_edit->name);
}
