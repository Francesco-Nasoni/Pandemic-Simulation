#include "pandemic.hpp"
#include <cmath>
#include <iostream>

Pandemic::Pandemic(double B_, double Y_, double S_, double I_)
    : B{B_}, Y{Y_}, N{S_ + I_}, p{S_, I_, 0} {
  if (N == 0 && Y == 0 && B == 0)
    throw std::runtime_error{"Can't perform simulation with all zeros"};
  if (Y == 0 && B == 0)
    throw std::runtime_error{"With B=0 and Y=0 nothing will happen"};
  if (S_ < 0 || I_ < 0)
    throw std::runtime_error{"S<0 or I<0 is nonsensical"};
  if (B < 0 || Y < 0)
    throw std::runtime_error{"B<0 and Y<0 is nonsensical"};
}

Population Pandemic::round() const {
  Population p1 = p;

  if (std::round(p1.S) + std::round(p1.I) + std::round(p1.R) > N) {
    if (p1.S < p1.I)
      if (p1.S < p1.R)
        p1.S = std::round(p1.S) - 1;
      else
        p1.R = std::round(p1.R) - 1;
    else if (p1.I < p1.R)
      p1.I = std::round(p1.I) - 1;
    else
      p1.R = std::round(p1.R) - 1;
  } else if (std::round(p1.S) + std::round(p1.I) + std::round(p1.R) < N) {
    if (p1.S > p1.I)
      if (p1.S > p1.R)
        p1.S = std::round(p1.S) + 1;
      else
        p1.R = std::round(p1.R) + 1;
    else if (p1.I > p1.R)
      p1.I = std::round(p1.I) + 1;
    else
      p1.R = std::round(p1.R) + 1;
  }

  p1.S = std::round(p1.S);
  p1.I = std::round(p1.I);
  p1.R = std::round(p1.R);

  return p1;
}

bool Pandemic::isEnded() const {
  if (round().I == 0)
    return true;
  else if (Y == 0 && round().S == 0)
    return true;
  else
    return false;
}

void Pandemic::roundPrint(unsigned int d) const {
  Population p1;
  p1 = round();
  std::cout << d << '\t' << p1.S << '\t' << p1.I << '\t' << p1.R << std::endl;
}

void Pandemic::evolve() {
  double x = 0;
  x = p.S;
  p.S = x * (1 - (B / N) * p.I);
  p.R = p.R + p.I * Y;
  p.I = p.I * (1 + (B / N) * x - Y);
}