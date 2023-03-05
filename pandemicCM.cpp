#include "pandemicCM.hpp"
#include <cassert>
#include <random>

PandemicCM::PandemicCM(Pandemic const &p, double m, double q, int v)
    : Pandemic(p), Musk_trigger{m}, Quar_trigger{q}, Vacc_trigger{v} {};

void PandemicCM::setMusk_B_effect(double a)
{
    Musk_B_effect = a;
}

void PandemicCM::setVacc_B_effect(double a)
{
    Vacc_B_effect = a;
}

void PandemicCM::setVacc_Y_effect(double a)
{
    Vacc_Y_effect = a;
}

void PandemicCM::setVacc_R_effect(double a)
{
    Vacc_R_effect = a;
}

void PandemicCM::setQuar_goal(double a)
{
    Quar_goal = a;
}

PandemicCM PandemicCM::evolveCM(PandemicCM const &current, int d)
{
    std::default_random_engine gen{std::random_device{}()};
    std::uniform_real_distribution<double> uniform_0(0, 1);
    std::uniform_int_distribution<int> uniform_1(0, N - 1);

    PandemicCM next = current;

    int j = 0;
    for (auto const &v : current.population)
    {
        double b = B;
        double y = Y;
        double r = R;
        double IS = static_cast<double>(I) / static_cast<double>(S);
        double IDN = static_cast<double>(I + D) / static_cast<double>(N);

        if (IDN >= Musk_trigger && Musk_trigger != 0)
            b = b * Musk_B_effect;
        if (d >= Vacc_trigger && Vacc_trigger != 0)
        {
            b = b * Vacc_B_effect;
            y = y * Vacc_Y_effect;
            r = r * Vacc_R_effect;
        }

        if (v.getState() == State::Infected)
        {
            if ((IS < Quar_trigger || Quar_trigger == 0) && Quar == 0)
            {
                for (int j = 0; j < v.getSocial(); j++)
                {
                    double r_0 = uniform_0(gen);
                    int r_1 = uniform_1(gen);
                    if (r_0 < b && next.population[r_1].getState() == State::Susceptible)
                    {
                        next.population[r_1].setState(State::Infected);
                        next.S--;
                        next.I++;
                    }
                }
            }
            else if (Quar == 0)
                next.Quar = 1;
            else if (Quar == 1 && IS <= Quar_goal)
                next.Quar = 0;

            if (uniform_0(gen) < y)
            {
                next.population[j].setState(State::Susceptible);
                next.S++;
                next.I--;
            }
            else if (uniform_0(gen) < r)
            {
                next.population[j].setState(State::Dead);
                next.I--;
                next.D++;
            }
        }
        j++;
    }
    return next;
}