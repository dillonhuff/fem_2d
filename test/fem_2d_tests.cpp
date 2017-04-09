#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include "constraint.h"
#include "direct_solve.h"
#include "trimesh.h"
#include "vec2.h"

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
    constraints.push_back({0, XY_FIXED});//{0, 1, 3, 6, 7};
    constraints.push_back({3, XY_FIXED});//{0, 1, 3, 6, 7};
    constraints.push_back({1, Y_FIXED});//{0, 1, 3, 6, 7};

    vector<vec2> displacements =
      compute_displacements(mesh, constraints, forces);


    REQUIRE(displacements.size() == 4);

    cout << "disp[2].y() = " << displacements[2].y() << endl;

    REQUIRE(displacements[2].y() < 0.0);
  }
  
}
