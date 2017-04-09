#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include "trimesh.h"
#include "vec2.h"

using namespace std;

namespace fem_2d {

  TEST_CASE("2 triangle system") {
    vector<vec2> points;
    points.push_back(vec2(0, 0));
    points.push_back(vec2(1, 0));
    points.push_back(vec2(1, 1));
    points.push_back(vec2(0, 1));

    vector<vertex_triangle> tris;
    
  }
  
}
