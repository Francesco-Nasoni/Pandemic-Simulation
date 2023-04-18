#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "SFML/Graphics.hpp"
#include "axis.hpp"
#include <cmath>

constexpr int c_margin_top = 10;
constexpr int c_margin_bottom = 35;
constexpr int c_margin_left = 10;

constexpr int c_axis_width = 2;
constexpr int c_axis_n_div = 5;
constexpr int c_y_ax_y = 10; // coordinata y del punto di origine dell'asse y

constexpr int c_legend_int_line = 5;
constexpr int c_legend_n_char = 11;
constexpr int c_legend_margin = 10;
constexpr int c_legend_square_l = 16;

class Graph {
private:
  sf::RenderWindow &window;
  std::vector<sf::VertexArray> point;
  std::vector<sf::Color> color;
  std::vector<sf::Text> leg_label;
  std::vector<sf::RectangleShape> leg_rec;
  sf::RectangleShape leg_box;
  sf::Font font;
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
  Graph(sf::RenderWindow &, double max_x, double max_y, std::vector<sf::Color> col, std::vector<std::string> tit);
  double get_max_x() const;
  double get_max_y() const;
  void resize(double, double);
  void add_point(sf::Vector2f, int i);
  void add_to_legend(std::string, int i);
  void draw();
};

#endif