#include <algorithm>
#include <iostream>

#include "pandemic.hpp"

Pandemic::Pandemic(int n, double b, double y, double r, double social, double in_infect_probability)
    : N{n}, S{N}, I{0}, D{0}, m_social{social}, B{b}, Y{y}, R{r} {

  if (N <= 0)
    throw std::runtime_error{"N must be greater than 0"};
  if (N % 10 != 0)
    throw std::runtime_error{"N must be a multiple of 10"};
  if (B < 0 || B > 1 || Y < 0 || Y > 1 || R < 0 || R > 1)
    throw std::runtime_error{"B , Y, R must be given in the range [0, 1]"};
  if (B == 0 && Y == 0 && R == 0)
    throw std::runtime_error{"If B, Y and R are zeros nothing will happen"};
  if (social < 1)
    throw std::runtime_error{"Sociality must be grater or equal to 1"};
  if (in_infect_probability <= 0)
    throw std::runtime_error{
        "Percentage of initial infected must be greater than 0"};

  population = std::vector<Person>(n, Person());
  gen();

  std::normal_distribution<double> normal(social, 2);
  std::uniform_real_distribution<double> uniform(0, 1);

  for (auto &v : population) {
    v.set_social(std::round(normal(gen())));
    while (v.get_social() < 1) {
      v.set_social(std::round(normal(gen())));
    }

    if (uniform(gen()) < in_infect_probability) {
      v.set_state(State::Infected);
      S--;
      I++;
    }
  }
}

Pandemic::Pandemic() {}

std::default_random_engine &Pandemic::gen() {
  static std::random_device rd;
  static std::default_random_engine gen{rd()};
  return gen;
}

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

void Pandemic::infect(int index, Pandemic &next, double b) const {
  std::uniform_real_distribution<double> uniform(0, 1);

  if (population[index].get_state() == State::Susceptible &&
      next.population[index].get_state() == State::Susceptible &&
      uniform(gen()) < b) {
    next.population[index].set_state(State::Infected);
    next.S--;
    next.I++;
  }
}

void Pandemic::die_or_heal(int index, Pandemic &next, double y,
                           double r) const {
  std::uniform_real_distribution<double> uniform(0, 1);

  if (uniform(gen()) < y) {
    next.population[index].set_state(State::Susceptible);
    next.S++;
    next.I--;
  } else if (uniform(gen()) < r) {
    next.population[index].set_state(State::Dead);
    next.I--;
    next.D++;
  }
}

Pandemic Pandemic::evolve() {
  std::uniform_int_distribution<int> uniform_1(0, N - 1);

  Pandemic next = *this;
  std::vector<Person>::iterator current_it;
  std::vector<Person>::iterator start_it = population.begin();

  while (true) {
    current_it = std::find_if(start_it, population.end(), [](Person p) -> bool {
      return p.get_state() == State::Infected;
    });

    if (current_it == population.end())
      break;

    start_it = current_it + 1;
    int index = std::distance(population.begin(), current_it);

    for (int j = 0; j < current_it->get_social(); j++) {
      int r_index = uniform_1(gen());
      infect(r_index, next, B);
    }
    die_or_heal(index, next, Y, R);
  }
  return next;
}