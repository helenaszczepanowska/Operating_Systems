#define _XOPEN_SOURCE 700
#define _DEFAULT_SOURCE

#include <ncurses.h>
#include <locale.h>
#include <unistd.h>
#include <stdbool.h>
#include "grid.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <math.h>
#include <signal.h>

typedef struct {
	int cell_start; 
	int cell_end; 
	char** background; 
	char** foreground; 
} thread_args_t;

void handler(int signo) {;}

void* thread_function(void* arg) {
	
	thread_args_t* args = (thread_args_t*)arg;
	
	while (true) {
		pause();

		for (int i = args->cell_start; i < args->cell_end; i++) {
			int row = i / GRID_HEIGHT;
			int col = i % GRID_WIDTH; 

			(*args->background)[i] = is_alive(row, col, *args->foreground);
		}
	}
}

int main(int argc, char **argv)
{
    if(argc != 2) {
        printf("Usage: %s <num_threads>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int n = atoi(argv[1]);
    signal(SIGUSR1, handler);

	srand(time(NULL));
	setlocale(LC_CTYPE, "");
	initscr(); 

	char *foreground = create_grid();
	char *background = create_grid();
	char *tmp;

	pthread_t threads[n];
	thread_args_t args[n];

	int cells_per_thread = (int)ceil(GRID_HEIGHT * GRID_WIDTH / n);
	for (int i = 0; i < n; i++) {
		
		args[i].cell_start = i * cells_per_thread;
		args[i].cell_end = min((i + 1) * cells_per_thread, GRID_HEIGHT * GRID_WIDTH); 
		args[i].foreground = &foreground;
		args[i].background = &background;

		pthread_create(&threads[i], NULL, thread_function, &args[i]);
	}

	init_grid(foreground);
 
	while (true)
	{
		draw_grid(foreground);

		for(int i = 0; i < n; i++) {
			pthread_kill(threads[i], SIGUSR1);
		}

		usleep(500 * 1000);
		update_grid(foreground, background);
		
		tmp = foreground;
		foreground = background;
		background = tmp;
	}

	endwin();
	destroy_grid(foreground);
	destroy_grid(background);

	return 0;
}