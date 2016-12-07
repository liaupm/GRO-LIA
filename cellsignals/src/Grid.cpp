#include "../include/Grid.h"
#include <deque>

Grid::Grid(unsigned int len, unsigned int cellSize) :len(len){
  vector<vector<GCell*> > newSignalGrid(len, vector<GCell*>(len));

  for (int row = 0; row < (int)len; row++) {
    for (int col = 0; col < (int)len; col++) {
      std::map<int,double> newmap = std::map<int,double>();
      newSignalGrid[row][col] = new GCell(cellSize, newmap);
    }
  }

  signalGrid = newSignalGrid;
}

Grid::Grid() {
  len = 10;
  unsigned int cellSize = 30;
  vector<vector<GCell*> > newSignalGrid(len, vector<GCell*>(len));

  for (int row = 0; row < (int)len; row++) {
    for (int col = 0; col < (int)len; col++) {
      std::map<int,double> newmap = std::map<int,double>();
      newSignalGrid[row][col] = new GCell(cellSize, newmap);
    }
  }
  
  signalGrid = newSignalGrid;
}

Grid::~Grid() {
  for (int row = 0; row < (int)(this->len); row++) {
      this->signalGrid[row].clear();
      vector<GCell*>().swap(this->signalGrid[row]);
 } 
}

unsigned int Grid::getLen() {
  return this->len;
}

unsigned int Grid::getCellSize() {
  return this->signalGrid[0][0]->getSize();
}

GCell* Grid::getCell(unsigned int row, unsigned int col) {
  return this->signalGrid[row][col];
}

float Grid::getCentre() {
  return (this->len*this->getCellSize())/2.0;
   
}

vector<GCell*> Grid::create_and_fill_vector(unsigned int len, unsigned int cellSize) {
  vector<GCell*> newvector(len);

  for (int i = 0; i < (int)len; i++) {
      newvector[i] = new GCell(cellSize);
  }

  return newvector;
}

void Grid::resize(unsigned int nCells) {
  unsigned int cellSize = this->signalGrid[0][0]->getSize();

  for (int row = 0; row < (int)(this->len); row++) {
    for (int newcols = 0; newcols < (int)nCells; newcols++) {
      GCell* newcell_front = new GCell(cellSize); GCell* newcell_back = new GCell(cellSize);
      this->signalGrid[row].insert(this->signalGrid[row].begin(), newcell_front);
      this->signalGrid[row].push_back(newcell_back);
    }
  }

  for (int newcols = 0; newcols < (int)nCells; newcols++) {
    vector<GCell*> newdeque_front = create_and_fill_vector(this->len+(nCells*2), cellSize);
    vector<GCell*> newdeque_back = create_and_fill_vector(this->len+(nCells*2), cellSize);

    this->signalGrid.insert(this->signalGrid.begin(), newdeque_front);
    this->signalGrid.push_back(newdeque_back);
  }

  this->len = this->signalGrid.size();
}

// int main () {
//   Grid grid(5,20);

//   grid.resize(2);
//   for (unsigned int i = 0; i < grid.getLen(); i++) {
//     for (unsigned int j = 0; j < grid.getLen(); j++) {
//       printf("Cell (%d,%d) = %u\n", i, j, grid.getSignalGrid(i,j).getSize());
//     }
//   }
// }



