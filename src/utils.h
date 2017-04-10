#pragma once

#include <cmath>

namespace fem_2d {

  static inline bool within_eps(const double x, const double y, const double tol) {
    double diff = fabs(x - y);
    return diff < tol;
  }

}
