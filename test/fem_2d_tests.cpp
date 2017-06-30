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
    points.push_back(vec2(0, 0));
    points.push_back(vec2(3, 0));
    points.push_back(vec2(3, 2));
    points.push_back(vec2(0, 2));

    vector<vertex_triangle> tris;
    tris.push_back({0, 1, 2});
    tris.push_back({0, 2, 3});

    trimesh mesh{points, tris};

    vector<vec2> forces;
    forces.push_back(vec2(0, 0));
    forces.push_back(vec2(0, 0));
    forces.push_back(vec2(0, -1000));
    forces.push_back(vec2(0, 0));

    vector<constraint2> constraints;
    constraints.push_back({0, XY_FIXED});
    constraints.push_back({3, XY_FIXED});
    constraints.push_back({1, Y_FIXED});

    vector<vec2> displacements =
      compute_displacements(mesh, constraints, forces);


    SECTION("Displacements") {
      REQUIRE(displacements.size() == 4);

      REQUIRE(within_eps(displacements[2].y(), -7.415e-5, 0.005));
    }

    SECTION("Stresses") {
      vector<double> stresses =
	compute_stresses(mesh, constraints, displacements);

      REQUIRE(stresses.size() == displacements.size());
    }
  }
  
}
