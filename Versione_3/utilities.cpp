#include "utilities.hpp"

void ut::configuration(Pandemic &p1, PandemicCM &p2, int &p_s, bool &a_m,
                       double &q_t, double &q_g, int &q_m_n, int &v_1_t,
                       int &v_2_t) {
  std::ifstream file("config.txt");
  std::string line;
  std::vector<double> val;
  while (std::getline(file, line)) {
    std::string row;
    std::size_t pos = line.find_last_of('=');
    if ((line.find_first_of('#') != line.find_first_not_of("\t ") ||
         line.find_first_of('#') == std::string::npos) &&
        pos != std::string::npos) {
      row = line.substr(pos + 2);

      if (row.find_first_not_of("1234567890.") != std::string::npos)
        throw std::runtime_error{"Invalid variables in config.txt"};

      val.push_back(stod(row));
    }
  }
  if (val.size() != 15)
    throw std::runtime_error{"Invalid nuber of senttings in config.txt"};

  int num = static_cast<int>(std::round(val[0]));
  int r_num = num % 10;
  num = num + r_num;
  p_s = num;
  Pandemic c1(num, val[1], val[2], val[3], val[4], val[5]);
  PandemicCM c2(c1, val[12], val[13], val[14]);

  if (val[6] > 0)
    a_m = true;
  else
    a_m = false;

  q_t = val[7];
  q_g = val[8];
  q_m_n = static_cast<int>(std::round(val[9]));
  v_1_t = static_cast<int>(std::round(val[10]));
  v_2_t = static_cast<int>(std::round(val[11]));

  p1 = c1;
  p2 = c2;
}

void ut::render(sf::RenderWindow &window, Graph &graph) {
  window.clear(sf::Color::White);
  graph.draw();
  window.display();
}

void ut::add_point(Graph &graph, Pandemic const &p, int d) {
  double m_x = graph.get_max_x();
  double m_y = graph.get_max_y();
  if (d > m_x) {
    graph.resize(m_x + 50, m_y);
  }
  graph.add_point(sf::Vector2f(d, p.get_susceptible()), 1);
  graph.add_point(sf::Vector2f(d, p.get_infected()), 2);
  graph.add_point(sf::Vector2f(d, p.get_dead()), 3);
}

void ut::proces_event(sf::RenderWindow &window, sf ::Event const &event,
                      PandemicCM &p, bool auto_mode) {
  if (event.type == sf::Event::KeyPressed) {
    if (!auto_mode) {
      switch (event.key.code) {
      case sf::Keyboard::Q:
        p.toggle_quar();
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

void ut::write(std::ostream &os, Pandemic const &sample, int d) {
  os << d << '\t' << sample.get_susceptible() << '\t' << sample.get_infected()
     << '\t' << sample.get_dead() << '\n';
}

void ut::print(Pandemic const &s1, Pandemic const &s2, int d1, int d2,
               int q_c) {
  double ISI_1 = static_cast<double>(s1.get_infected()) /
                 static_cast<double>(s1.get_susceptible() + s1.get_infected());
  double ISI_2 = static_cast<double>(s2.get_infected()) /
                 static_cast<double>(s2.get_susceptible() + s2.get_infected());

  constexpr auto clear = "\033[2J";
  std::cout << clear;
  std::cout << std::fixed << std::setprecision(3) << SPACE(11) << "Sim. w/o CM"
            << SPACE(16) << "" << SPACE(11) << "Sim. w CM" << std::endl
            << SPACE(8) << "Day: " << SPACE(8) << d1 << SPACE(11) << ""
            << SPACE(8) << "Day: " << SPACE(8) << d2 << std::endl
            << SPACE(8) << "S: " << SPACE(8) << s1.get_susceptible()
            << SPACE(11) << " "
            << SPACE(8) << "S: " << SPACE(8) << s2.get_susceptible() << std::endl
            << SPACE(8) << "I: " << SPACE(8) << s1.get_infected() << SPACE(11)
            << " " << SPACE(8) << "I: " << s2.get_infected() << std::endl
            << SPACE(8) << "D: " << SPACE(8) << s1.get_dead() << SPACE(11)
            << " " << SPACE(8) << "D: " << s2.get_dead() << std::endl
            << SPACE(8) << "I/S+I: " << SPACE(8) << ISI_1 << SPACE(11) << " "
            << SPACE(8) << "I/S+I: " << SPACE(8) << ISI_2 << std::endl
            << SPACE(8) << " " << SPACE(8) << " " << SPACE(11) << " "
            << SPACE(8) << "quar_c: " << SPACE(8) << q_c << std::endl;
}