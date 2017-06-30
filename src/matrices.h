#pragma once

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/io.hpp>

namespace ublas = boost::numeric::ublas;

#include "trimesh.h"

namespace fem_2d {

  ublas::matrix<double>
  build_element_B_matrix(const unsigned elem_ind,
			 const trimesh& mesh);

  ublas::matrix<double>
  build_D_matrix();

}
