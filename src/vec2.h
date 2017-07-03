#pragma once

#include <iostream>

namespace fem_2d {

  class vec2 {
    double x_v, y_v;

  public:
    vec2(const double p_x, const double p_y) :
      x_v(p_x), y_v(p_y) {}

    double x() const { return x_v; }
    double y() const { return y_v; }
  };

  std::ostream& operator<<(std::ostream& out, const vec2 v);
  
}
