#ifndef PANDEMIC_HPP
#define PANDEMIC_HPP

#include "person.hpp"
#include <vector>

class Pandemic {
protected:
  std::vector<Person> population;
  double m_social;
  int N;
  double B;
  double Y;
  double R;
  int S;
  int I;
  int D;

public:
  Pandemic(int, double, double, double, double, double);
  Pandemic();
  int get_susceptible() const;
  int get_infected() const;
  int get_dead() const;
  int get_N() const;
  bool is_ended() const;
  Pandemic evolve();
};

#endif