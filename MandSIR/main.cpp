#include "epidemic.hpp"
#include <iostream>
#include <vector>
#include <stdexcept>

int main(int n_arg, char** arg) {

  if( n_arg != 5)
    throw std::runtime_error("Invalid number of parameters");

  std::vector<double> var;

  for (int i = 1; i < n_arg; i++)
  {
    std::string row(arg[i]);
    if (row.find_first_not_of("1234567890.") != std::string::npos){
        throw std::runtime_error{"Invalid variables"};
      }
    var.push_back(stod(row));
  }
  
  Epidemic sample{var[2], var[3], var[0], var[1]};

  std::cout << std::endl
            << "Giorno" << '\t' << "Sani" << '\t' << "Infetti" << '\t'
            << "Rimossi" << '\n';
  sample.round_print(0);

  int i = 1;
  while (!sample.is_ended()) {
    sample.evolve();
    sample.round_print(i);
    i++;
  }
}