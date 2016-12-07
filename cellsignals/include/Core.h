#include "GSignal.h"
#include "Grid.h"
#include <algorithm>
#include <map>
#include <stdlib.h>
#include <time.h>

#define CELLS_BY_CONC 10
#define DIFF_THRESHOLD 0.1

using namespace std;

class Core {
  
  //Structures required
  struct rp_t
  {
    float x;
    float y;
    float lengthX;
    float lengthY;
  };

  struct vp_t
  {
    float x;
    float y;
  };

  struct sector_t {
    unsigned int row;
    unsigned int col;
  };

  struct interval_t {
    sector_t top;
    sector_t bottom;
  };
  
 public:
  #if __cplusplus > 199711L
  Grid* grid = nullptr;
  #else
  Grid* grid;
  #endif
  std::map<int,GSignal> sig;

  //Constructors
  Core(unsigned int len=10, unsigned int cellSize=30);
  
  //Auxiliar functions
  sector_t _getSector(vp_t virtualP);
  vp_t _getVirtual(rp_t realP);
  vp_t _getTopV(rp_t point);
  sector_t _getTop(rp_t point);
  vp_t _getBottomV(rp_t point);
  sector_t _getBottom(rp_t point);
  interval_t _getInterval(sector_t top, sector_t bottom);
  interval_t _getInterval(rp_t point);
  unsigned int _getCellsInter(interval_t inter);
  bool _outOfBounds(vp_t vpoint);
  unsigned int _maxCellsToResize(vp_t point, vp_t top, vp_t bottom);
  unsigned int _cellsToResize(vp_t vpoint);
  bool _existSignal(int id);
  bool _signalReachBorder();
  float _cellsToResizeBySignals();
  unsigned int _cellsToResizeByConcentration();
  double _signalC(int id, unsigned int row, unsigned int col);
  GCell* getSignalGrid(double x, double y);
  
  // Functions - Signals
  void add_signal(int id, double kdiff, double kdeg);
  void add_signal(int id, double kdiff, double kdeg, double concentration, double coords[]);
  void emit_signal(int id, double concentration, double coords[]);
  void emit_signal_average(int id, double concentration, double coords[]);
  void absorb_signal(int id, double concentration, double coords[]);
  void absorb_signal_random(int id, double concentration, double coords[]);
  double getSignalValue(int id, double coords[]);
  double getSignalValueRange(int id, double coords[]);
  void setSignalValue(int id, double concentration, double coords[]);

  // Feedback functions
  void update(float dt);
};
