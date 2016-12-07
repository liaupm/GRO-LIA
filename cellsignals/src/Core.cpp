#include "../include/Core.h"
#include <cmath>

using namespace std;

// Constructor
Core::Core(unsigned int len, unsigned int cellSize) {
    grid = new Grid(len, cellSize);
    std::map<int,GSignal> newMap;
    sig = newMap;
}

//Auxiliar functions
Core::vp_t Core::_getVirtual(Core::rp_t realP) {
    Core::rp_t centre;
    centre.x = centre.y = this->grid->getCentre();

    Core::vp_t virtualP;
    virtualP.x = centre.x+realP.x;
    virtualP.y = centre.y+realP.y;

    return virtualP;
}

Core::sector_t Core::_getSector(Core::vp_t virtualP) {
    Core::sector_t sector;
    unsigned int size = this->grid->getCellSize();
    sector.col = (unsigned int)floor(virtualP.x/size);
    sector.row = (unsigned int)floor(virtualP.y/size);

    return sector;
}

Core::vp_t Core::_getTopV(Core::rp_t point) {
    Core::rp_t rTop;
    rTop.x = point.x - point.lengthX;
    rTop.y = point.y - point.lengthY;
    rTop.lengthX = rTop.lengthY = 0.0;

    Core::vp_t vTop = _getVirtual(rTop);

    return vTop;
}

Core::sector_t Core::_getTop(Core::rp_t point) {
    Core::rp_t rTop;
    rTop.x = point.x - point.lengthX;
    rTop.y = point.y - point.lengthY;
    rTop.lengthX = rTop.lengthY = 0.0;

    Core::vp_t vTop = _getVirtual(rTop);
    Core::sector_t sector = _getSector(vTop);

    return sector;
}

Core::vp_t Core::_getBottomV(Core::rp_t point) {
    Core::rp_t rBottom;
    rBottom.x = point.x + point.lengthX;
    rBottom.y = point.y + point.lengthY;
    rBottom.lengthX = rBottom.lengthY = 0.0;

    Core::vp_t vBottom = _getVirtual(rBottom);

    return vBottom;
}

Core::sector_t Core::_getBottom(Core::rp_t point) {
    Core::rp_t rBottom;
    rBottom.x = point.x + point.lengthX;
    rBottom.y = point.y + point.lengthY;
    rBottom.lengthX = rBottom.lengthY = 0.0;

    Core::vp_t vBottom = _getVirtual(rBottom);
    Core::sector_t sector = _getSector(vBottom);

    return sector;
}

Core::interval_t Core::_getInterval(Core::rp_t point) {
    Core::sector_t top = _getTop(point);
    Core::sector_t bottom = _getBottom(point);

    Core::interval_t inter; inter.top = top; inter.bottom = bottom;

    return inter;
}

unsigned int Core::_getCellsInter(Core::interval_t inter) {
    unsigned int diffRows = inter.bottom.row - inter.top.row;
    unsigned int diffCols = inter.bottom.col - inter.top.col;

    return (diffRows*diffCols);
}

bool Core::_outOfBounds(Core::vp_t vpoint) {
    return (vpoint.x <= this->grid->getCellSize() || vpoint.x >= ((this->grid->getLen()-1)*this->grid->getCellSize()) ||
      vpoint.y <= this->grid->getCellSize() || vpoint.y >= ((this->grid->getLen()-1)*this->grid->getCellSize()));
}

unsigned int Core::_cellsToResize(Core::vp_t vpoint) {
    double max_dist_x, max_dist_y;

    if (vpoint.x <= 0) {
        max_dist_x = -vpoint.x;
    } else if (vpoint.x >= this->grid->getLen()) {
        max_dist_x = vpoint.x-(this->grid->getLen()*this->grid->getCellSize());
    } else {
        max_dist_x = 0;
    }

    if (vpoint.y <= 0) {
        max_dist_y = -vpoint.y;
    } else if (vpoint.y >= this->grid->getLen()) {
        max_dist_y = vpoint.y-this->grid->getLen();
    } else {
        max_dist_y = 0;
    }

    double max = std::max(max_dist_x, max_dist_y);
    return floor(max/(this->grid->getCellSize()))+1;
}

