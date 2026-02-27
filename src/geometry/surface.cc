#include "geometry/surface.h"

#include <cmath>

#include "basic_types.h"

namespace charmander {

bool Surface::Sense(Point p) const { return not std::signbit(Evaluate(p)); }

}  // namespace charmander