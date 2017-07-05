#include "stresses.h"

#include "matrices.h"

using namespace std;

namespace fem_2d {

  std::vector<ublas::vector<double> >
  compute_stresses(const trimesh& mesh,
		   const std::vector<constraint2>& constraints,
		   const std::vector<vec2>& displacements) {

    cout << "# of displacements = " << displacements.size() << endl;
    // Steel material properties
    double youngs_modulus = 30e6;
    double nu = 0.25;

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

      cout << "B matrix = " << endl;
      cout << B << endl;
      cout << endl;

      auto db = prod(D, B);

      cout << "d*b matrix = " << endl;
      cout << db << endl;

      auto stress = prod(db, v);
      cout << "stress = " << stress << endl;
      stresses.push_back(stress);
    }

    return stresses;
  }
  
}
