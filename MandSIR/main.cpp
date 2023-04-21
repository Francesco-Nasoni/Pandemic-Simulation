#include "epidemic.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>

int main(int argc, char **argv) {

  Epidemic sample;
  try {
    if (argc != 5) {
      throw std::runtime_error("Invalid number of parameters");
    }

    std::vector<double> passed_parameters;

    for (int i = 1; i < argc; i++) {
      std::string row(argv[i]);
      if (row.find_first_not_of("-1234567890.") != std::string::npos ||
          (row.find_last_of("-") != 0 &&
           row.find_last_of("-") != std::string::npos)) {
        throw std::runtime_error{"Invalid alphanumeric variables"};
      }
      passed_parameters.push_back(stod(row));
    }
    sample = Epidemic(passed_parameters[0], passed_parameters[1],
                      passed_parameters[2], passed_parameters[3]);
  } catch (std::runtime_error &e) {
    std::cerr << "ERROR: " << e.what() << '\n';
    exit(0);
  }

  std::cout << std::endl
            << "Day" << '\t' << "Susceptible" << '\t' << "Infected" << '\t'
            << "Removed" << '\n';
  sample.round_print(0);

  for (int i = 0; !sample.is_ended(); i++) {
    sample.evolve();
    sample.round_print(i);
  }
}