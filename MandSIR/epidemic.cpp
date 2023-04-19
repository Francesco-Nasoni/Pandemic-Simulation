#include "epidemic.hpp"
#include <cmath>
#include <iostream>

Epidemic::Epidemic(double b, double y, double s, double i)
    : B{b}, Y{y}, N{s + i}, p{s, i, 0} {
  if (N == 0 && Y == 0 && B == 0)
    throw std::runtime_error{"Can't perform simulation with all zeros"};
  if (Y == 0 && B == 0)
    throw std::runtime_error{"With B=0 and Y=0 nothing will happen"};
  if (s < 0 || i < 0)
    throw std::runtime_error{"S<0 or I<0 is nonsensical"};
  if (B < 0 || Y < 0)
    throw std::runtime_error{"B<0 and Y<0 is nonsensical"};
}

Population Epidemic::round() const {
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

bool Epidemic::is_ended() const {
  if (round().I == 0)
    return true;
  else if (Y == 0 && round().S == 0)
    return true;
  else
    return false;
}

void Epidemic::round_print(unsigned int d) const {
  Population p1;
  p1 = round();
  std::cout << d << '\t' << p1.S << '\t' << p1.I << '\t' << p1.R << std::endl;
}

void Epidemic::evolve() {
  double x = 0;
  x = p.S;
  p.S = x * (1 - (B / N) * p.I);
  p.R = p.R + p.I * Y;
  p.I = p.I * (1 + (B / N) * x - Y);
}