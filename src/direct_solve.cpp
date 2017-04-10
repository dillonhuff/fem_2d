#include "direct_solve.h"

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/io.hpp>

namespace ublas = boost::numeric::ublas;

using namespace std;

namespace fem_2d {

  boost::numeric::ublas::vector<double>
  to_vector(const std::vector<vec2> pts) {
    ublas::vector<double> v(2*pts.size());

    for (unsigned i = 0; i < pts.size(); i++) {
      v(2*i) = pts[i].x();
      v(2*i + 1) = pts[i].y();
    }

    return v;
  }

  std::vector<vec2>
  from_vector(const ublas::vector<double>& pts) {
    vector<vec2> v;

    for (unsigned i = 0; i < pts.size(); i += 2) {
      v.push_back( vec2(pts(i), pts(i + 1)) );
    }

    return v;
  }

  template<typename T>
  bool elem(const T& e, const std::vector<T>& elems) {
    return find(begin(elems), end(elems), e) != end(elems);
  }

  std::vector<vec2>
  from_vector(const ublas::vector<double>& pts,
	      const unsigned len,
	      std::vector<unsigned>& deleted_inds) {
    vector<vec2> v;

    unsigned pts_ind = 0;
    for (unsigned i = 0; i < (deleted_inds.size() + pts.size()) / 2.0; i++) {

      unsigned x_ind = 2*i;
      unsigned y_ind = 2*i + 1;

      vec2 next(0, 0);
      if (!elem(x_ind, deleted_inds)) {
	next = vec2(pts[pts_ind], next.y());
	pts_ind++;
      }
      if (!elem(y_ind, deleted_inds)) {
	next = vec2(next.x(), pts[pts_ind]);
	pts_ind++;
      }

      v.push_back(next);

    }

    return v;
  }

