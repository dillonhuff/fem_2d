#include "stresses.h"

#include "matrices.h"

using namespace std;

namespace fem_2d {

  ublas::matrix<double> 
  build_d_matrix(const std::vector<vec2>& displacements) {
    return {};
  }
  
  std::vector<double>
  compute_stresses(const trimesh& mesh,
		   const std::vector<constraint2>& constraints,
		   const std::vector<vec2>& displacements) {
    ublas::matrix<double> d_matrix =
      build_d_matrix(displacements);

    for (int i = 0; i < mesh.tris.size(); i++) {
      vertex_triangle t = mesh.tris[i];
      cout << 2*t.verts[0] << endl;
      cout << 2*t.verts[0] + 1 << endl;

      cout << 2*t.verts[1] << endl;
      cout << 2*t.verts[1] + 1 << endl;

      cout << 2*t.verts[2] << endl;
      cout << 2*t.verts[2] + 1 << endl;
      
    }
    return {};
  }
  
}
