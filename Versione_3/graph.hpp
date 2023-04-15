#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "SFML/Graphics.hpp"
#include "axis.hpp"
#include <cmath>

class Graph {
private:
  sf::RenderWindow &window;
  std::vector<sf::VertexArray> point;
  std::vector<sf::Color> color;
  Axis x_axis;
  Axis y_axis;

  double max_x;
  double max_y;
  double x_exst;
  double y_exst;
  int n;
  double width = 2;
  double r_margin;
  double l_margin;

public:
  Graph(sf::RenderWindow &, double max_x, double max_y, int n);
  double getMax_x();
  double getMax_y();
  void resize(double, double);
  void addPoint(sf::Vector2f, int i);
  void setColor(sf::Color, int i);
  void set_RL_Margin(double m_x, double m_y);
  void draw();
};

#endif