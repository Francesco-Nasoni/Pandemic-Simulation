#include "utilities.hpp"

Pandemic ut::configuration() {
  std::vector<double> val;
  std::ifstream file("config.txt");
  for (;;) {
    std::string line;
    if (!std::getline(file, line))
      break;

    std::string row;
    std::size_t pos = line.find_last_of('=');
    if ((line.find_first_of('#') != line.find_first_not_of("\t ") ||
         line.find_first_of('#') == std::string::npos) &&
        pos != std::string::npos) {
      row = line.substr(pos + 2);

      if (row.find_first_not_of("1234567890.") != std::string::npos)
        throw std::runtime_error{"Invalid setting.txt"};

      val.push_back(stod(row));
      std::cout << val.back() << '\n';
    }
  }

  // Pandemic p;

  return ;
}

void ut::render(sf::RenderWindow &window, Graph &graph) {
  window.clear(sf::Color::White);
  graph.draw();
  window.display();
}

void ut::add_point(Graph &graph, Pandemic const &p, int d) {
  double m_x = graph.getMax_x();
  double m_y = graph.getMax_y();
  if (d > m_x) {
    graph.resize(m_x + 50, m_y);
  }
  graph.addPoint(sf::Vector2f(d, p.get_susceptible()), 1);
  graph.addPoint(sf::Vector2f(d, p.get_infected()), 2);
  graph.addPoint(sf::Vector2f(d, p.get_dead()), 3);
}

void ut::proces_event(sf::RenderWindow &window, sf ::Event const &event) {
  if (event.type == sf::Event::KeyPressed) {
    if (event.key.code == sf::Keyboard::Escape)
      window.close();
  } else if (event.type == sf::Event::Closed)
    window.close();
}

void ut::write(std::ostream &os, Pandemic const &sample, int d) {
  os << d << '\t' << sample.get_susceptible() << '\t' << sample.get_infected()
     << '\t' << sample.get_dead() << '\n';
}

void ut::print(Pandemic const &s1, Pandemic const &s2, int d1, int d2) {
  double SI_1 = static_cast<double>(s1.get_infected()) /
                static_cast<double>(s1.get_susceptible());
  double SI_2 = static_cast<double>(s2.get_infected()) /
                static_cast<double>(s2.get_susceptible());
  double IDN_1 = static_cast<double>(s1.get_infected() + s1.get_dead()) /
                 static_cast<double>(s1.get_N());
  double IDN_2 = static_cast<double>(s2.get_infected() + s2.get_dead()) /
                 static_cast<double>(s2.get_N());

  constexpr auto clear = "\033[2J";
  std::cout << clear;
  std::cout << FIXED_FLOAT(3) << SPACE(6) << "Day: " << SPACE(6) << d1
            << SPACE(6) << " " << SPACE(6) << "Day: " << SPACE(6) << d2
            << std::endl
            << SPACE(6) << "S: " << SPACE(6) << s1.get_susceptible() << SPACE(6)
            << " " << SPACE(6) << "S: " << SPACE(6) << s2.get_susceptible()
            << std::endl
            << SPACE(6) << "I: " << SPACE(6) << s1.get_infected() << SPACE(6)
            << " " << SPACE(6) << "I: " << s2.get_infected() << std::endl
            << SPACE(6) << "D: " << SPACE(6) << s1.get_dead() << SPACE(6) << " "
            << SPACE(6) << "D: " << s2.get_dead() << std::endl
            << SPACE(6) << "S/I: " << SPACE(6) << SI_1 << SPACE(6) << " "
            << SPACE(6) << "S/I: " << SI_2 << std::endl
            << SPACE(6) << "ID/N: " << SPACE(6) << IDN_1 << SPACE(6) << " "
            << SPACE(6) << "ID/N: " << IDN_2 << std::endl;
}