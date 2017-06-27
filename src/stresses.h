#pragma once

#include <vector>

#include "vec2.h"

namespace fem_2d {

  std::vector<double>
  compute_stresses(const std::vector<vec2>& displacements);

}
