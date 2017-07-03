#include "vec2.h"

namespace fem_2d {

  std::ostream& operator<<(std::ostream& out, const vec2 v) {
    out << "( " << v.x() << ", " << v.y() << " )";
    return out;
  }

}
