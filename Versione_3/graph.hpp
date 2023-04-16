#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "SFML/Graphics.hpp"
#include "axis.hpp"
#include <cmath>

#define WIDTH 2
#define N_DIV 5
#define y_ax_y 10 // coordinata y del punto di origine dell'asse y
#define TOP_MARGIN 10
#define BOTTOM_MARGIN 35
#define LEFT_MARGIN 10

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
  double r_margin;
  double l_margin;

  void set_RL_margin(double m_x, double m_y);

public:
  Graph(sf::RenderWindow &, double max_x, double max_y, int n);
  double get_max_x() const;
  double get_max_y() const;
  void resize(double, double);
  void add_point(sf::Vector2f, int i);
  void set_color(sf::Color, int i);
  void draw();
};

#endif