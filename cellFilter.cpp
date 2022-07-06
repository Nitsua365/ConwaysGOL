#include "cellFilter.h"

CellFilter::CellFilter(int x, int y) {
    this->x = x;
    this->y = y;
    this->filter = new bool* [x];
    this->output = new bool* [x];
    for (int i = 0; i < x; i++) {
        this->filter[i] = new bool[y];
        this->output[i] = new bool[y];
    }
}

CellFilter::~CellFilter() {
    for (int i = 0; i < this->x; i++) {
        delete[] this->filter[i];
        delete[] this->output[i];
    }
    delete[] this->filter;
    delete[] this->output;
}

void CellFilter::initializeCellFilter() {
    srand(time(0));
    for (int i = 0; i < this->x; i++) {
        for (int j = 0; j < this->y; j++) {
            short random = rand() % 1000;
            filter[i][j] = (random < 500);
        }
    }
}

void CellFilter::clearOutputFilter() {
    for (int i = 0; i < this->x; i++) {
        for (int j = 0; j < this->y; j++) {
            output[i][j] = false;
        }
    }
}

short CellFilter::countNeighbors(int x, int y) {
    short count = 0;


    if ((y + 1) < this->y && this->filter[x][y + 1]) { count++; }
    if ((y - 1) >= 0 && this->filter[x][y - 1]) { count++; }

    if ((x - 1) >= 0) {
        if ((y - 1) >= 0 && this->filter[x - 1][y - 1]) { count++; }
        if ((y + 1) < this->y && this->filter[x - 1][y + 1]) { count++; }
        if (this->filter[x - 1][y]) { count++; }
    }

    if ((x + 1) < this->x) {
        if (this->filter[x + 1][y]) { count++; }
        if ((y - 1) >= 0 && this->filter[x + 1][y - 1]) { count++; }
        if ((y + 1) < this->y && this->filter[x + 1][y + 1]) { count++; }
    }




    return count;
}

const bool& CellFilter::getCellState(const int& x, const int& y) {
    return this->filter[x][y];
}

const bool& CellFilter::getOutputState(const int& x, const int& y) {
    return this->output[x][y];
}

void CellFilter::setOutputState(const int& x, const int& y, const bool& cellState) {
    this->output[x][y] = cellState;
}

void CellFilter::updateCellFilterFromOutputFilter() {
    for (int i = 0; i < this->x; i++) {
        for (int j = 0; j < this->y; j++) {
            this->filter[i][j] = this->output[i][j];
        }
    }
}