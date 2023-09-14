#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "movies.h"

#define compare(X, Y) ((X) > (Y)) ? ((X) : (Y))

void sorting_menu() {
	int continue_sorting = 0;
	int initial_input = 0;
	int status = 0;
	const int OPTION_COUNT = 7;

	printf("Please choose a sorting option for your movies.\n\n");
	
	while (initial_input < 1 || initial_input > OPTION_COUNT) {
		printf("1. By name (alphabetically)\n");
		printf("2. By rating\n");
		printf("3. By runtime\n");
		printf("4. By first watched\n");
		printf("5. By most recently watched\n");
		printf("6. By actor count\n");
		printf("7. Exit\n\n");

		status = scanf("%d", &initial_input);
		while (getchar() != '\n');
		if (status < 1 || initial_input < 1 || initial_input > OPTION_COUNT) {
			printf("Incorrect input. Please enter a number from 1 to %d.\n\n", OPTION_COUNT);
			printf("initial_input is %d\n", initial_input);
			clearerr(stdin);
		}
	}

	if (initial_input == 1) {
		sort_movies(initial_input);
	}
	else if (initial_input == OPTION_COUNT) {
		return;
	}
}

void sort_movies(int sorting_type) {
	movie_t *new_list = NULL;
	movie_t *get_movie = NULL;
	movie_t *nav = NULL;

	while (nav != NULL) {
		get_movie = nav;
		nav = nav->next;
		if (get_movie->prev != NULL) {
			get_movie->prev->next = get_movie->next;
		}
		if (get_movie->next != NULL) {
			get_movie->next->prev = get_movie->prev;
		}
		get_movie->prev = NULL;
		get_movie->next = NULL;
		new_list = insert_movie(new_list, get_movie, sorting_type);
		get_movie = NULL;
	}
}

movie_t *insert_movie(movie_t *sorted_list, movie_t *insert, int sorting_type) {
	movie_t *nav = sorted_list;
	char *val1 = NULL;
	char *val2 = NULL;

	// If the sorted_list is currently empty
	if (sorted_list == NULL) {
		sorted_list = insert;
		return sorted_list;
	}

	// If the insert node's value is less than that of the head
	if (insert->val < sorted_list->val) {
		insert->next = sorted_list;
		insert->prev = NULL;
		sorted_list = insert;
		return sorted_list;
	}
	else if (sorted_list->next == NULL) {
		sorted_list->next = insert;
		insert->next = NULL;
		return sorted_list;
	}
	else {
		while (nav->next != NULL) {
			if (nav->val <= insert->val && insert->val <= nav->next->val) {
				insert->next = nav->next;
				nav->next = insert;
				return sorted_list;
			}
			nav = nav->next;
		}
	}
	nav->next = insert;
	return sorted_list;
}
