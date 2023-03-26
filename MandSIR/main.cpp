#include "pandemic.hpp"
#include <iostream>

int main() {
  double B = 0.20;
  double Y = 0.107;
  double S = 10000;
  double I = 50;

  Pandemic sample{B, Y, S, I};

  std::cout << std::endl
            << "Giorno" << '\t' << "Sani" << '\t' << "Infetti" << '\t'
            << "Rimossi" << '\n';
  sample.roundPrint(0);

  unsigned int i = 1;
  while (!sample.isEnded()) {
    sample.evolve();
    sample.roundPrint(i);
    i++;
  }
}