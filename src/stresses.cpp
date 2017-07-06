#include "stresses.h"

#include "matrices.h"

using namespace std;

namespace fem_2d {

  std::vector<ublas::vector<double> >
  compute_stresses(const trimesh& mesh,
		   const std::vector<constraint2>& constraints,
		   const std::vector<vec2>& displacements,
		   const material_properties& material) {

    double youngs_modulus = material.youngs_modulus;
    double nu = material.nu;

    auto D = build_D_matrix(youngs_modulus, nu);

    vector<ublas::vector<double> > stresses;
    for (int i = 0; i < mesh.tris.size(); i++) {
      ublas::vector<double> v(6);

      vertex_triangle t = mesh.tris[i];
      v(0) = displacements[t.verts[0]].x();
      v(1) = displacements[t.verts[0]].y();

      v(2) = displacements[t.verts[1]].x();
      v(3) = displacements[t.verts[1]].y();

      v(4) = displacements[t.verts[2]].x();
      v(5) = displacements[t.verts[2]].y();

      auto B = build_element_B_matrix(i, mesh);

      auto db = prod(D, B);

      auto stress = prod(db, v);
      stresses.push_back(stress);
    }

    return stresses;
  }
  
}
