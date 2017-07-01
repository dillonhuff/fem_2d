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

    auto D = build_D_matrix();
    for (int i = 0; i < mesh.tris.size(); i++) {
      ublas::vector<double> v(6);

      vertex_triangle t = mesh.tris[i];
      v(0) = mesh.verts[t.verts[0]].x();
      v(1) = mesh.verts[t.verts[0]].y();
      v(2) = mesh.verts[t.verts[1]].x();
      v(3) = mesh.verts[t.verts[1]].y();
      v(4) = mesh.verts[t.verts[2]].x();
      v(5) = mesh.verts[t.verts[2]].y();

      // cout << 2*t.verts[0] << endl;
      // cout << 2*t.verts[0] + 1 << endl;

      // cout << 2*t.verts[1] << endl;
      // cout << 2*t.verts[1] + 1 << endl;

      // cout << 2*t.verts[2] << endl;
      // cout << 2*t.verts[2] + 1 << endl;
      auto B = build_element_B_matrix(i, mesh);

      auto db = prod(D, B);

      cout << db << endl;

      auto stress = prod(db, v);
      cout << "stress = " << stress << endl;
    }
    return {};
  }
  
}
