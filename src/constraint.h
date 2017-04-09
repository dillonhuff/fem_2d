#pragma once

namespace fem_2d {

  enum xy_constraint {
    X_FIXED = 0,
    Y_FIXED,
    XY_FIXED
  };

  struct constraint2 {
    unsigned point_number;
    xy_constraint c;
  };

}
