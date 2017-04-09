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

  ublas::matrix<double>
  build_k_matrix(const trimesh& mesh) {
    //ublas::vector<double> pts = to_vector(mesh.pts);
    unsigned dof = 2*mesh.verts.size();
    ublas::matrix<double> k_inv = ublas::identity_matrix<float>(dof);

    return k_inv;
  }

  template<typename T>
  bool elem(const T& e, const std::vector<T>& elems) {
    return find(begin(elems), end(elems), e) != end(elems);
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
			   const std::vector<unsigned> to_delete) {
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

    ublas::matrix<double> k_inv = ublas::identity_matrix<float>(k.size1());
    ublas::permutation_matrix<size_t> pm(k.size1());
    lu_factorize(k, pm);
    lu_substitute(k, pm, k_inv);    

    ublas::vector<double> u = prod(k_inv, f);

    cout << "u = " << u << endl;

    return from_vector(u);

  }

}
