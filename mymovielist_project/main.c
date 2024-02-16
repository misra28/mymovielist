/* Ahvan Misra, 9/30/2023
 *
 */

#include <stdio.h>
#include <string.h>
#include "movies.h"

int main() {
	int continue_program = 0;
	int initial_input = 0;
	int status = 0;
	const int OPTION_COUNT = 8;
	movie_t *new_movie = NULL;
	char movie_buffer[300] = { 0 };
	movie_t *nav = NULL;
	int found_desired_movie = 0;

#ifdef DEBUG
	printf("TODO: Test the parse_text_input function.\n\n");
#endif

	printf("\n\n\nWelcome to MyMovieList!\nSelect an option by entering the corresponding number.\n\n");
	
	while (continue_program == 0) {
		while (initial_input < 1 || initial_input > OPTION_COUNT) {
			printf("1. Add a movie to this list\n");
			printf("2. Remove a movie from this list\n");
			printf("3. View my list\n");
			printf("4. Edit movie information\n");
			printf("5. Sort movies\n");
			printf("6. Save my list to a file\n");
			printf("7. Load a list from a file\n");
			printf("8. Exit\n\n");

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
			printf("\nAdded a movie called '%s'!\n", loaded_movies->name);
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
				printf("\nThe movie name you entered is not in your list. Please make sure it is entered correctly.\n");
			}
			else {
				printf("\n'%s' was successfully removed from your list!\n", movie_buffer);
			}
		}
		else if (initial_input == 3) {
			print_movie_list();
		}
		else if (initial_input == 4) {
			select_movie();
		}
		else if (initial_input == 5) {
			sorting_menu();
		}
		else if (initial_input == 6) {
			save_movie_list();
		}
		else if (initial_input == 7) {
			load_movie_list();
		}
		else if (initial_input == OPTION_COUNT) {
			continue_program = -1;
			while (loaded_movies != NULL) {
				remove_movie(&loaded_movies);
			}
		}
		
		if (initial_input != OPTION_COUNT) {
			printf("\nWhat would you like to do next?\n");
			continue_program = 0;
			initial_input = 0;
		}
	}
	printf("\nThank you so much for using MyMovieList!\n\n");
	return 0;
}
