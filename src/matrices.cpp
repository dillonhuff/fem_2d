#include "matrices.h"

namespace fem_2d {

  ublas::matrix<double>
  build_element_B_matrix(const unsigned elem_ind,
			 const trimesh& mesh) {
    vertex_triangle t = mesh.tris[elem_ind];

    double y1 = mesh.verts[t.verts[0]].y();
    double y2 = mesh.verts[t.verts[1]].y();
    double y3 = mesh.verts[t.verts[2]].y();

    double y23 = y2 - y3;
    double y31 = y3 - y1;
    double y12 = y1 - y2;

    double x1 = mesh.verts[t.verts[0]].x();
    double x2 = mesh.verts[t.verts[1]].x();
    double x3 = mesh.verts[t.verts[2]].x();

    double x32 = x3 - x2;
    double x13 = x1 - x3;
    double x21 = x2 - x1;

    //double y23 = y2 - y3;

    double y13 = y1 - y3;
    double x23 = x2 - x3;    

    ublas::matrix<double> B = ublas::zero_matrix<double>(3, 6);
    B(0, 0) = y23;
    B(0, 1) = 0;
    B(0, 2) = y31;
    B(0, 3) = 0;
    B(0, 4) = y12;
    B(0, 5) = 0;

    B(1, 0) = 0;
    B(1, 1) = x32;
    B(1, 2) = 0;
    B(1, 3) = x13;
    B(1, 4) = 0;
    B(1, 5) = x21;

    B(2, 0) = x32;
    B(2, 1) = y23;
    B(2, 2) = x13;
    B(2, 3) = y31;
    B(2, 4) = x21;
    B(2, 5) = y12;

    double detJ = x13*y23 - y13*x23;

    B = (1.0 / detJ) * B;
    
    return B;
  }

  ublas::matrix<double>
  build_D_matrix(double const youngs_modulus,
		 double const nu) {

    ublas::matrix<double> D = ublas::zero_matrix<double>(3, 3);

    D(0, 0) = 1;
    D(0, 1) = nu;
    D(0, 2) = 0;

    D(1, 0) = nu;
    D(1, 1) = 1;
    D(1, 2) = 0;

    D(2, 0) = 0;
    D(2, 1) = 0;
    D(2, 2) = (1 - nu) / 2;

    D = (youngs_modulus / (1 - nu*nu)) * D;
    return D;
  }

  
}
