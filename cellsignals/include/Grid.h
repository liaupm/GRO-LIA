#include "GCell.h"
#include <vector>

using namespace std;

class Grid {
 private:
  unsigned int len;
  std::vector<std::vector<GCell*> > signalGrid;
  
 public:
  Grid(unsigned int len, unsigned int cellSize);
  Grid();
  ~Grid();
  unsigned int getLen();
  unsigned int getCellSize();
  float getCentre();
  GCell* getCell(unsigned int row, unsigned int col);
  std::vector<GCell*> create_and_fill_vector(unsigned int len, unsigned int cellSize);
  void resize(unsigned int nCells);
};