  ublas::matrix<double>
  build_element_k_matrix(const unsigned elem_ind, const trimesh& mesh) {
    unsigned dof = 2*mesh.verts.size();

    // Steel material properties
    double youngs_modulus = 30e6;
    double nu = 0.25;

    // Geometry
    double thickness = 0.5;

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
    cout << "D = " << endl;
    cout << D << endl;

    vertex_triangle t = mesh.tris[elem_ind];

    double y1 = mesh.verts[t.verts[0]].y();
    double y2 = mesh.verts[t.verts[1]].y();
    double y3 = mesh.verts[t.verts[2]].y();

    double y23 = y2 - y3;
    double y31 = y3 - y1;
    double y12 = y1 - y2;
    double y13 = y1 - y3;

    double x1 = mesh.verts[t.verts[0]].x();
    double x2 = mesh.verts[t.verts[1]].x();
    double x3 = mesh.verts[t.verts[2]].x();

    double x32 = x3 - x2;
    double x13 = x1 - x3;
    double x21 = x2 - x1;
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

    cout << B << endl;
    
    B = (1.0 / detJ) * B;


    double area = std::abs(x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2;

    cout << "Area = " << area << endl;

    //    D = thickness*area*D;
    ublas::matrix<double> B_t = trans(B);
    ublas::matrix<double> DB = prod(D, B);

    cout << "Computing B_t D B" << endl;

    ublas::matrix<double> k_basic = prod(B_t, DB);
    cout << "Done with B_t D B" << endl;
    k_basic = thickness*area*k_basic;

    ublas::matrix<double> k = ublas::zero_matrix<double>(dof);
    for (unsigned i = 0; i < 6; i++) {

      unsigned global_row_ind = 0;
      if (i % 2 == 0) {
	global_row_ind = 2 * t.verts[i / 2];
      } else {
	global_row_ind = 2 * t.verts[(i - 1) / 2] + 1;
      }

      for (unsigned j = 0; j < 6; j++) {
	unsigned global_col_ind = 0;
	if (j % 2 == 0) {
	  global_col_ind = 2 * t.verts[j / 2];
	} else {
	  global_col_ind = 2 * t.verts[(j - 1) / 2] + 1;
	}

	k(global_row_ind, global_col_ind) = k_basic(i, j);

      }
    }
    
    return k;
  }
  
  ublas::matrix<double>
  build_k_matrix(const trimesh& mesh) {
    //ublas::vector<double> pts = to_vector(mesh.pts);
    unsigned dof = 2*mesh.verts.size();
    ublas::matrix<double> k = ublas::zero_matrix<double>(dof);

    for (unsigned i = 0; i < mesh.tris.size(); i++) {
      ublas::matrix<double> k_i = build_element_k_matrix(i, mesh);
      k += k_i;
    }

    return k;
  }

  void cull_by_constraints(ublas::vector<double>& v,
			   std::vector<unsigned>& to_delete) {
    assert(v.size() > to_delete.size());

    sort(begin(to_delete), end(to_delete));

    ublas::vector<double> cv(v.size() - to_delete.size());

    unsigned cv_ind = 0;
    for (unsigned i = 0; i < v.size(); i++) {
      if (!elem(i, to_delete)) {
	cv(cv_ind) = v[i];
	cv_ind++;
      }
    }
    v = cv;
  }

  void cull_by_constraints(ublas::matrix<double>& v,
			   std::vector<unsigned>& to_delete) {
    assert(v.size1() > to_delete.size());
    assert(v.size2() > to_delete.size());

    sort(begin(to_delete), end(to_delete));

    ublas::matrix<double> cv(v.size1() - to_delete.size(),
			     v.size2() - to_delete.size());

    unsigned cv_ind_1 = 0;

    for (unsigned i = 0; i < v.size1(); i++) {
      if (!elem(i, to_delete)) {

	unsigned cv_ind_2 = 0;
    
	for (unsigned j = 0; j < v.size2(); j++) {
	  if (!elem(j, to_delete)) {
	    cv(cv_ind_1, cv_ind_2) = v(i, j);
	    cv_ind_2++;
	  }
	}

	cv_ind_1++;
      }
    }

    v = cv;

  }

  std::vector<unsigned>
  build_num_constraints(const std::vector<constraint2>& constraints) {
    vector<unsigned> cnums;
    for (auto c : constraints) {
      switch (c.c) {
      case X_FIXED:
	cnums.push_back(2*c.point_number);
	break;

      case Y_FIXED:
	cnums.push_back(2*c.point_number + 1);
	break;

      case XY_FIXED:
	cnums.push_back(2*c.point_number);
	cnums.push_back(2*c.point_number + 1);
	break;
	
      default:
	assert(false);
      }
    }
    return cnums;
  }

  std::vector<vec2>
  compute_displacements(const trimesh& mesh,
			const std::vector<constraint2>& constraints,
			const std::vector<vec2>& forces) {
    ublas::vector<double> f = to_vector(forces);
    ublas::matrix<double> k = build_k_matrix(mesh);

    vector<unsigned> num_constraints = build_num_constraints(constraints);
    cull_by_constraints(f, num_constraints);
    cull_by_constraints(k, num_constraints);

    cout << "f size = " << f.size() << endl;
    cout << "f = " << endl;
    cout << f << endl;

    cout << "k size 1 = " << k.size1() << endl;
    cout << "k size 2 = " << k.size2() << endl;
    cout << "k = " << endl;
    cout << k << endl;

    ublas::matrix<double> k_inv = ublas::identity_matrix<double>(k.size1());
    ublas::permutation_matrix<size_t> pm(k.size1());
    lu_factorize(k, pm);
    lu_substitute(k, pm, k_inv);    

    ublas::vector<double> u = prod(k_inv, f);

    cout.precision(17);
    cout << "u = " << u << endl;

    unsigned original_size = 2*forces.size();
    return from_vector(u, original_size, num_constraints);

  }

}
