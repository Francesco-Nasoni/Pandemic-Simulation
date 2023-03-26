#ifndef PANDEMIC_HPP
#define PANDEMIC_HPP

#include <iostream>
#include <stdexcept>

struct Population {
  double S;
  double I;
  double R;
};

class Pandemic {
private:
  double B;
  double Y;
  double N;
  Population p;

public:
  Pandemic(double, double, double, double);
  Population round() const;
  void roundPrint(unsigned int) const;
  bool isEnded() const;
  void evolve();
};

#endif