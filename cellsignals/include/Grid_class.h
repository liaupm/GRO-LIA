#include <vector>
#include "Cell.h"

using namespace std;

class Grid {
 private:
  unsigned int nCells;
  vector<vector<Cell>> signalGrid;

 public:
  // Constructor
 Grid(unsigned int cells, vector<vector<Cell>> grid) : nCells(cells), signalGrid(grid) {};

  // Functions interfaces
  void create(unsigned int len);
  void destroy(Grid& grid);
  int resize(Grid& grid, unsigned int cells);
  
};
