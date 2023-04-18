#ifndef PANDEMICCM_HPP
#define PANDEMICCM_HPP

#include "pandemic.hpp"

class PandemicCM : public Pandemic {
private:
  bool quar;
  bool vacc_1;
  bool vacc_2;
  double vacc_B_effect;
  double vacc_Y_effect;
  double vacc_R_effect;

  double eval_B(int i_1, int i_2) const;
  double eval_Y(int index) const;
  double eval_R(int index) const;
  PandemicCM evolve() const;

public:
  PandemicCM(const Pandemic &, double, double, double);
  PandemicCM();
  void toggle_quar();
  void toggle_vacc_1();
  void toggle_vacc_2();
  bool get_quar();
  PandemicCM evolveCM() const;
};

#endif