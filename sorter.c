#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "movies.h"

void sorting_menu() {
	int initial_input = 0;
	int status = 0;
	const int OPTION_COUNT = 6;
	int ascending = 0;

	// Check to make sure there are movies to sort
	if (loaded_movies == NULL || loaded_movies->next == NULL) {
		printf("\nYou do not have enough movies to sort.\n\n");
		return;
	}

	printf("Please choose a sorting option for your movies.\n\n");

	// Display sorting options
	while (initial_input < 1 || initial_input > OPTION_COUNT) {
		printf("1. By name (alphabetically)\n");
		printf("2. By rating\n");
		printf("3. By runtime\n");
		printf("4. By date watched\n");
		printf("5. By actor count\n");
		printf("6. Exit\n\n");

		status = scanf("%d", &initial_input);
		while (getchar() != '\n');
		if (status < 1 || initial_input < 1 || initial_input > OPTION_COUNT) {
			printf("Incorrect input. Please enter a number from 1 to %d.\n\n", OPTION_COUNT);
#ifdef DEBUG
			printf("initial_input is %d\n", initial_input);
#endif
			clearerr(stdin);
		}
	}

	// Display ascending/descending options
	while (ascending < 1 || ascending > 2) {
		printf("Sort in ascending or descending order?\n\n");
		printf("1. Ascending (least/lowest first)\n");
		printf("2. Descending (greatest/largest first)\n\n");

		status = scanf("%d", &ascending);
		while (getchar() != '\n');
		if (status < 1 || ascending < 1 || ascending > 2) {
			printf("Incorrect input. Please enter either '1' or '2'.\n\n");
#ifdef DEBUG
			printf("ascending is %d\n", ascending);
#endif
			clearerr(stdin);
		}
	}

	// Call sort_movies() if arguments are correct
	if (initial_input > 0 && initial_input < OPTION_COUNT) {
		sort_movies(initial_input, ascending);
	}
	else if (initial_input == OPTION_COUNT) {
		return;
	}
}

void sort_movies(int sorting_type, int ascending) {
	movie_t *new_list = NULL;
	movie_t *get_movie = NULL;
	movie_t *nav = loaded_movies;
	loaded_movies = NULL;

	// Go through the remaining nodes in the original list
	while (nav != NULL) {
		// Isolate the first node of the list
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
		
		// Insert the isolated node into the new list
		new_list = insert_movie(new_list, get_movie, sorting_type, ascending);
		get_movie = NULL;
	}

	printf("\nSorting complete!\n");
	loaded_movies = new_list;
}

int custom_strcmp(void *char1, void *char2, int ascending) {
	int result = strcmp((char *) char1, (char *) char2);
	if (ascending == 2) result = result * -1;
	return result;
}

int custom_numcmp(void *num1, void *num2, int ascending) {
	int result = 0;
	if (*((float *) num1) < *((float *) num2)) result = -1;
	else if (*((float *) num1) > *((float *) num2)) result = 1;
	else result = 0;

	if (ascending == 2) result = result * -1;
	return result;
}

movie_t *insert_movie(movie_t *sorted_list, movie_t *insert, int sorting_type, int ascending) {
	movie_t *nav = sorted_list;
	void *insert_val = NULL;
	void *nav_val = NULL;
	void *nav_next_val = NULL;
	int (*compare_func)(void *, void *, int);
	int insert_date = 0;
	int nav_date = 0;
	int nav_next_date = 0;

	// If the sorted_list is currently empty
	if (sorted_list == NULL) {
		sorted_list = insert;
		return sorted_list;
	}

	/* Determine the elements to compare and the
	   function to be stored in compare_func
	*/
	if (sorting_type == 1) {
		insert_val = (void *) insert->name;
		nav_val = (void *) nav->name;
		compare_func = custom_strcmp;
	}
	else if (sorting_type == 2) {
		insert_val = (void *) &(insert->rating);
		nav_val = (void *) &(nav->rating);
		compare_func = custom_numcmp;
	}
	else if (sorting_type == 3) {
		insert_val = (void *) &(insert->runtime);
		nav_val = (void *) &(nav->runtime);
		compare_func = custom_numcmp;
	}
	else if (sorting_type == 4) {
		insert_date = 365 * insert->date_watched.year_watched;
		insert_date += 31 * insert->date_watched.month_watched;
		insert_date += insert->date_watched.day_watched;
		nav_date = 365 * nav->date_watched.year_watched;
		nav_date += 31 * nav->date_watched.month_watched;
		nav_date += nav->date_watched.day_watched;

		insert_val = (void *) &insert_date;
		nav_val = (void *) &nav_date;
		compare_func = custom_numcmp;
	} else if (sorting_type == 5) {
		insert_val = (void *) &(insert->actor_count);
		nav_val = (void *) &(nav->actor_count);
		compare_func = custom_numcmp;
	}

	// If the insert node's value is less than that of the head
	if (compare_func(insert_val, nav_val, ascending) <= 0) {
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
			// Get the right elements to compare
			if (sorting_type == 1) {
				insert_val = (void *) insert->name;
				nav_val = (void *) nav->name;
				nav_next_val = (void *) nav->next->name;
			}
			else if (sorting_type == 2) {
				insert_val = (void *) &(insert->rating);
				nav_val = (void *) &(nav->rating);
				nav_next_val = (void *) &(nav->next->rating);
			}
			else if (sorting_type == 3) {
				insert_val = (void *) &(insert->runtime);
				nav_val = (void *) &(nav->runtime);
				nav_next_val = (void *) &(nav->next->runtime);
			}
			else if (sorting_type == 4) {
				insert_date = 365 * insert->date_watched.year_watched;
				insert_date += 31 * insert->date_watched.month_watched;
				insert_date += insert->date_watched.day_watched;
				nav_date = 365 * nav->date_watched.year_watched;
				nav_date += 31 * nav->date_watched.month_watched;
				nav_date += nav->date_watched.day_watched;
				nav_next_date = 365 * nav->next->date_watched.year_watched;
				nav_next_date += 31 * nav->next->date_watched.month_watched;
				nav_next_date += nav->next->date_watched.day_watched;

				insert_val = (void *) &insert_date;
				nav_val = (void *) &nav_date;
				nav_next_val = (void *) &nav_next_date;
			}
			else if (sorting_type == 5) {
				insert_val = (void *) &(insert->actor_count);
				nav_val = (void *) &(nav->actor_count);
				nav_next_val = (void *) &(nav->next->actor_count);
			}

			// Insert the node into the correct location
			if (compare_func(insert_val, nav_val, ascending) > 0 &&
					compare_func(nav_next_val, insert_val, ascending) >= 0) {
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

	/* If the node to insert has not yet been inserted,
	   it must be inserted at the tail
	*/
	nav->next = insert;
	insert->prev = nav;
	insert->next = NULL;
	return sorted_list;
}