unsigned int Core::_maxCellsToResize(Core::vp_t point, Core::vp_t top, Core::vp_t bottom) {
    unsigned int cellsP, cellsT, cellsB;
    cellsP = _cellsToResize(point);
    cellsT = _cellsToResize(top);
    cellsB = _cellsToResize(bottom);
    unsigned int max = 0;

    if ((cellsP >= cellsT) && (cellsP >= cellsB)) {
        max = cellsP;
    }
    if ((cellsT >= cellsP) && (cellsT >= cellsB)) {
        max = cellsT;
    }
    if ((cellsB >= cellsP) && (cellsB >= cellsT)) {
        max = cellsB;
    }

    return max;
}

bool Core::_existSignal(int id) {
    return (this->sig.find(id) != this->sig.end());
}

bool Core::_signalReachBorder() {
// Look for signals in first and last row
    for (unsigned int col = 1; col < this->grid->getLen()-1; col++) {
        GCell* cellF = this->grid->getCell(1, col);
        GCell* cellL = this->grid->getCell(this->grid->getLen()-2, col);

        if ((!cellF->getSignalsMap().empty()) || (!cellL->getSignalsMap().empty()))
        {
            for(auto& it:cellF->getSignalsMap())
            {
                //Cambiar a valor de threshold BAJO para evitar resizeos innecesarios por difusion.
                if(it.second > DIFF_THRESHOLD)
                return true;
            }
            for(auto& it:cellL->getSignalsMap())
            {
                //Cambiar a valor de threshold BAJO para evitar resizeos innecesarios por difusion.
                if(it.second > DIFF_THRESHOLD)
                return true;
            }
        }
    }

    // Look for signals in first and last col
    for (unsigned int row = 1; row < this->grid->getLen()-1; row++) {
        GCell* cellF = this->grid->getCell(row, 1);
        GCell* cellL = this->grid->getCell(row, this->grid->getLen()-2);
        if ((!cellF->getSignalsMap().empty()) || (!cellL->getSignalsMap().empty()))
        {
            for(auto& it:cellF->getSignalsMap())
            {
               if(it.second > DIFF_THRESHOLD)
                   return true;
            }
            for(auto& it:cellL->getSignalsMap())
            {
               if(it.second > DIFF_THRESHOLD)
                   return true;
            }
        }
    }
    return false;
}

float Core::_cellsToResizeBySignals() {
    float factor = ((this->grid->getLen()*this->grid->getCellSize())/this->grid->getCellSize());
    return (factor/this->grid->getCellSize());
}

unsigned int Core::_cellsToResizeByConcentration() {
    double maxC = 0.0;
    // Look for signals in first row
    for (unsigned int col = 1; col < this->grid->getLen()-1; col++) {
        GCell* cell = this->grid->getCell(1, col);
        if (!cell->getSignalsMap().empty()) {
            //Explore the signals map of each cell
            for (auto x : cell->getSignalsMap()) {
                if (x.second > maxC)
                  maxC = x.second;
            }
        }
    }

    // Look for signals in last row
    for (unsigned int col = 1; col < this->grid->getLen()-1; col++) {
        GCell* cell = this->grid->getCell(this->grid->getLen()-2, col);
        //GCell* cell = this->grid[this->grid->getLen()-2][col];
        if (!cell->getSignalsMap().empty()) {
            //Explore the signals map of each cell
            for (auto x : cell->getSignalsMap()) {
                if (x.second > maxC)
                  maxC = x.second;
            }
        }
    }

    // Look for signals in first col
    for (unsigned int row = 1; row < this->grid->getLen()-1; row++) {
        GCell* cell = this->grid->getCell(row, 1);
        //GCell* cell = this->grid[row][1];
        if (!cell->getSignalsMap().empty()) {
            //Explore the signals map of each cell
            for (auto x : cell->getSignalsMap()) {
                if (x.second > maxC)
                  maxC = x.second;
            }
        }
    }

    // Look for signals in last col
    for (unsigned int row = 1; row < this->grid->getLen()-1; row++) {
        GCell* cell = this->grid->getCell(row, this->grid->getLen()-2);
        //GCell* cell = this->grid[row][this->grid->getLen()-2];
        if (!cell->getSignalsMap().empty()) {
            //Explore the signals map of each cell
            for (auto x : cell->getSignalsMap()) {
                if (x.second > maxC)
                  maxC = x.second;
            }
        }
    }

    return ceil(maxC/CELLS_BY_CONC);
}

double Core::_signalC(int id, unsigned int row, unsigned int col) {
    return (this->grid->getCell(row,col)->getConcentration(id));
}

GCell* Core::getSignalGrid(double x, double y) {
    Core::rp_t point; point.x = x; point.y = y;
    Core::vp_t virtualP = _getVirtual(point);
    Core::sector_t sector = _getSector(virtualP);

    return this->grid->getCell(sector.row, sector.col);
}


