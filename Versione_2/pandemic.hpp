#ifndef PANDEMIC_HPP
#define PANDEMIC_HPP

#include "person.hpp"
#include <vector>

class Pandemic {
protected:
  std::vector<Person> population;
  int N;    // dimensione della popolazione
  double B; // probabilità di trasmissione
  double Y; // probabilità di guarigione
  double R; // probabilità di morte
  int S;
  int I = 0;
  int D = 0;

public:
  Pandemic(int, double, double, double, double, double);
  int getSusceptible() const;
  int getInfected() const;
  int getDead() const;
  int getPeopleN() const;
  bool isEnded()const;
  Pandemic evolve();
};

#endif