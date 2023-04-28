#include "epidemic.hpp"
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <vector>

void print(Population const p, int d) {
  std::cout << std::left << std::setw(8) << d << std::left << std::setw(12)
            << p.S << std::left << std::setw(12) << p.I << std::left
            << std::setw(12) << p.R << std::endl;
}

void write_on_file(std::ostream &os, Population const p, int d) {
  os << d << '\t' << p.S << '\t' << p.I << '\t' << p.R << '\n';
}

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
           row.find_last_of("-") != std::string::npos) ||
          row == "-") {
        throw std::runtime_error{"Invalid alphanumeric variables"};
      }
      passed_parameters.push_back(stod(row));
    }
    sample = Epidemic(std::round(passed_parameters[0]),
                      std::round(passed_parameters[1]), passed_parameters[2],
                      passed_parameters[3]);
  } catch (std::runtime_error &e) {
    std::cerr << "ERROR: " << e.what() << '\n';
    exit(0);
  }

  std::ofstream file{"Result.txt", std::ofstream::trunc};

  std::cout << std::endl
            << std::left << std::setw(8) << "Day" << std::left << std::setw(12)
            << "Susceptible" << std::left << std::setw(12) << "Infected"
            << std::left << std::setw(12) << "Removed" << std::endl;
  print(sample.round(), 0);
  write_on_file(file, sample.round(), 0);

  for (int i = 0; !sample.is_ended(); i++) {
    sample.evolve();
    print(sample.round(), i);
    write_on_file(file, sample.round(), 0);
  }
}