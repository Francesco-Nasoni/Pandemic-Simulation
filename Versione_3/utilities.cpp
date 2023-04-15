#include "utilities.hpp"

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

void ut::proces_event(sf::RenderWindow &window, sf ::Event const &event, PandemicCM& p, bool auto_mode) {
  if (event.type == sf::Event::KeyPressed) {
    if(!auto_mode){
        switch (event.type)
        {
        case sf::Keyboard::Q:
            p.toggle_quar();
            break;
        case sf::Keyboard::V:
            p.toggle_vacc_1();
            
        default:
            break;
        }
    }

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