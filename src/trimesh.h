#pragma once

#include <vector>

#include "vec2.h"

namespace fem_2d {

  struct vertex_triangle {
    unsigned verts[3];

    vertex_triangle(const unsigned v0_ind,
		    const unsigned v1_ind,
		    const unsigned v2_ind) {
      verts[0] = v0_ind;
      verts[1] = v1_ind;
      verts[2] = v2_ind;
    }
  };

  class trimesh {
  public:
    std::vector<vec2> verts;
    std::vector<vertex_triangle> tris;
  };
}
