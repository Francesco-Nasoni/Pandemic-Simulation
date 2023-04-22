#ifndef PANDEMIC_HPP
#define PANDEMIC_HPP

#include "person.hpp"
#include <random>
#include <vector>

class Pandemic {
protected:
  std::vector<Person> population;
  int N;
  int S;
  int I;
  int D;
  double m_social;
  double B;
  double Y;
  double R;

  static std::default_random_engine &gen();

public:
  Pandemic(int, double, double, double, double, double);
  Pandemic();
  int get_susceptible() const;
  int get_infected() const;
  int get_dead() const;
  int get_N() const;
  bool is_ended() const;
  void infect(int index, Pandemic &next, double b) const;
  void die_or_heal(int index, Pandemic &next, double y, double r) const;
  Pandemic evolve();
};

#endif