// Functions - Signals //

// This function is more similar to a create function. It doesn't add the signal to a point.
void Core::add_signal(int id, double kdiff, double kdeg) {
    // It only creates the signal object and add it to the signal map
    GSignal newsignal(id, kdiff, kdeg);
    sig[id] = newsignal;
}


void Core::add_signal(int id, double kdiff, double kdeg, double concentration, double coords[4]) {
    // Create the rp_t that represents the point
    Core::rp_t point; point.x = coords[0]; point.y = coords[1];
    point.lengthX = coords[2]; point.lengthY = coords[3];

    // Check if it is neccessary to resize the grid
    Core::vp_t virtualP = _getVirtual(point);
    Core::vp_t vtop = _getTopV(point);
    Core::vp_t vbottom = _getBottomV(point);

    if (_outOfBounds(virtualP) || _outOfBounds(vtop) || _outOfBounds(vbottom)) {
        unsigned int ncells = _maxCellsToResize(virtualP, vtop, vbottom);
        this->grid->resize(ncells);
    }

    // Add a new signal
    Core::interval_t inter = _getInterval(point);
    GSignal newsignal(id, kdiff, kdeg);
    sig[id] = newsignal;

    for (unsigned int i = inter.top.row; i <= inter.bottom.row; i++) {
        for (unsigned int j = inter.top.col; j <= inter.bottom.col; j++) {
            this->grid->getCell(i,j)->insertSignal(id, concentration);
        }
    }
}

void Core::emit_signal(int id, double concentration, double coords[4]) {
    // Create the rp_t that represents the point
    Core::rp_t point; point.x = coords[0]; point.y = coords[1];
    point.lengthX = coords[2]; point.lengthY = coords[3];

    // Check if the emit point is on the Grid. If there is no a cell in that position don't do anything
    Core::vp_t virtualP = _getVirtual(point);
    Core::vp_t vtop = _getTopV(point);
    Core::vp_t vbottom = _getBottomV(point);

    if (_outOfBounds(virtualP) || _outOfBounds(vtop) || _outOfBounds(vbottom)) {
        //return;
        unsigned int ncells = _maxCellsToResize(virtualP, vtop, vbottom);
        this->grid->resize(ncells);
    }

    //Check of signal exist into the vector
    if (_existSignal(id)) {
        Core::interval_t inter = _getInterval(point);

        for (unsigned int i = inter.top.row; i <= (inter.bottom.row); i++) {
            for (unsigned int j = inter.top.col; j <= (inter.bottom.col); j++) {
                this->grid->getCell(i,j)->incSignal(id, concentration);
            }
        }
    }
}

void Core::emit_signal_average(int id, double concentration, double coords[4]) {
    // Create the rp_t that represents the point
    Core::rp_t point; point.x = coords[0]; point.y = coords[1];
    point.lengthX = coords[2]; point.lengthY = coords[3];

    // Check if the emit point is on the Grid. If there is no a cell in that position don't do anything
    Core::vp_t virtualP = _getVirtual(point);
    Core::vp_t vtop = _getTopV(point);
    Core::vp_t vbottom = _getBottomV(point);

    if (_outOfBounds(virtualP) || _outOfBounds(vtop) || _outOfBounds(vbottom)) {
        //return;
        unsigned int ncells = _maxCellsToResize(virtualP, vtop, vbottom);
        this->grid->resize(ncells);
    }

    // Check of signals exists in the environment
    if (_existSignal(id)) {
        Core::interval_t inter = _getInterval(point);
        double averageC = concentration/_getCellsInter(inter);

        for (unsigned int i = inter.top.row; i <= (inter.bottom.row); i++) {
            for (unsigned int j = inter.top.col; j <= (inter.bottom.col); j++) {
                //printf("Emit to {%d,%d}\n", i, j);
                this->grid->getCell(i,j)->incSignal(id, averageC);
            }
        }
    }
}

