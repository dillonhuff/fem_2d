#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include "constraint.h"
#include "direct_solve.h"
#include "trimesh.h"
#include "utils.h"
#include "vec2.h"
#include "visual_debug.h"
#include "stresses.h"

using namespace std;

namespace fem_2d {

  TEST_CASE("2 triangle system") {
    vector<vec2> points;
    points.push_back(vec2(3, 0));
    points.push_back(vec2(3, 2));
    points.push_back(vec2(0, 2));
    points.push_back(vec2(0, 0));

    vector<vertex_triangle> tris;
    tris.push_back({0, 1, 3});
    tris.push_back({3, 1, 2});

    trimesh mesh{points, tris};

    vector<vec2> forces;
    forces.push_back(vec2(0, 0));
    forces.push_back(vec2(0, -1000));
    forces.push_back(vec2(0, 0));
    forces.push_back(vec2(0, 0));

    vector<constraint2> constraints;
    constraints.push_back({2, XY_FIXED});
    constraints.push_back({3, XY_FIXED});
    constraints.push_back({0, Y_FIXED});

    material_properties material{30e6, 0.25};    

    vector<vec2> displacements =
      compute_displacements(mesh, constraints, forces, material);

    SECTION("Displacements") {
      REQUIRE(displacements.size() == 4);

      REQUIRE(within_eps(displacements[2].y(), -7.415e-5, 0.005));
    }

    SECTION("Stresses") {
      auto stresses =
	compute_stresses(mesh, constraints, displacements, material);

      REQUIRE(stresses.size() == mesh.tris.size());

      REQUIRE(within_eps(stresses[0](0), -93, 1.0));
    }
  }
  
}
