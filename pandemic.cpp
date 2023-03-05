#include "pandemic.hpp"

#include <cassert>
#include <iostream>
#include <random>

Pandemic::Pandemic(int n, double b, double y, double r, double i)
    : population(n, Person()), N{n}, B{b}, Y{y}, R{r} {
  S = N;
  std::default_random_engine gen{std::random_device{}()};
  std::normal_distribution<double> dist_0(4, 2);
  std::uniform_real_distribution<double> dist_1(0, 1);

  for (auto &v : population) {
    do {
      v.setSocial(std::round(dist_0(gen)));
    } while (v.getSocial() < 1 || v.getSocial() > 7);

    assert(v.getSocial() > 0 && v.getSocial() < 8);

    if (dist_1(gen) < i) {
      v.setState(State::Infected);
      S--;
      I++;
    }
  }
}

Pandemic::Pandemic(Pandemic const &p1) {
  population = p1.population;
  N = p1.N;
  B = p1.B;
  Y = p1.Y;
  R = p1.R;
  S = p1.S;
  I = p1.I;
  D = p1.D;
}

int Pandemic::getSusceptible() const { return S; }

int Pandemic::getInfected() const { return I; }

int Pandemic::getDead() const { return D; }

int Pandemic::getPeopleN() const { return N; }

Pandemic Pandemic::evolve(Pandemic const &current) {
  std::default_random_engine gen{std::random_device{}()};
  std::uniform_real_distribution<double> uniform_0(0, 1);
  std::uniform_int_distribution<int> uniform_1(0, N - 1);

  Pandemic next = current;
  int j = 0;
  for (auto const &v : current.population) {
    if (v.getState() == State::Infected) {
      for (int j = 0; j < v.getSocial(); j++) {
        double r_0 = uniform_0(gen);
        int r_1 = uniform_1(gen);
        if (r_0 < B && next.population[r_1].getState() == State::Susceptible) {
          next.population[r_1].setState(State::Infected);
          next.S--;
          next.I++;
        }
      }

      if (uniform_0(gen) < Y) {
        next.population[j].setState(State::Susceptible);
        next.S++;
        next.I--;
      } else if (uniform_0(gen) < R) {
        next.population[j].setState(State::Dead);
        next.I--;
        next.D++;
      }
    }
    j++;
  }

  return next;
}