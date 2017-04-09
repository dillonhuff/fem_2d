#pragma once

#include "constraint.h"
#include "trimesh.h"

namespace fem_2d {

  std::vector<vec2>
  compute_displacements(const trimesh& mesh,
			const std::vector<constraint2>& constraints,
			const std::vector<vec2>& forces);

}
