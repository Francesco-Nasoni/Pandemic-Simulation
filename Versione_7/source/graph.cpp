#include "graph.hpp"
#include <iostream>

Graph::Graph(sf::RenderWindow &m_window, double max_x, double max_y,
             std::vector<sf::Color> colors, std::vector<std::string> titles)
    : window{m_window}, colors{colors}, max_x{max_x}, max_y{max_y}, x_exst{1},
      y_exst{1} {
  font.loadFromFile("arial.ttf");

  resize(max_x, max_y);
  for (long unsigned int i = 0; i < colors.size(); i++) {
    sf::VertexArray curve;
    points.push_back(curve);
    points[i].setPrimitiveType(sf::LineStrip);
    add_to_legend(titles[i], i);
  }

  leg_box.setFillColor(sf::Color::Transparent);
  leg_box.setOutlineThickness(1);
  leg_box.setOutlineColor(sf::Color::Black);
}

double Graph::get_max_x() const { return max_x; }

double Graph::get_max_y() const { return max_y; }

// This metod make the graph be properly resized
void Graph::resize(double new_max_x, double new_max_y) {
  sf::Vector2u size = window.getSize();
  // Store old left margin
  double o_l_margin = l_margin;

  // Set the margins to the new value
  this->set_RL_margin(new_max_x, new_max_y);

  // Calculate the ration between the old extension and the new extension of the
  // axis
  double ext_ratio_x =
      (static_cast<double>(size.x) - r_margin - l_margin) / x_exst;
  double ext_ration_y =
      (static_cast<double>(size.y) - c_margin_bottom - c_margin_top) / y_exst;

  // Set the extensions to the new values by subtracting the margin dimensions
  // to the window sizes
  x_exst = static_cast<double>(size.x) - r_margin - l_margin;
  y_exst = static_cast<double>(size.y) - c_margin_bottom - c_margin_top;

  // Calculate the x axis vertical coordinate
  double x_ax_y = static_cast<double>(size.y) - c_margin_bottom;

  // Create two new axis and assign them
  Axis x(new_max_x, x_exst, c_axis_width, c_axis_n_div, l_margin, x_ax_y,
         Direction::x);
  Axis y(new_max_y, y_exst, c_axis_width, c_axis_n_div, l_margin, c_y_ax_y,
         Direction::y);
  x_axis = x;
  y_axis = y;

  // Calculate the ratios by witch the points coordinate must
  // be multiplied to maintain the correct position relative to the axis
  double rel_ratio_x = max_x / new_max_x * ext_ratio_x;
  double rel_ratio_y = max_y / new_max_y * ext_ration_y;

  for (auto &v : points)
    for (long unsigned i = 0; i < v.getVertexCount(); i++) {
      // Calculate the old relative positions by subtracting the old margin
      // values
      double old_rel_x = v[i].position.x - o_l_margin;
      double old_rel_y = v[i].position.y -
                         static_cast<double>(window.getSize().y) +
                         c_margin_bottom;

      // Reassign both x and y coordinates by multiplying the old relative
      // positions times the raltive ratio and by adding the new margin values
      v[i].position.x = old_rel_x * rel_ratio_x + l_margin;
      v[i].position.y = old_rel_y * rel_ratio_y +
                        static_cast<double>(window.getSize().y) -
                        c_margin_bottom;
    }

  // Update the max_ and max_y attributes to the new value
  max_x = new_max_x;
  max_y = new_max_y;
}

// This metod is called only by Graph::resize and let the margins be properly
// resized
void Graph::set_RL_margin(double new_max_x, double new_max_y) {

  // Get the corrent string of the max value of the y axis
  std::string new_label_y = std::to_string(new_max_y);
  Axis::clear_string(new_label_y);

  // Calculate proper left margin by adding all the offset from the y-axis
  l_margin = Axis::get_font_length(new_label_y, c_font_size) + c_margin_left +
             (c_div_rect_length - 1) / 2 * c_axis_width + c_label_offset;

  // If the max value of y is an integer witch is not divisble by n_div, an
  // extra space for the decimal part must be added
  if (std::round(new_max_y) == new_max_y &&
      static_cast<int>(new_max_y) % c_axis_n_div != 0)
    l_margin = l_margin + c_font_size * c_arial_ratio;

  std::string new_label_x = std::to_string(new_max_x);
  Axis::clear_string(new_label_x);

  r_margin = Axis::get_font_length(new_label_x, c_font_size);
}

void Graph::add_point(sf::Vector2f new_point, int i) {
  // Calculate the factors to make the components of the new point proportional
  // to the axis extensions
  double scale_x = x_exst / max_x;
  double scale_y = y_exst / max_y;

  // Multiply each component by the factor just calculated and adding the
  // margins
  new_point.x = new_point.x * scale_x + l_margin;
  new_point.y = static_cast<double>(window.getSize().y) -
                new_point.y * scale_y - c_margin_bottom;

  // Add the point to the correct sf::VertexArray and setting the color
  points[i - 1].append(new_point);
  points[i - 1][points[i - 1].getVertexCount() - 1].color = colors[i - 1];
}

void Graph::add_to_legend(std::string str, int i) {

  // Set positions of the labels of the legend
  // X coordinate is fixed beacuse the length of the labels is fixed to
  // c_legend_n_char
  // Y coordinates increase when a new line is added
  sf::Vector2f lab_pos;
  lab_pos.x = window.getSize().x - c_legend_margin -
              c_legend_n_char * c_font_size * c_arial_ratio;
  lab_pos.y = c_legend_margin + i * (c_font_size + c_legend_int_line);

  // Calculate the position of the small colored rectangles that indicate the
  // right curve
  sf::Vector2f rec_pos;
  rec_pos.x = lab_pos.x - c_label_offset - c_legend_square_l;
  rec_pos.y = lab_pos.y;

  // Push in the vector the rectangles with fixed sizes and set their color and
  // coordinates
  leg_rec.push_back(
      sf::RectangleShape(sf::Vector2f(c_legend_square_l, c_legend_square_l)));
  leg_rec[i].setPosition(rec_pos);
  leg_rec[i].setFillColor(colors[i]);

  sf::Text tex;
  // Erase every character that exceeds c_legend_n_char
  if (str.size() > c_legend_n_char)
    str = str.erase(c_legend_n_char);

  tex.setString(str);
  tex.setCharacterSize(c_font_size);
  tex.setPosition(lab_pos);
  tex.setFillColor(sf::Color::Black);
  leg_label.push_back(tex);

  // Create the box thath surrounds the legend
  sf::Vector2f box_pos;
  sf::Vector2f box_size;

  box_pos.x = rec_pos.x - c_legend_margin / 2;
  box_pos.y = c_legend_margin / 2;

  // The extension on the x is fixed but the extension on the y increase with
  // the number of object inside the legend
  box_size.x = c_legend_margin + c_legend_n_char * c_font_size * c_arial_ratio;
  box_size.y = c_legend_margin + leg_label.size() * c_font_size +
               (leg_label.size() - 1) * c_legend_int_line;

  leg_box.setPosition(box_pos);
  leg_box.setSize(box_size);
}

void Graph::draw() {
  x_axis.draw(window);
  y_axis.draw(window);
  for (auto &v : points)
    window.draw(v);
  for (long unsigned int i = 0; i < leg_label.size(); i++) {
    leg_label[i].setFont(font);
    window.draw(leg_label[i]);
    window.draw(leg_rec[i]);
  }
  window.draw(leg_box);
}