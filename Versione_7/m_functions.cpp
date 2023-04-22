#include "m_functions.hpp"

void mf::read_from_config(Pandemic &sample, PandemicCM &sample_CM,
                          int &pop_size, bool &auto_mode, double &quar_trigger,
                          double &quar_goal, int &quar_max_n,
                          int &vacc_1_trigger, int &vacc_2_trigger) {
  std::ifstream file("config.txt");
  if (!file) {
    throw std::runtime_error{"Impossible to open the file config.txt"};
  }
  std::string line;
  std::vector<double> passed_parameters;
  for (int n_line = 1; std::getline(file, line); n_line++) {
    std::string row;
    std::size_t pos = line.find_last_of('=');
    if ((line.find_first_of('#') != line.find_first_not_of("\t ") ||
         line.find_first_of('#') == std::string::npos) &&
        pos != std::string::npos) {
      row = line.substr(pos + 2);

      if (row.find_first_not_of("-1234567890.") != std::string::npos ||
          (row.find_last_of("-") != 0 &&
           row.find_last_of("-") != std::string::npos)) {
        throw std::runtime_error{"Invalid alphanumeric variables in line " +
                                 std::to_string(n_line) +
                                 " of config.txt, please fix it"};
      }

      passed_parameters.push_back(stod(row));
    }
  }
  if (passed_parameters.size() != 15) {
    throw std::runtime_error{"Invalid nuber of senttings in config.txt, they "
                             "should be 15, please fix it"};
  }

  int pop_size_input = static_cast<int>(std::round(passed_parameters[0]));
  int remainder = pop_size_input % 10;
  pop_size = pop_size_input - remainder;

  Pandemic p(pop_size, passed_parameters[1], passed_parameters[2],
             passed_parameters[3], passed_parameters[4], passed_parameters[5]);
  PandemicCM p_CM(p, passed_parameters[12], passed_parameters[13],
                  passed_parameters[14]);

  if (passed_parameters[6] > 0)
    auto_mode = true;
  else
    auto_mode = false;

  quar_trigger = passed_parameters[7];
  quar_goal = passed_parameters[8];
  quar_max_n = static_cast<int>(std::round(passed_parameters[9]));
  vacc_1_trigger = static_cast<int>(std::round(passed_parameters[10]));
  vacc_2_trigger = static_cast<int>(std::round(passed_parameters[11]));

  sample = p;
  sample_CM = p_CM;
}

void mf::render(sf::RenderWindow &window, Graph &graph) {
  window.clear(sf::Color::White);
  graph.draw();
  window.display();
}

void mf::add_point(Graph &graph, Pandemic const &p, int d) {
  double max_x = graph.get_max_x();
  double max_y = graph.get_max_y();
  if (d > max_x) {
    graph.resize(max_x + 50, max_y);
  }
  graph.add_point(sf::Vector2f(d, p.get_susceptible()), 1);
  graph.add_point(sf::Vector2f(d, p.get_infected()), 2);
  graph.add_point(sf::Vector2f(d, p.get_dead()), 3);
}

void mf::proces_event(sf::RenderWindow &window, sf ::Event const &event,
                      PandemicCM &p, bool auto_mode, int &quar_count) {
  if (event.type == sf::Event::KeyPressed) {
    if (!auto_mode) {
      switch (event.key.code) {
      case sf::Keyboard::Q:
        p.toggle_quar();
        if (p.get_quar()) {
          quar_count++;
        }
        break;
      case sf::Keyboard::V:
        p.toggle_vacc_1();
        break;
      case sf::Keyboard::B:
        p.toggle_vacc_2();
        break;
      default:
        break;
      }
    }
    if (event.key.code == sf::Keyboard::Escape)
      window.close();
  } else if (event.type == sf::Event::Closed)
    window.close();
}

void mf::write(std::ostream &os, Pandemic const &sample, int d) {
  os << d << '\t' << sample.get_susceptible() << '\t' << sample.get_infected()
     << '\t' << sample.get_dead() << '\n';
}

void mf::print(Pandemic const &s1, Pandemic const &s2, int d1, int d2,
               int q_c) {
  double ISI_1 = static_cast<double>(s1.get_infected()) /
                 static_cast<double>(s1.get_susceptible() + s1.get_infected());
  double ISI_2 = static_cast<double>(s2.get_infected()) /
                 static_cast<double>(s2.get_susceptible() + s2.get_infected());

  constexpr auto clear = "\033[2J";
  std::cout << clear;
  std::cout << std::fixed << std::setprecision(3) << SPACE(11) << "Sim. w/o CM"
            << SPACE(16) << "" << SPACE(11) << "Sim. w CM" << std::endl
            << SPACE(11) << "-----------" << SPACE(16) << "" << SPACE(11)
            << "-----------" << std::endl
            << SPACE(8) << "Day: " << SPACE(8) << d1 << SPACE(11) << ""
            << SPACE(8) << "Day: " << SPACE(8) << d2 << std::endl
            << SPACE(8) << "S: " << SPACE(8) << s1.get_susceptible()
            << SPACE(11) << " " << SPACE(8) << "S: " << SPACE(8)
            << s2.get_susceptible() << std::endl
            << SPACE(8) << "I: " << SPACE(8) << s1.get_infected() << SPACE(11)
            << " " << SPACE(8) << "I: " << s2.get_infected() << std::endl
            << SPACE(8) << "D: " << SPACE(8) << s1.get_dead() << SPACE(11)
            << " " << SPACE(8) << "D: " << s2.get_dead() << std::endl
            << SPACE(8) << "I/S+I: " << SPACE(8) << ISI_1 << SPACE(11) << " "
            << SPACE(8) << "I/S+I: " << SPACE(8) << ISI_2 << std::endl
            << SPACE(8) << " " << SPACE(8) << " " << SPACE(11) << " "
            << SPACE(8) << "quar_c: " << SPACE(8) << q_c << std::endl;
}