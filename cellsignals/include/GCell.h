#include <map>

class GCell {
 private:
  unsigned int size;
  std::map<int,double> signls;

 public:
  // Constructor
  GCell() { signls = std::map<int,double>(); }
 GCell(unsigned int pixels) :size(pixels) { signls = std::map<int,double>(); }
 GCell(unsigned int pixels, std::map<int,double> signalMap) : size(pixels), signls(signalMap) {}

  // Functions interfaces
  std::map<int,double> getSignalsMap();
  bool _existSignal(int id);
  unsigned int getSize();
  void setSize(unsigned int newSize);
  double getConcentration(int signal_id);
  void setConcentration(int signal_id, double concentration);
  void insertSignal(int signal_id, double signal_concentration);
  void deleteSignal(int signal_id);
  void incSignal(int signal_id, double concentration);
  void decSignal(int signal_id, double concentration);
};