void Core::absorb_signal(int id, double concentration, double coords[4]) {
    // Create the rp_t that represents the point
    Core::rp_t point; point.x = coords[0]; point.y = coords[1];
    point.lengthX = coords[2]; point.lengthY = coords[3];

    // Check if the emit point is on the Grid. If there is no a cell in that position don't do anything
    Core::vp_t virtualP = _getVirtual(point);
    Core::vp_t vtop = _getTopV(point);
    Core::vp_t vbottom = _getBottomV(point);

    if (_outOfBounds(virtualP) || _outOfBounds(vtop) || _outOfBounds(vbottom)) {
        return;
    }

    if (_existSignal(id)) {
        Core::interval_t inter = _getInterval(point);

        for (unsigned int i = inter.top.row; i <= (inter.bottom.row); i++) {
            for (unsigned int j = inter.top.col; j <= (inter.bottom.col); j++) {
                double available = this->grid->getCell(i,j)->getConcentration(id);
                if ((available - concentration) > 0) {
                  this->grid->getCell(i,j)->decSignal(id, concentration);
                } else if (available > 0) {
                  this->grid->getCell(i,j)->decSignal(id, available);
                }
            }
        }
    }
}

void Core::absorb_signal_random(int id, double concentration, double coords[4]) {
    // Create the rp_t that represents the point
    Core::rp_t point; point.x = coords[0]; point.y = coords[1];
    point.lengthX = coords[2]; point.lengthY = coords[3];

    // Check if the emit point is on the Grid. If there is no a cell in that position don't do anything
    Core::vp_t virtualP = _getVirtual(point);
    Core::vp_t vtop = _getTopV(point);
    Core::vp_t vbottom = _getBottomV(point);

    if (_outOfBounds(virtualP) || _outOfBounds(vtop) || _outOfBounds(vbottom)) {
        return;
    }

    if (_existSignal(id)) {
        Core::interval_t inter = _getInterval(point);
        // Generate random row and col
        srand((unsigned int)time(NULL));
        int rRow = rand() % ((inter.bottom.row+1) - inter.top.row) + inter.top.row;
        int rCol = rand() % ((inter.bottom.col+1) - inter.top.col) + inter.top.col;

        printf("Absorb from {%d,%d}\n", rRow, rCol);
        double available = this->grid->getCell(rRow,rCol)->getConcentration(id);
        if ((available - concentration) > 0) {
            this->grid->getCell(rRow,rCol)->decSignal(id, concentration);
        } else if (available > 0) {
            this->grid->getCell(rRow,rCol)->decSignal(id, available);
        }
    }
}

double Core::getSignalValue(int id, double coords[4]) {
    // Create the rp_t that represents the point
    Core::rp_t point; point.x = coords[0]; point.y = coords[1];
    point.lengthX = coords[2]; point.lengthY = coords[3];

    // Check if the emit point is on the Grid. If there is no a cell in that position returns -1 (error).
    Core::vp_t virtualP = _getVirtual(point);
    Core::vp_t vtop = _getTopV(point);
    Core::vp_t vbottom = _getBottomV(point);

    if (_outOfBounds(virtualP) || _outOfBounds(vtop) || _outOfBounds(vbottom)) {
        return -1;
    }
  
    //Check of signal exist into the vector
    if (_existSignal(id)) {
        Core::vp_t virtualP = _getVirtual(point);
        Core::sector_t sector = _getSector(virtualP);
        //if(this->grid->getSignalGrid(sector.row,sector.col)->_existSignal(id) && this->grid->getSignalGrid(sector.row,sector.col)->getConcentration(id) >= 0)
            // return this->grid->getSignalGrid(sector.row,sector.col)->getConcentration(id);
        return this->grid->getCell(sector.row,sector.col)->getConcentration(id);
        //else
        //    return -1;

        //printf("Concentration: %lf\n", this->grid->getSignalGrid(sector.row,sector.col)->getConcentration(id));
    }

  return -1;
}

/*double Core::getSignalValueRange(int id, double coords[4]) {
  // Create the rp_t that represents the point
  Core::rp_t point; point.x = coords[0]; point.y = coords[1];
  point.lengthX = coords[2]; point.lengthY = coords[3];

  // Check if the emit point is on the Grid. If there is no a cell in that position returns -1 (error).
  Core::vp_t virtualP = _getVirtual(point);
  Core::vp_t vtop = _getTopV(point);
  Core::vp_t vbottom = _getBottomV(point);

  if (_outOfBounds(virtualP) || _outOfBounds(vtop) || _outOfBounds(vbottom)) {
    return -1;
  }
  
  //Check of signal exist into the vector 
  if (_existSignal(id)) {
    Core::interval_t inter = _getInterval(point);
    
    double sum = 0.0;
    for (unsigned int i = (inter.top.row); i < (inter.bottom.row); ++i) {
        for (unsigned int j = (inter.top.col); j < (inter.bottom.col); ++j) {
            sum += this->grid->getCell(i,j)->getConcentration(id);
        }
    }
    
    return sum;
  }

  return -1;
}*/

