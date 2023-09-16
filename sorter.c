#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "movies.h"

void sorting_menu() {
	int initial_input = 0;
	int status = 0;
	const int OPTION_COUNT = 7;

	if (loaded_movies == NULL || loaded_movies->next == NULL) {
		printf("\nYou do not have enough movies to sort.\n\n");
		return;
	}

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

	if (initial_input > 0 && initial_input < OPTION_COUNT) {
		sort_movies(initial_input);
	}
	else if (initial_input == OPTION_COUNT) {
		return;
	}
}

void sort_movies(int sorting_type) {
	movie_t *new_list = NULL;
	movie_t *get_movie = NULL;
	movie_t *nav = loaded_movies;
	loaded_movies = NULL;

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

	printf("\nSorting complete!\n");
	loaded_movies = new_list;
}

movie_t *insert_movie(movie_t *sorted_list, movie_t *insert, int sorting_type) {
	movie_t *nav = sorted_list;
	char *insert_val = NULL;
	char *nav_val = NULL;
	char *nav_next_val = NULL;
	float insert_num = -1.0;
	float nav_num = -1.0;
	float nav_next_num = -1.0;

	// If the sorted_list is currently empty
	if (sorted_list == NULL) {
		sorted_list = insert;
		return sorted_list;
	}

	// Determine the values to compare
	if (sorting_type == 1) {
		insert_val = (char *) insert->name;
		nav_val = (char *) nav->name;
	}
	else if (sorting_type == 2) {
		insert_num = (float) insert->rating;
		nav_num = (float) nav->rating;
	}

	// If the insert node's value is less than that of the head
	if ((insert_num != -1.0 && (insert_num > nav_num)) ||
		(insert_val != NULL && (strcmp(insert_val, nav_val) < 0))) {
		insert->next = sorted_list;
		insert->prev = NULL;
		sorted_list->prev = insert;
		sorted_list = insert;
		return sorted_list;
	}
	// If the sorted_list only has one node and the insert node must be placed after it
	else if (sorted_list->next == NULL) {
		sorted_list->next = insert;
		insert->next = NULL;
		insert->prev = sorted_list;
		return sorted_list;
	}
	// If there are multiple nodes in sorted_list
	else {
		while (nav->next != NULL) {
			if (sorting_type == 1) {
				insert_val = (char *) insert->name;
				nav_val = (char *) nav->name;
				nav_next_val = (char *) nav->next->name;
			}
			else if (sorting_type == 2) {
				insert_num = (float) insert->rating;
				nav_num = (float) nav->rating;
				nav_next_num = (float) nav->next->rating;
			}

			if ((insert_num != -1.0 && (insert_num < nav_num && nav_next_num < insert_num)) ||
					(insert_val != NULL && (strcmp(insert_val, nav_val) > 0 &&
					strcmp(nav_next_val, insert_val) > 0))) {
				insert->next = nav->next;
				nav->next = insert;
				insert->prev = nav;
				if (insert->next != NULL) {
					insert->next->prev = insert;
				}
				return sorted_list;
			}
			nav = nav->next;
		}
	}

	nav->next = insert;
	return sorted_list;
}
