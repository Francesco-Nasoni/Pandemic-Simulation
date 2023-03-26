#include "pandemic.hpp"

#include <cassert>
#include <iostream>
#include <random>
#include <stdexcept>

Pandemic::Pandemic(int n, double b, double y, double r, double social, double i)
    : population(n, Person()), N{n}, B{b}, Y{y}, R{r} {
      
  if ( B<0 || B>1  || Y<0 || Y>1 || R<0 || R>1) 
      throw std::runtime_error{"B , Y, R must be given in the range [0, 1]"};
  if ( N <= 0)
      throw std::runtime_error{"N must be greater than 0"};
  if ( social < 1)
      throw std::runtime_error{"Sociality must be grater or equal to 1"};
  if( i <= 0)
      throw std::runtime_error{"Percentage of initial infected must be greater than 0"};

  S = N;
  std::default_random_engine gen{std::random_device{}()};
  std::normal_distribution<double> dist_0(social, 2);
  std::uniform_real_distribution<double> dist_1(0, 1);

  for (auto &v : population) {
    do {
      v.setSocial(std::round(dist_0(gen)));
    } while (v.getSocial() < 1);

    if (dist_1(gen) < i) {
      v.setState(State::Infected);
      S--;
      I++;
    }
  }
}

int Pandemic::getSusceptible() const { return S; }

int Pandemic::getInfected() const { return I; }

int Pandemic::getDead() const { return D; }

int Pandemic::getPeopleN() const { return N; }

Pandemic Pandemic::evolve() {
  std::default_random_engine gen{std::random_device{}()};
  std::uniform_real_distribution<double> uniform_0(0, 1);
  std::uniform_int_distribution<int> uniform_1(0, N - 1);

  Pandemic next = *this;
  int j = 0;
  for (auto const &v : population) {
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