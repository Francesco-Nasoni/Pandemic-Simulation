#ifndef PANDEMICCM_HPP
#define PANDEMICCM_HPP

#include "pandemic.hpp"

class PandemicCM : public Pandemic {
private:

  bool Quar = false;
  bool vacc_1 = false;
  bool vacc_2 = false;

  double vacc_B_effect = 1.;
  double vacc_Y_effect = 1.;
  double vacc_R_effect = 1.;


public:
  PandemicCM(const Pandemic &, double, double, double);
  void die_or_live(int index, PandemicCM &next) const;
  void infect(int i_1, int i_2, PandemicCM &next) const;
  void toggle_quar();
  void toggle_vacc_1();
  void toggle_vacc_2();
  PandemicCM evolve() const;
  PandemicCM evolveCM() const;
};

#endif