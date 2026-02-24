#ifndef CHARMANDER_SURFACES_SURFACE_H_
#define CHARMANDER_SURFACES_SURFACE_H_

#include "basic_types.h"

namespace charmander {

class Surface {
 public:
  Surface() {};

  bool Sense(Point p) const;

  virtual double Evaluate(Point p) const = 0;

  virtual double Distance(Point p, Direction d) const = 0;

 protected:
  int id_;
};

}  // namespace charmander
#endif  // CHARMANDER_SURFACES_SURFACE_H_