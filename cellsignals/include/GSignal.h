class GSignal {
 private:
  int id;
  double kdiff;
  double kdeg;

 public:
  //Constructor
 GSignal(int id, double diff, double deg): id(id), kdiff(diff), kdeg(deg) {};
  GSignal() {};
  
  
  // Setters y getters
  void set_kdiff(double newKdiff) {
    this->kdiff = newKdiff;
  }

  void set_kdeg(double newKdeg) {
    this->kdeg = newKdeg;
  }

  int getId() {
    return this->id;
  }

  double get_kdiff() {
    return this->kdiff;
  }
  
  double get_kdeg() {
    return this->kdeg;
  }

  /* bool operator== (const GSignal& s) { */
  /*   return this->id == s.id; */
  /* } */

  bool operator== (int s_id) {
    return this->id == s_id;
  }
};
