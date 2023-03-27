#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <thread>

#include "pandemicCM.hpp"

#define FIXED_FLOAT(x) std::fixed << std::setprecision(x)
#define SPACE(x) std::left << std::setw(x)

void write(std::ostream &os, Pandemic const &sample, int d) {
  os << d << '\t' << sample.getSusceptible() << '\t' << sample.getInfected()
     << '\t' << sample.getDead() << '\n';
}

void print(Pandemic const &s1, Pandemic const &s2, int d1, int d2) {
  double SI_1 = static_cast<double>(s1.getInfected()) /
                static_cast<double>(s1.getSusceptible());
  double SI_2 = static_cast<double>(s2.getInfected()) /
                static_cast<double>(s2.getSusceptible());
  double IDN_1 = static_cast<double>(s1.getInfected() + s1.getDead()) /
                 static_cast<double>(s1.getPeopleN());
  double IDN_2 = static_cast<double>(s2.getInfected() + s2.getDead()) /
                 static_cast<double>(s2.getPeopleN());

  constexpr auto clear = "\033[2J";
  std::cout << clear;
  std::cout << FIXED_FLOAT(3) << SPACE(6) << "Day: " << SPACE(6) << d1
            << SPACE(6) << " " << SPACE(6) << "Day: " << SPACE(6) << d2
            << std::endl
            << SPACE(6) << "S: " << SPACE(6) << s1.getSusceptible() << SPACE(6)
            << " " << SPACE(6) << "S: " << SPACE(6) << s2.getSusceptible()
            << std::endl
            << SPACE(6) << "I: " << SPACE(6) << s1.getInfected() << SPACE(6)
            << " " << SPACE(6) << "I: " << s2.getInfected() << std::endl
            << SPACE(6) << "D: " << SPACE(6) << s1.getDead() << SPACE(6) << " "
            << SPACE(6) << "D: " << s2.getDead() << std::endl
            << SPACE(6) << "S/I: " << SPACE(6) << SI_1 << SPACE(6) << " "
            << SPACE(6) << "S/I: " << SI_2 << std::endl
            << SPACE(6) << "ID/N: " << SPACE(6) << IDN_1 << SPACE(6) << " "
            << SPACE(6) << "ID/N: " << IDN_2 << std::endl;
}

int main() {
  int population_size = 10000;
  double initial_infected = 0.5 / 100;
  double sociality=4;
  double B = 0.05;
  double Y = 0.1;
  double R = 0.007;
  double Musk_trigger = 0.20;
  double Quar_trigger = 0.16;
  int Vacc_trigger = 0;

  std::ofstream file{"Data/Result.txt", std::ofstream::trunc};
  std::ofstream fileCM{"Data/ResultCM.txt", std::ofstream::trunc};

  Pandemic sample(population_size, B, Y, R, sociality, initial_infected);
  PandemicCM sampleCM(sample, Musk_trigger, Quar_trigger, Vacc_trigger);
  sampleCM.setMusk_B_effect(0.8);
  sampleCM.setVacc_B_effect(0.8);
  sampleCM.setVacc_Y_effect(1.2);
  sampleCM.setVacc_R_effect(0.80);
  sampleCM.setQuar_goal(0.04);

  int day = 0;
  int dayCM = 0;

  write(file, sample, day);
  write(fileCM, sample, day);
  print(sample, sampleCM, day, dayCM);

  while (sample.getInfected() > 0 || sampleCM.getInfected() > 0) {
    if (sample.getInfected() > 0) {
      day++;
      sample = sample.evolve();
      write(file, sample, day);
    }
    if (sampleCM.getInfected() > 0) {
      dayCM++;
      sampleCM = sampleCM.evolveCM(dayCM);
      write(fileCM, sampleCM, dayCM);
    }
    print(sample, sampleCM, day, dayCM);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  std::cout << '\n' << '\n';
};