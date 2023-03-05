#ifndef PANDEMICCM_HPP
#define PANDEMICCM_HPP

#include "pandemic.hpp"

class PandemicCM : public Pandemic
{
  private:
    double Musk_trigger;
    double Quar_trigger;
    int Vacc_trigger;
    double Musk_B_effect = 1.;
    double Vacc_B_effect = 1.;
    double Vacc_Y_effect = 1.;
    double Vacc_R_effect = 1.;
    double Quar_goal = 0;
    bool Quar = 0;

  public:
    PandemicCM(Pandemic const &, double, double, int);
    void setMusk_B_effect(double);
    void setVacc_B_effect(double);
    void setVacc_Y_effect(double);
    void setVacc_R_effect(double);
    void setQuar_goal(double);
    PandemicCM evolveCM(PandemicCM const &, int);
};

#endif