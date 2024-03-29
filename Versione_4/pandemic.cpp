#include <algorithm>
#include <iostream>
#include <random>

#include "pandemic.hpp"

Pandemic::Pandemic(int n, double b, double y, double r, double social, double i)
    : population(n, Person()), m_social{social}, N{n}, B{b}, Y{y}, R{r}, S{N},
      I{0}, D{0} {

  if (B < 0 || B > 1 || Y < 0 || Y > 1 || R < 0 || R > 1)
    throw std::runtime_error{"B , Y, R must be given in the range [0, 1]"};
  if (B == 0 && Y == 0 && R == 0)
    throw std::runtime_error{"If B, Y and R are zeros nothing will happen"};
  if (N <= 0)
    throw std::runtime_error{"N must be greater than 0"};
  if (social < 1)
    throw std::runtime_error{"Sociality must be grater or equal to 1"};
  if (i <= 0)
    throw std::runtime_error{
        "Percentage of initial infected must be greater than 0"};

  std::default_random_engine gen{std::random_device{}()};
  std::normal_distribution<double> dist_0(social, 2);
  std::uniform_real_distribution<double> dist_1(0, 1);

  for (auto &v : population) {
    v.set_social(std::round(dist_0(gen)));
    while (v.get_social() < 1) {
      v.set_social(std::round(dist_0(gen)));
    }
    while (v.get_social() < 1)
      ;

    if (dist_1(gen) < i) {
      v.set_state(State::Infected);
      S--;
      I++;
    }
  }
}

Pandemic::Pandemic() {}

int Pandemic::get_susceptible() const { return S; }

int Pandemic::get_infected() const { return I; }

int Pandemic::get_dead() const { return D; }

int Pandemic::get_N() const { return N; }

bool Pandemic::is_ended() const {
  if (I == 0)
    return 1;
  else if (Y == 0 && R == 0 && S == 0)
    return 1;
  else
    return 0;
}

void Pandemic::infect(std::vector<Person>::iterator n_it, Pandemic &next,
                      double b) const {
  std::default_random_engine gen{std::random_device{}()};
  std::uniform_real_distribution<double> uniform_0(0, 1);

  std::vector<Person>::const_iterator c_it =
      population.begin() + std::distance(next.population.begin(), n_it);
  if (c_it->get_state() == State::Susceptible &&
      n_it->get_state() == State::Susceptible && uniform_0(gen) < b) {
    n_it->set_state(State::Infected);
    next.S--;
    next.I++;
  }
}

void Pandemic::die_or_heal(std::vector<Person>::iterator n_it, Pandemic &next,
                           double y, double r) const {
  std::default_random_engine gen{std::random_device{}()};
  std::uniform_real_distribution<double> uniform_0(0, 1);

  if (uniform_0(gen) < y) {
    n_it->set_state(State::Susceptible);
    next.S++;
    next.I--;
  } else if (uniform_0(gen) < r) {
    n_it->set_state(State::Dead);
    next.I--;
    next.D++;
  }
}

Pandemic Pandemic::evolve() {
  std::default_random_engine gen{std::random_device{}()};
  std::uniform_int_distribution<int> uniform_1(0, N - 1);

  Pandemic next = *this;
  std::vector<Person>::iterator current_it;
  std::vector<Person>::iterator next_it;
  std::vector<Person>::iterator start_it = population.begin();

  for (;;) {
    current_it = std::find_if(start_it, population.end(), [](Person p) -> bool {
      return p.get_state() == State::Infected;
    });

    if (current_it == population.end())
      break;

    start_it = current_it + 1;
    next_it =
        next.population.begin() + std::distance(population.begin(), current_it);

    for (int j = 0; j < current_it->get_social(); j++) {
      int r = uniform_1(gen);
      infect(next.population.begin() + r, next, B);
    }
    die_or_heal(next_it, next, Y, R);
  }
  return next;
}