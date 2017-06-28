#include "stresses.h"

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/io.hpp>

namespace ublas = boost::numeric::ublas;

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

    return {};
  }
  
}
