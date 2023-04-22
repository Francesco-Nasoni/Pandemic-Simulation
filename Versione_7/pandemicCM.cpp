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
    if (population[i_1].get_social() > m_social * in_a_sigma ||
        population[i_2].get_social() > m_social * in_a_sigma) {
      return B * vacc_B_effect;
    }
  }
  return B;
}

double PandemicCM::eval_Y(int index) const {
  if (vacc_2) {
    return Y * vacc_Y_effect;
  } else if (vacc_1) {
    if (population[index].get_social() > m_social * in_a_sigma) {
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


void PandemicCM::evolve(){

  std::uniform_int_distribution<int> uniform_1(0, N - 1);

  std::vector<Person> old_population = this->population;
  std::vector<Person>::iterator old_it;
  std::vector<Person>::iterator start_it = old_population.begin();

  while (true) {
    old_it = std::find_if(start_it, old_population.end(), [](Person p) -> bool {
      return p.get_state() == State::Infected;
    });

    if (old_it == old_population.end())
      break;

    start_it = old_it + 1;
    int index = std::distance(old_population.begin(), old_it);

    for (int i = 0; i < old_it->get_social(); i++) {
      int r_index = uniform_1(gen());
      infect(r_index, old_population, eval_B(index, r_index));
    }
    die_or_heal(index, eval_Y(index), eval_R(index));
  }
}


void PandemicCM::evolveCM(){
  if (quar == false) {
    evolve();
  } else {
    std::uniform_real_distribution<double> uniform_0(0, 1);
    std::uniform_int_distribution<int> uniform_1(0, N / 10 - 1);
    std::uniform_int_distribution<int> uniform_2(0, n_worker - 1);
    std::uniform_int_distribution<int> uniform_3(0, family_size - 1);

    std::vector<Person> old_population = this->population;
    for (int i = 0; i < N; i = i + family_size) {
      for (int j = 0; j < n_worker; j++) {
        if (population[i + j].get_state() == State::Infected) {
          for (int l = 0; l < n_worker; l++) {
            int r_index = uniform_1(gen()) * family_size + uniform_2(gen());
            infect(r_index, old_population, eval_B(i + j, r_index));
          }
        }
      }
    }

    for (int i = 0; i < N; i = i + family_size) {
      for (int j = 0; j < family_size; j++) {
        if (population[i + j].get_state() == State::Infected) {
          for (int l = 0; l < n_worker; l++) {
            int r_index = i + uniform_3(gen());
            infect(r_index, old_population, eval_B(i + j, r_index));
          }
          die_or_heal(i + j, eval_Y(i + j), eval_R(i + j));
        }
      }
    }
  }
}