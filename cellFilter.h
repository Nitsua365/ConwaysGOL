#ifndef CELL_FILTER
#define CELL_FILTER

#include <ctime>
#include <cstdlib>

class CellFilter {
private:
    bool** filter;
    bool** output;
    int x, y;

public:
    CellFilter(int x, int y);
    ~CellFilter();

    
    void initializeCellFilter();
    void clearOutputFilter();

    const int& getX() const { return this->x;  }
    const int& getY() const { return this->y;  }

    short countNeighbors(int x, int y);

    const bool& getCellState(const int& x, const int& y);
    const bool& getOutputState(const int& x, const int& y);

    void setOutputState(const int& x, const int& y, const bool& cellState);

    void updateCellFilterFromOutputFilter();
};


#endif // !CELL_FILTER