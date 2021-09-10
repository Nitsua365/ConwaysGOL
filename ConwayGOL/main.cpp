

#include <SDL.h>
#include <stdio.h>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <utility>
#include <iostream>

//Screen dimension constants
const short SCREEN_WIDTH = 1280;
const short SCREEN_HEIGHT = 720;
const short CELL_DIMENSION = 5;
const short FILTER_WIDTH = (SCREEN_WIDTH / CELL_DIMENSION);
const short FILTER_HEIGHT = (SCREEN_HEIGHT / CELL_DIMENSION);

short countNeighbors(bool a[FILTER_WIDTH][FILTER_HEIGHT], int x, int y) {
	short count = 0;

	
	if ((y + 1) < FILTER_HEIGHT && a[x][y + 1]) { count++; }
	if ((y - 1) >= 0 && a[x][y - 1]) { count++; }

	if ((x - 1) >= 0) {
		if ((y - 1) >= 0 && a[x - 1][y - 1]) { count++; }
		if ((y + 1) < FILTER_HEIGHT && a[x - 1][y + 1]) { count++; }
		if (a[x - 1][y]) { count++; }
	}

	if ((x + 1) < FILTER_WIDTH) {
		if (a[x + 1][y]) { count++; }
		if ((y - 1) >= 0 && a[x + 1][y - 1]) { count++; }
		if ((y + 1) < FILTER_HEIGHT && a[x + 1][y + 1]) { count++; }
	}
	

	

	return count;
}



std::pair<short, short> filterToPixelLoc(short i, short j) {
	return std::make_pair((i * CELL_DIMENSION), (j * CELL_DIMENSION));
}

void initializeCellFilter(bool filter[FILTER_WIDTH][FILTER_HEIGHT]) {
	srand(time(0));
	for (int i = 0; i < FILTER_WIDTH; i++) {
		for (int j = 0; j < FILTER_HEIGHT; j++) {
			short random = rand() % 1000;
			filter[i][j] = (random < 500) ? 1 : 0;
		}
	}
}

void initializeOutputFilter(bool filter[FILTER_WIDTH][FILTER_HEIGHT]) {
	for (int i = 0; i < FILTER_WIDTH; i++) {
		for (int j = 0; j < FILTER_HEIGHT; j++) {
			filter[i][j] = 0;
		}
	}
}

void printFilter(bool filter[FILTER_WIDTH][FILTER_HEIGHT], std::ostream& out = std::cout) {
	for (int i = 0; i < FILTER_WIDTH; i++) {
		for (int j = 0; j < FILTER_HEIGHT; j++) {
			out << filter[i][j] << " ";
		}
		out << std::endl;
	}
}

void clearPanel(SDL_Rect& rect, SDL_Renderer* r) {
	
	SDL_RenderClear(r);
	SDL_SetRenderDrawColor(r, 0, 0, 0, SDL_ALPHA_OPAQUE);
	std::pair<short, short> p;

	for (int i = 0; i < FILTER_WIDTH; i++) {
		for (int j = 0; j < FILTER_HEIGHT; j++) {
			p = filterToPixelLoc(i, j);
			rect = SDL_Rect{p.first , p.second, CELL_DIMENSION, CELL_DIMENSION };
			SDL_RenderFillRect(r, &rect);
		}
	}
}

int main( int argc, char* argv[] ) {

	SDL_Window* window = NULL;

	SDL_Renderer* renderer = NULL;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return -1;
	}

	if (SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer) != 0) {
		printf("SDL Could not create window! SDL_Error: %s\n", SDL_GetError());
		return -1;
	}
	SDL_RenderClear(renderer);      // Clear the newly created window

	// initialize thread boolean and close event
	bool isRunning = true;

	SDL_Rect cell;

	// initialize frame counter and filter
	int frameCounter = 0;
	int neighbors = 0;

	bool cellFilter[FILTER_WIDTH][FILTER_HEIGHT];
	bool output[FILTER_WIDTH][FILTER_HEIGHT];

	// initialize cellFilter 
	initializeCellFilter(cellFilter);
	initializeOutputFilter(output);

	// printFilter(cellFilter);

	while (isRunning) {

		SDL_Event e;

		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT)
				isRunning = false;
		}

		clearPanel(cell, renderer);
		initializeOutputFilter(output);
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);

		for (int i = 0; i < FILTER_WIDTH; i++) {
			for (int j = 0; j < FILTER_HEIGHT; j++) {
				short neighbors = countNeighbors(cellFilter, i, j);

				if (neighbors == 2 && cellFilter[i][j])
					output[i][j] = true;
				else if (neighbors == 3)
					output[i][j] = true;
				else
					output[i][j] = false;
				
				if (output[i][j]) {
					std::pair<short, short> coord = filterToPixelLoc(i, j);
					cell = SDL_Rect{ coord.first, coord.second, CELL_DIMENSION, CELL_DIMENSION };
					SDL_RenderFillRect(renderer, &cell);
				}

			}
		}

		for (int i = 0; i < FILTER_WIDTH; i++) {
			for (int j = 0; j < FILTER_HEIGHT; j++) {
				cellFilter[i][j] = output[i][j];
			}
		}
		

		SDL_RenderPresent(renderer);
	}

	// Destroy window
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow( window );
	SDL_Quit();

	return 0;
}