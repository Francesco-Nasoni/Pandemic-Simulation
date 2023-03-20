#include "graph.hpp"
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <thread>

#include "pandemicCM.hpp"

#define FIXED_FLOAT(x) std::fixed << std::setprecision(x)
#define SPACE(x) std::left << std::setw(x)

void render(sf::RenderWindow &window, Graph &graph) {
  window.clear(sf::Color::White);
  graph.draw();
  window.display();
}

void addPoint(Graph &graph, Pandemic const &p, int d) {
  double m_x = graph.getMax_x();
  double m_y = graph.getMax_y();
  if (d > m_x) {
    graph.resize(m_x + 50, m_y);
  }
  graph.addPoint(sf::Vector2f(d, p.getSusceptible()), 1);
  graph.addPoint(sf::Vector2f(d, p.getInfected()), 2);
  graph.addPoint(sf::Vector2f(d, p.getDead()), 3);
}

void procesEvent(sf::RenderWindow &window, sf ::Event const &event) {
  if (event.type == sf::Event::KeyPressed) {
    if (event.key.code == sf::Keyboard::Escape)
      window.close();
  } else if (event.type == sf::Event::Closed)
    window.close();
}

void write(std::ostream &os, Pandemic const &sample, int d) {
  os << d << '\t' << sample.getSusceptible() << '\t' << sample.getInfected()
     << '\t' << sample.getDead() << '\n';
}

void print(Pandemic const &s1, Pandemic const &s2, int d1, int d2) {
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

int main() {
  int population_size = 10000;
  double sociality = 4;
  double initial_infected = 0.5 / 100;
  double B = 0.05;
  double Y = 0.1;
  double R = 0.007;
  double Musk_trigger = 0.2;
  double Quar_trigger = 0.13;
  int Vacc_trigger = 0;

  std::ofstream file{"Data/Result.txt", std::ofstream::trunc};
  std::ofstream fileCM{"Data/ResultCM.txt", std::ofstream::trunc};

  auto desktop = sf::VideoMode::getDesktopMode();
  sf::RenderWindow window{
      sf::VideoMode(desktop.width / 2, desktop.height / 2 - 20),
      "Pandemic Similation", sf::Style::Close};
  sf::RenderWindow window_CM{
      sf::VideoMode(desktop.width / 2, desktop.height / 2 - 20),
      "Pandemic Similation with CM", sf::Style::Close};
  window.setPosition(sf::Vector2i(0, 0));
  window_CM.setPosition(sf::Vector2i(desktop.width / 2, 0));

  Pandemic sample(population_size, B, Y, R, sociality, initial_infected);
  PandemicCM sample_CM(sample, Musk_trigger, Quar_trigger, Vacc_trigger);
  sample_CM.setMusk_B_effect(0.8);
  sample_CM.setVacc_B_effect(0.8);
  sample_CM.setVacc_Y_effect(1.2);
  sample_CM.setVacc_R_effect(0.80);
  sample_CM.setQuar_goal(0.04);

  Graph graph(window, 50., population_size, 3);
  graph.setColor(sf::Color::Red, 2);
  graph.setColor(sf::Color::Green, 3);

  Graph graph_CM(window_CM, 50., population_size, 3);
  graph_CM.setColor(sf::Color::Red, 2);
  graph_CM.setColor(sf::Color::Green, 3);

  int day = 0;
  int day_CM = 0;

  write(file, sample, 0);
  write(fileCM, sample, 0);
  print(sample, sample_CM, 0, 0);

  while (window.isOpen() || window_CM.isOpen() || sample.getInfected() > 0 ||
         sample_CM.getInfected() > 0) {

    sf::Event event;
    while (window.pollEvent(event))
      procesEvent(window, event);

    while (window_CM.pollEvent(event))
      procesEvent(window_CM, event);

    if (sample.getInfected() > 0) {
      day++;
      sample = sample.evolve();
      write(file, sample, day);
    }
    if (sample_CM.getInfected() > 0) {
      day_CM++;
      sample_CM = sample_CM.evolveCM(day_CM);
      write(fileCM, sample_CM, day_CM);
    }
    print(sample, sample_CM, day, day_CM);

    addPoint(graph, sample, day);
    addPoint(graph_CM, sample_CM, day_CM);

    render(window, graph);
    render(window_CM, graph_CM);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  std::cout << '\n';
};