#pragma once

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
    
  };
}
