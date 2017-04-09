#include "direct_solve.h"

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/io.hpp>

namespace ublas = boost::numeric::ublas;

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
  compute_displacements(const trimesh& mesh,
			const std::vector<constraint2>& constraints,
			const std::vector<vec2>& forces) {
    ublas::vector<double> fs = to_vector(forces);
    return {};
  }

}