double Core::getSignalValueRange(int id, double coords[4]) {
  // Create the rp_t that represents the point
  Core::rp_t point; point.x = coords[0]; point.y = coords[1];
  point.lengthX = coords[2]; point.lengthY = coords[3];

  // Check if the emit point is on the Grid. If there is no a cell in that position returns -1 (error).
  Core::vp_t virtualP = _getVirtual(point);
  Core::vp_t vtop = _getTopV(point);
  Core::vp_t vbottom = _getBottomV(point);

    if (_outOfBounds(virtualP)) {
    return -1;
  }

  //Check of signal exist into the vector
  if (_existSignal(id)) {

    //Core::interval_t inter = _getInterval(point);
    if(vtop.x < 0)
    {
        vtop.x = 0;
    }
    if(vbottom.x > (this->grid->getLen()*this->grid->getCellSize()))
    {
        vbottom.x = this->grid->getLen()*this->grid->getCellSize();
    }
    if(vtop.y < 0)
    {
        vtop.y = 0;
    }
    if(vbottom.y > (this->grid->getLen()*this->grid->getCellSize()))
    {
        vbottom.y = this->grid->getLen()*this->grid->getCellSize();
    }

    Core::interval_t inter;
    inter.top = _getSector(vtop);
    inter.bottom = _getSector(vbottom);

    double sum = 0.0;
    for (unsigned int i = (inter.top.row); i < (inter.bottom.row); ++i) {
        for (unsigned int j = (inter.top.col); j < (inter.bottom.col); ++j) {
            sum += this->grid->getCell(i,j)->getConcentration(id);
        }
    }

    return sum;
  }

  return -1;
}

void Core::setSignalValue(int id, double concentration, double coords[4]) {
  // Create the rp_t that represents the point
  Core::rp_t point; point.x = coords[0]; point.y = coords[1];
  point.lengthX = coords[2]; point.lengthY = coords[3];

  // Check if the emit point is on the Grid. If there is no a cell in that position don't do anything
  Core::vp_t virtualP = _getVirtual(point);
  Core::vp_t vtop = _getTopV(point);
  Core::vp_t vbottom = _getBottomV(point);

  if (_outOfBounds(virtualP) || _outOfBounds(vtop) || _outOfBounds(vbottom)) {
    return;
  }
  
  //Check of signal exist into the vector 
  if (_existSignal(id)) {
    Core::vp_t virtualP = _getVirtual(point);
    Core::sector_t sector = _getSector(virtualP);

    // return this->grid->getSignalGrid(sector.row,sector.col)->setConcentration(id, concentration);
    return this->grid->getCell(sector.row,sector.col)->setConcentration(id, concentration);
  }
}

void Core::update(float dt) {
  //Check if grid exists. If it doesn't exist, create a default grid.
  if (this->grid == nullptr) {
    this->grid = new Grid();
  }
  
  // Check if it's necessary to resize if signals reach the border
  if (_signalReachBorder()) {
    this->grid->resize(ceil(_cellsToResizeBySignals()/2));
    //this->grid->resize(ceil(((float)(this->grid->getLen()))/2)); // Double the size
  }
  
  //Update signal values
  //Inicializar aux a 0? ojo con el problema de las H
  std::map<int,GSignal>::iterator it_signal;
  double newConcentration = 0.0;
  for (it_signal = this->sig.begin(); it_signal != this->sig.end(); it_signal++) {
    GSignal s = it_signal->second;
    int id = s.getId();
    for (unsigned int row = 1; row < this->grid->getLen()-1; row++) {
      for (unsigned int col = 1; col < this->grid->getLen()-1; col++) {
        newConcentration = -6*(s.get_kdiff())*_signalC(id,row,col) - s.get_kdeg()*_signalC(id,row,col);
        newConcentration += s.get_kdiff() * (
                        0.5*_signalC(id,row+1,col-1) + _signalC(id,row+1,col) + 0.5*_signalC(id,row+1,col+1)
                        + _signalC(id,row,col-1) + _signalC(id,row,col+1)
                        + 0.5*_signalC(id,row-1,col-1) + _signalC(id,row-1,col) + 0.5*_signalC(id,row-1,col+1));

        newConcentration = _signalC(id,row,col) + dt*newConcentration;
     	this->grid->getCell(row,col)->setConcentration(id, newConcentration); 
     }
    }
  }
}
