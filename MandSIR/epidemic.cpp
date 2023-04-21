#include "epidemic.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

Epidemic::Epidemic(double s, double i, double b, double y)
    : B{b}, Y{y}, N{s + i}, pop{s, i, 0} {
  if (N == 0 && Y == 0 && B == 0)
    throw std::runtime_error{"Can't perform simulation with all zeros"};
  if (Y == 0 && B == 0)
    throw std::runtime_error{"With B=0 and Y=0 nothing will happen"};
  if (s < 0 || i < 0)
    throw std::runtime_error{"S<0 or I<0 is nonsensical"};
  if (B < 0 || B > 1 || Y < 0 || Y > 1)
    throw std::runtime_error{"B and Y are probabilities thus they "
                             "must be given in the range [0,1]"};
}

Population Epidemic::round() const {
  std::vector<double> pop_values{pop.S, pop.I, pop.R};
  std::vector<double> decimal_parts{pop.S - floor(pop.S), pop.I - floor(pop.I),
                                    pop.R - floor(pop.R)};

  if (std::round(pop.S) + std::round(pop.I) + std::round(pop.R) < N) {
    auto max = std::max_element(decimal_parts.begin(), decimal_parts.end());
    int index = std::distance(decimal_parts.begin(), max);
    pop_values[index] = ceil(pop_values[index]);
  } else if (std::round(pop.S) + std::round(pop.I) + std::round(pop.R) > N) {
    auto min = std::min_element(decimal_parts.begin(), decimal_parts.end());
    int index = std::distance(decimal_parts.begin(), min);
    pop_values[index] = floor(pop_values[index]);
  }

  for (double &v : pop_values) {
    v = std::round(v);
  }

  return Population{pop_values[0], pop_values[1], pop_values[2]};
}

bool Epidemic::is_ended() const {
  if (round().I == 0)
    return true;
  else if (Y == 0 && round().S == 0)
    return true;
  else
    return false;
}

void Epidemic::round_print(int d) const {
  Population p1;
  p1 = round();
  std::cout << d << '\t' << p1.S << '\t' << p1.I << '\t' << p1.R << std::endl;
}

void Epidemic::evolve() {
  double x = 0;
  x = pop.S;
  pop.S = x * (1 - (B / N) * pop.I);
  pop.R = pop.R + pop.I * Y;
  pop.I = pop.I * (1 + (B / N) * x - Y);
}
