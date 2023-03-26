#ifndef AXIS_HPP
#define AXIS_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <cassert>

enum class Direction : bool { x, y };

class Axis {
 private:
  sf::RectangleShape line;
  sf::Color color;
  std::vector<sf::Text> label;
  std::vector<sf::RectangleShape> div;
  sf::Font font;
  double x;
  double y;
  double max;
  double length;
  int nDiv;
  Direction dir;

 public:
  Axis(double max, double lenght, double width, int nDiv, sf::Color, double x, double y, Direction);
  Axis();
  static double getOffSetX(std::string, double);
  static double getOffSetY(std::string, double);
  void draw(sf::RenderWindow&);
};

#endif