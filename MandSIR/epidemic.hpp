#ifndef EPIDEMIC_HPP
#define EPIDEMIC_HPP

#include <iostream>
#include <stdexcept>

struct Population {
  double S;
  double I;
  double R;
};

class Epidemic {
private:
  double B;
  double Y;
  double N;
  Population pop;

public:
  Epidemic(double, double, double, double);
  Epidemic(){};
  Population round() const;
  void round_print(unsigned int) const;
  bool is_ended() const;
  void evolve();
};

#endif