#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>

#include "pandemicCM.hpp"

PandemicCM::PandemicCM(const Pandemic &p, double m, double q, int v)
    : Pandemic(p), Musk_trigger{m}, Quar_trigger{q}, Vacc_trigger{v}
{
    if (q < 0 || q > 1 || m < 0 || m > 1)
        throw std::runtime_error{"Musk trigger and Quar trigger must be given in the range [0, 1]"};
    if (v > 0)
        throw std::runtime_error{"Vacc trigger must be greate or equal to 0"};
}

void PandemicCM::die_or_live(int index, PandemicCM &next) const
{
    std::default_random_engine gen{std::random_device{}()};
    std::uniform_real_distribution<double> uniform_0(0, 1);

    double y = Y;
    double r = R;

    if (Vacc_2)
    {
        y = Y * Vacc_Y_effect;
        r = R * Vacc_R_effect;
    }
    else if (Vacc_1)
    {
        if (population[index].get_social() > m_social * 1.34)
        {
            y = Y * Vacc_Y_effect;
            r = R * Vacc_R_effect;
        }
    }

    if (uniform_0(gen) < y)
    {
        next.population[index].set_state(State::Susceptible);
        next.S++;
        next.I--;
    }
    else if (uniform_0(gen) < r)
    {
        next.population[index].set_state(State::Dead);
        next.I--;
        next.D++;
    }
}

void PandemicCM::infect(int i_1, int i_2, PandemicCM &next) const
{

    double b = B;

    if (Vacc_2)
    {
        b = B * Vacc_B_effect;
    }
    else if (Vacc_1)
    {
        if (population[i_1].get_social() > m_social * 1.34 || population[i_2].get_social() > m_social * 1.34)
            b = B * Vacc_B_effect;
    }

    std::default_random_engine gen{std::random_device{}()};
    std::uniform_real_distribution<double> uniform_0(0, 1);

    if (next.population[i_2].get_state() == State::Susceptible && population[i_2].get_state() == State::Susceptible &&
        uniform_0(gen) < b)
    {
        next.population[i_2].set_state(State::Infected);
        next.S--;
        next.I++;
    }
}

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

bool PandemicCM::getQuar() const
{
    return Quar;
}

void PandemicCM::setQuar(bool a)
{
    Quar = a;
}

PandemicCM PandemicCM::evolve()
{
    std::default_random_engine gen{std::random_device{}()};
    std::uniform_real_distribution<double> uniform_0(0, 1);
    std::uniform_int_distribution<int> uniform_1(0, N - 1);

    PandemicCM next = *this;
    std::vector<Person>::iterator current_it;
    std::vector<Person>::iterator next_it;
    std::vector<Person>::iterator start_it = population.begin();

    while (true)
    {
        current_it =
            std::find_if(start_it, population.end(), [](Person p) -> bool { return p.get_state() == State::Infected; });

        if (current_it == population.end())
            break;

        start_it = current_it + 1;

        for (int i = 0; i < current_it->get_social(); i++)
        {
            infect(std::distance(population.begin(), current_it), uniform_1(gen), next);
        }
        die_or_live(std::distance(population.begin(), current_it), next);
    }
    return next;
}

PandemicCM PandemicCM::evolveCM(int d)
{
    if (Quar == 0)
    {
        return this->evolve();
    }
    else
    {
        std::default_random_engine gen{std::random_device{}()};
        std::uniform_int_distribution<int> uniform_1(0, N / 10 - 1);
        std::uniform_int_distribution<int> uniform_2(0, 1);
        std::uniform_int_distribution<int> uniform_3(0, 10);

        PandemicCM next = *this;
        for (int i = 0; i < N; i = i + 10)
        {
            for (int j = 0; j < 2; j++)
            {
                if (population[i + j].get_state() == State::Infected)
                    for (int l = 0; l < 2; l++)
                    {
                        int index = uniform_1(gen) * 10 + uniform_2(gen);
                        infect(i + j, index, next);
                    }
            }
        }

        for (int i = 0; i < N; i = i + 10)
        {
            for (int j = 0; j < 10; j++)
            {
                if (population[i + j].get_state() == State::Infected)
                    for (int l = 0; l < 2; l++)
                    {
                        int index = i + uniform_3(gen);
                        infect(i + j, index, next);
                    }
                die_or_live(i + j, next);
            }
        }
        return next;
    }
}