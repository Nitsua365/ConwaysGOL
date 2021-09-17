

#include <SDL.h>
#include <stdio.h>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <utility>
#include <iostream>
#include "cellFilter.h"

//Screen dimension constants
const short SCREEN_WIDTH = 1280;
const short SCREEN_HEIGHT = 720;
const short CELL_DIMENSION = 5;
const short FILTER_WIDTH = (SCREEN_WIDTH / CELL_DIMENSION);
const short FILTER_HEIGHT = (SCREEN_HEIGHT / CELL_DIMENSION);


std::pair<short, short> filterToPixelLoc(short i, short j) {
	return std::make_pair((i * CELL_DIMENSION), (j * CELL_DIMENSION));
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

	// initialize cellFilter 
	CellFilter mainFilter(FILTER_WIDTH, FILTER_HEIGHT);
	mainFilter.initializeCellFilter();


	SDL_Event e;

	while (isRunning) {

		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT)
				isRunning = false;
		}

		// clear render panel
		clearPanel(cell, renderer);

		// clear the output filter
		mainFilter.clearOutputFilter();

		// set the draw color
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);

		// Conway Logic
		for (int i = 0; i < mainFilter.getX(); i++) {
			for (int j = 0; j < mainFilter.getY(); j++) {
				short neighbors = mainFilter.countNeighbors(i, j);

				if (neighbors == 2 && mainFilter.getCellState(i, j))
					mainFilter.setOutputState(i, j, true);
				else if (neighbors == 3)
					mainFilter.setOutputState(i, j, true);
				else
					mainFilter.setOutputState(i, j, false);
				
				if (mainFilter.getOutputState(i, j)) {
					std::pair<short, short> coord = filterToPixelLoc(i, j);
					cell = SDL_Rect{ coord.first, coord.second, CELL_DIMENSION, CELL_DIMENSION };
					SDL_RenderFillRect(renderer, &cell);
				}
			}
		}
		
		// copy the output Filter to the cell filter
		mainFilter.updateCellFilterFromOutputFilter();

		// present the frame
		SDL_RenderPresent(renderer);
	}

	// Destroy window
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow( window );
	SDL_Quit();

	return 0;
}