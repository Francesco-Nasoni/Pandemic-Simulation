#include "epidemic.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>

int main(int argc, char **argv) {

  try {
    if (argc != 5)
      throw std::runtime_error("Invalid number of parameters");
  } catch (std::runtime_error &e) {
    std::cerr << e.what() << '\n';
    exit(0);
  }
  std::vector<double> var;

  for (int i = 1; i < argc; i++) {
    std::string row(argv[i]);
    if (row.find_first_not_of("1234567890.") != std::string::npos) {
      throw std::runtime_error{"Invalid variables"};
    }
    var.push_back(stod(row));
  }

  Epidemic sample{var[2], var[3], var[0], var[1]};

  std::cout << std::endl
            << "Giorno" << '\t' << "Sani" << '\t' << "Infetti" << '\t'
            << "Rimossi" << '\n';
  sample.round_print(0);

  for (int i = 0; !sample.is_ended(); i++) {
    sample.evolve();
    sample.round_print(i);
  }
}