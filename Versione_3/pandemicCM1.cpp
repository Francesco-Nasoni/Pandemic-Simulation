#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>

#include "pandemicCM.hpp"

PandemicCM::PandemicCM(const Pandemic &p, double V_B_E, double V_Y_E,
                       double V_R_E)
    : Pandemic(p), vacc_B_effect{V_B_E}, vacc_Y_effect{V_Y_E}, vacc_R_effect{
                                                                   V_R_E} {
  if (V_B_E < 0  || V_Y_E < 0 || V_R_E < 0 )
    throw std::runtime_error{
        "The effects of countermeasure are multiplicative factor," 
        "they must be positive or zero"};
}

void PandemicCM::die_or_live(int index, PandemicCM &next) const {
  std::default_random_engine gen{std::random_device{}()};
  std::uniform_real_distribution<double> uniform_0(0, 1);

  double y = Y;
  double r = R;

  if (vacc_2) {
    y = Y * vacc_Y_effect;
    r = R * vacc_R_effect;
  } else if (vacc_1) {
    if (population[index].get_social() > m_social * 1.34) {
      y = Y * vacc_Y_effect;
      r = R * vacc_R_effect;
    }
  }

  if (uniform_0(gen) < y) {
    next.population[index].set_state(State::Susceptible);
    next.S++;
    next.I--;
  } else if (uniform_0(gen) < r) {
    next.population[index].set_state(State::Dead);
    next.I--;
    next.D++;
  }
}

void PandemicCM::infect(int i_1, int i_2, PandemicCM &next) const {

  double b = B;

  if (vacc_2) {
    b = B * vacc_B_effect;
  } else if (vacc_1) {
    if (population[i_1].get_social() > m_social * 1.34 ||
        population[i_2].get_social() > m_social * 1.34)
      b = B * vacc_B_effect;
  }

  std::default_random_engine gen{std::random_device{}()};
  std::uniform_real_distribution<double> uniform_0(0, 1);

  if (next.population[i_2].get_state() == State::Susceptible &&
      population[i_2].get_state() == State::Susceptible && uniform_0(gen) < b) {
    next.population[i_2].set_state(State::Infected);
    next.S--;
    next.I++;
  }
}

void PandemicCM::toggle_quar() { Quar = !Quar; };

void PandemicCM::toggle_vacc_1() {
  if (!vacc_1)
    vacc_1 = !vacc_1;
};

void PandemicCM::toggle_vacc_2() {
  if (!vacc_2)
    vacc_2 = !vacc_2;
};

PandemicCM PandemicCM::evolve() const {
  std::default_random_engine gen{std::random_device{}()};
  std::uniform_real_distribution<double> uniform_0(0, 1);
  std::uniform_int_distribution<int> uniform_1(0, N - 1);

  PandemicCM next = *this;
  std::vector<Person>::const_iterator current_it;
  std::vector<Person>::iterator next_it;
  std::vector<Person>::const_iterator start_it = population.begin();

  while (true) {
    current_it = std::find_if(start_it, population.end(), [](Person p) -> bool {
      return p.get_state() == State::Infected;
    });

    if (current_it == population.end())
      break;

    start_it = current_it + 1;

    for (int i = 0; i < current_it->get_social(); i++) {
      infect(std::distance(population.begin(), current_it), uniform_1(gen),
             next);
    }
    die_or_live(std::distance(population.begin(), current_it), next);
  }
  return next;
}

PandemicCM PandemicCM::evolveCM() const {
  if (Quar == false) {
    return this->evolve();
  } else {
    std::default_random_engine gen{std::random_device{}()};
    std::uniform_int_distribution<int> uniform_1(0, N / 10 - 1);
    std::uniform_int_distribution<int> uniform_2(0, 1);
    std::uniform_int_distribution<int> uniform_3(0, 10);

    PandemicCM next = *this;
    for (int i = 0; i < N; i = i + 10) {
      for (int j = 0; j < 2; j++) {
        if (population[i + j].get_state() == State::Infected){
          for (int l = 0; l < 2; l++) {
            int index = uniform_1(gen) * 10 + uniform_2(gen);
            infect(i + j, index, next);
          }
        }
      }
    }

    for (int i = 0; i < N; i = i + 10) {
      for (int j = 0; j < 10; j++) {
        if (population[i + j].get_state() == State::Infected){
          for (int l = 0; l < 2; l++) {
            int index = i + uniform_3(gen);
            infect(i + j, index, next);
          }
          die_or_live(i + j, next);
        }
      }
    }
    return next;
  }
}