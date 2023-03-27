#include "utilities.hpp"

void ut::render(sf::RenderWindow &window, Graph &graph) {
  window.clear(sf::Color::White);
  graph.draw();
  window.display();
}

void ut::addPoint(Graph &graph, Pandemic const &p, int d) {
  double m_x = graph.getMax_x();
  double m_y = graph.getMax_y();
  if (d > m_x) {
    graph.resize(m_x + 50, m_y);
  }
  graph.addPoint(sf::Vector2f(d, p.getSusceptible()), 1);
  graph.addPoint(sf::Vector2f(d, p.getInfected()), 2);
  graph.addPoint(sf::Vector2f(d, p.getDead()), 3);
}

void ut::procesEvent(sf::RenderWindow &window, sf ::Event const &event) {
  if (event.type == sf::Event::KeyPressed) {
    if (event.key.code == sf::Keyboard::Escape)
      window.close();
  } else if (event.type == sf::Event::Closed)
    window.close();
}

void ut::write(std::ostream &os, Pandemic const &sample, int d) {
  os << d << '\t' << sample.getSusceptible() << '\t' << sample.getInfected()
     << '\t' << sample.getDead() << '\n';
}

void ut::print(Pandemic const &s1, Pandemic const &s2, int d1, int d2) {
  double SI_1 = static_cast<double>(s1.getInfected()) /
                static_cast<double>(s1.getSusceptible());
  double SI_2 = static_cast<double>(s2.getInfected()) /
                static_cast<double>(s2.getSusceptible());
  double IDN_1 = static_cast<double>(s1.getInfected() + s1.getDead()) /
                 static_cast<double>(s1.getPeopleN());
  double IDN_2 = static_cast<double>(s2.getInfected() + s2.getDead()) /
                 static_cast<double>(s2.getPeopleN());

  constexpr auto clear = "\033[2J";
  std::cout << clear;
  std::cout << FIXED_FLOAT(3) << SPACE(6) << "Day: " << SPACE(6) << d1
            << SPACE(6) << " " << SPACE(6) << "Day: " << SPACE(6) << d2
            << std::endl
            << SPACE(6) << "S: " << SPACE(6) << s1.getSusceptible() << SPACE(6)
            << " " << SPACE(6) << "S: " << SPACE(6) << s2.getSusceptible()
            << std::endl
            << SPACE(6) << "I: " << SPACE(6) << s1.getInfected() << SPACE(6)
            << " " << SPACE(6) << "I: " << s2.getInfected() << std::endl
            << SPACE(6) << "D: " << SPACE(6) << s1.getDead() << SPACE(6) << " "
            << SPACE(6) << "D: " << s2.getDead() << std::endl
            << SPACE(6) << "S/I: " << SPACE(6) << SI_1 << SPACE(6) << " "
            << SPACE(6) << "S/I: " << SI_2 << std::endl
            << SPACE(6) << "ID/N: " << SPACE(6) << IDN_1 << SPACE(6) << " "
            << SPACE(6) << "ID/N: " << IDN_2 << std::endl;
}