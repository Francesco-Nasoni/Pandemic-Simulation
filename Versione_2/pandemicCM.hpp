#ifndef PANDEMICCM_HPP
#define PANDEMICCM_HPP

#include "pandemic.hpp"

class PandemicCM : public Pandemic {
private:
  double Musk_trigger; // (I+D)/N si attiva
  double Quar_trigger;  //(I/S)
  int Vacc_trigger; //day
  double Musk_B_effect = 1.;
  double Vacc_B_effect = 1.;
  double Vacc_Y_effect = 1.;
  double Vacc_R_effect = 1.;
  double Quar_goal = 0; // (I/S) si disattiva
  bool Quar = 0;

public:
  PandemicCM(const Pandemic&, double, double, int);
  //PandemicCM(const PandemicCM &);
  void setMusk_B_effect(double);
  void setVacc_B_effect(double);
  void setVacc_Y_effect(double);
  void setVacc_R_effect(double);
  void setQuar_goal(double);
  PandemicCM evolveCM(int);
};

#endif