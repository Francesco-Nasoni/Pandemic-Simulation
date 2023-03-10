#ifndef PANDEMIC_HPP
#define PANDEMIC_HPP

#include "person.hpp"
#include <vector>

class Pandemic {
protected:
  std::vector<Person> population;
  int N;    // dimensione della popolazione
  double B; // probabilit√† di trasmissione
  double Y; // probabilit√† di guarigione
  double R; // probabilit√† di morte
  int S;
  int I = 0;
  int D = 0;

public:
  Pandemic(int, double, double, double, double);
  Pandemic(Pandemic const &);
  int getSusceptible() const;
  int getInfected() const;
  int getDead() const;
  int getPeopleN() const;
  Pandemic evolve(Pandemic const &);
};

#endif