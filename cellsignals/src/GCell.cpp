#include <iostream>
#include <map>
#include "../include/GCell.h"

bool GCell::_existSignal(int id) {
  return (this->signls.find(id) != this->signls.end());
}

std::map<int,double> GCell::getSignalsMap() {
  return this->signls;
}

unsigned int GCell::getSize() {
  return this->size;
}

void GCell::setSize(unsigned int newSize) {
  this->size = newSize;
}

double GCell::getConcentration(int signal_id) {
  return this->signls[signal_id];
}

void GCell::setConcentration(int signal_id, double concentration) {
  this->signls[signal_id] = concentration;
}

void GCell::insertSignal(int signal_id, double signal_concentration) {
  this->signls.insert(std::pair<int,double>(signal_id, signal_concentration));
}

void GCell::deleteSignal(int signal_id) {
  this->signls.erase(signal_id);
}

void GCell::incSignal(int signal_id, double concentration) {
  this->signls[signal_id] += concentration;
}

void GCell::decSignal(int signal_id, double concentration) {
  this->signls[signal_id] -= concentration;
}

//int main() {}
//Function used for testing
// int main () {
//   std::map<int,double> pruebaM;
//   Cell obj = Cell(10, pruebaM);

//   obj.insertSignal(1, 3.4);
//   obj.insertSignal(2, 80);
//   //printf("Map size = %d\n", obj.getSignals().size());
  
//   //obj->deleteSignal(1);
//   //obj->incSignal(2, 79.0);
//   //obj->decSignal(2, 10);
  

//   // std::cout << obj->getConcentration(1) << '\n';
//   // std::cout << obj->getConcentration(2) << '\n';
// }

