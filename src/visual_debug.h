#pragma once

#include "matrices.h"
#include "trimesh.h"

namespace fem_2d {

  void visualize_mesh(const trimesh& mesh);

  void visualize_stresses(const trimesh& mesh,
			  const std::vector<ublas::vector<double>>& displacements);

}
