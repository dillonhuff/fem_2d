#pragma once

#include <vector>

#include "constraint.h"
#include "material.h"
#include "matrices.h"
#include "trimesh.h"
#include "vec2.h"

namespace fem_2d {

  std::vector<ublas::vector<double> >
  compute_stresses(const trimesh& mesh,
		   const std::vector<constraint2>& constraints,
		   const std::vector<vec2>& displacements,
		   const material_properties& material);
  
}
