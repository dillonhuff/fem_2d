#pragma once

#include <vector>

#include "constraint.h"
#include "trimesh.h"
#include "vec2.h"

namespace fem_2d {

  std::vector<double>
  compute_stresses(const trimesh& mesh,
		   const std::vector<constraint2>& constraints,
		   const std::vector<vec2>& displacements);
  
}
