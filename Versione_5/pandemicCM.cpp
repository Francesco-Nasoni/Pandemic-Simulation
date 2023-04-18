#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>

#include "pandemicCM.hpp"

PandemicCM::PandemicCM(const Pandemic &p, double V_B_E, double V_Y_E,
                       double V_R_E)
    : Pandemic(p), quar{false}, vacc_1{false}, vacc_2{false},
      vacc_B_effect{V_B_E}, vacc_Y_effect{V_Y_E}, vacc_R_effect{V_R_E} {
  if (V_B_E < 0 || V_Y_E < 0 || V_R_E < 0)
    throw std::runtime_error{"The effects of countermeasure are multiplicative "
                             "factor, they must be positive or zero"};
}

PandemicCM::PandemicCM() {}

double PandemicCM::eval_B(int i_1, int i_2) const {
  if (vacc_2) {
    return B * vacc_B_effect;
  } else if (vacc_1) {
    if (population[i_1].get_social() > m_social * 1.34 ||
        population[i_2].get_social() > m_social * 1.34) {
      return B * vacc_B_effect;
    }
  }
  return B;
}

double PandemicCM::eval_Y(int index) const {
  if (vacc_2) {
    return Y * vacc_Y_effect;
  } else if (vacc_1) {
    if (population[index].get_social() > m_social * 1.34) {
      return Y * vacc_Y_effect;
    }
  }
  return Y;
}

double PandemicCM::eval_R(int index) const {
  if (vacc_2) {
    return R * vacc_R_effect;
  } else if (vacc_1) {
    if (population[index].get_social() > m_social * 1.34) {
      return R * vacc_R_effect;
    }
  }
  return R;
}

void PandemicCM::toggle_quar() { quar = !quar; };

void PandemicCM::toggle_vacc_1() {
  if (!vacc_1)
    vacc_1 = !vacc_1;
};

void PandemicCM::toggle_vacc_2() {
  if (!vacc_2)
    vacc_2 = !vacc_2;
};

bool PandemicCM::get_quar() { return quar; };

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
    int index = std::distance(population.begin(), current_it);

    for (int i = 0; i < current_it->get_social(); i++) {
      int r_index = uniform_1(gen);
      if (population[r_index].get_state() == State::Susceptible &&
          next.population[r_index].get_state() == State::Susceptible &&
          uniform_0(gen) < eval_B(index, r_index)) {
        infect(r_index, next);
      }
    }
    die_or_heal(index, next, eval_Y(index), eval_R(index));
  }
  return next;
}

PandemicCM PandemicCM::evolveCM() const {
  if (quar == false) {
    return this->evolve();
  } else {
    std::default_random_engine gen{std::random_device{}()};
    std::uniform_real_distribution<double> uniform_0(0, 1);
    std::uniform_int_distribution<int> uniform_1(0, N / 10 - 1);
    std::uniform_int_distribution<int> uniform_2(0, 1);
    std::uniform_int_distribution<int> uniform_3(0, 9);

    PandemicCM next = *this;
    for (int i = 0; i < N; i = i + 10) {
      for (int j = 0; j < 2; j++) {
        if (population[i + j].get_state() == State::Infected) {
          for (int l = 0; l < 2; l++) {
            int r_index = uniform_1(gen) * 10 + uniform_2(gen);
            if (population[r_index].get_state() == State::Susceptible &&
                next.population[r_index].get_state() == State::Susceptible &&
                uniform_0(gen) < eval_B(i + j, r_index)) {
              infect(r_index, next);
            }
          }
        }
      }
    }

    for (int i = 0; i < N; i = i + 10) {
      for (int j = 0; j < 10; j++) {
        if (population[i + j].get_state() == State::Infected) {
          for (int l = 0; l < 2; l++) {
            int r_index = i + uniform_3(gen);
            if (population[r_index].get_state() == State::Susceptible &&
                next.population[r_index].get_state() == State::Susceptible &&
                uniform_0(gen) < eval_B(i + j, r_index)) {
              infect(r_index, next);
            }
          }
          die_or_heal(i + j, next, eval_Y(i + j), eval_R(i + j));
        }
      }
    }
    return next;
  }
}