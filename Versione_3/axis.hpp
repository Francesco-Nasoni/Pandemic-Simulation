#ifndef AXIS_HPP
#define AXIS_HPP

#include <SFML/Graphics.hpp>
#include <cassert>
#include <string>
#include <vector>

#define DIV_LENGTH 7 // times the width must be odd
#define FONT_SIZE 16
#define ARIAL_RATIO 0.57
#define LABEL_OFFSET 3

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
  Axis(double max, double lenght, double width, int nDiv, sf::Color, double x,
       double y, Direction);
  Axis();
  static double get_lab_offset(std::string, double);
  void draw(sf::RenderWindow &);
};

#endif