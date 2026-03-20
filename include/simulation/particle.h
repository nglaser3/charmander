#ifndef CHARMANDER_SIMULATION_PARTICLE_H_
#define CHARMANDER_SIMULATION_PARTICLE_H_

#include "basic_types.h"

namespace charmander
{
  struct Particle
  {
    Point position;
    Direction direction;
    double energy;
    double weight = 1.0;
    bool alive=true;
  };
  
} // namespace charmander

#endif  // CHARMANDER_SIMULATION_PARTICLE_H_