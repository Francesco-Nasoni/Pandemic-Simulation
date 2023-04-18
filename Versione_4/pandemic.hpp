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
  void infect(std::vector<Person>::iterator it_2, Pandemic &next,
              double b) const;
  void die_or_heal(std::vector<Person>::iterator, Pandemic &next, double y,
                   double r) const;
  Pandemic evolve();
};

#endif