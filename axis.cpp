#include "axis.hpp"
#include <iostream>
#include <cassert>

Axis::Axis(double max, double length, double width, int nDiv, sf::Color color,
           double x, double y, Direction d)
    :color{color}, x{x}, y{y}, max{max}, length{length}, nDiv{nDiv}, dir{d} {
  font.loadFromFile("arial.ttf");

  line.setSize(sf::Vector2f(length, width));
  line.setPosition(sf::Vector2f(x, y));
  line.setFillColor(color);

  div.reserve(nDiv);

  for (int i = 0; i <= nDiv; i++) {
    sf::Text tex;
    sf::RectangleShape rec;
    std::string str = std::to_string(i * max / nDiv);
    str.erase(str.find_last_not_of('0') + 1, std::string::npos);
    str.erase(str.find_last_not_of('.') + 1, std::string::npos);

    double l;
    sf::Vector2f rec_dim;
    sf::Vector2f rec_pos;
    sf::Vector2f lab_pos;

    if (dir == Direction::x) {
      l = length / static_cast<double>(nDiv);
      rec_dim = sf::Vector2f(width, 7 * width);
      rec_pos = sf::Vector2f( i * l + x , y -3 * width);
      lab_pos = sf::Vector2f(i * l + x - getOffSetX(str, 16), y + 10);
    } else {
      l = width / static_cast<double>(nDiv);
      rec_dim = sf::Vector2f(7 * length, length);
      rec_pos = sf::Vector2f(x - 3 * length,  -i * l + y + width);
      lab_pos =
          sf::Vector2f(x - 10 - 3*length - getOffSetY(str, 16.), -i * l + y + width - 8);
    }

    rec.setSize(rec_dim);
    rec.setPosition(rec_pos);
    rec.setFillColor(color);

    tex.setString(str);
    tex.setCharacterSize(16);
    tex.setPosition(lab_pos);
    tex.setFillColor(color);

    div.push_back(rec);
    label.push_back(tex);
  }
}

Axis::Axis(){}

double Axis::getOffSetX(std::string s, double f) {
  return s.length() * f * 0.25;
}

double Axis::getOffSetY(std::string s, double f) { return s.length() * f * 0.5; }

void Axis::draw(sf::RenderWindow& window) {
  window.draw(line);
  for (auto& v:label ) 
  {
    v.setFont(font);
    window.draw(v);
  }
  for (auto& v : div) window.draw(v);
}
