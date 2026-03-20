#ifndef CHARMANDER_SIMULATION_SIMULATION_H_
#define CHARMANDER_SIMULATION_SIMULATION_H_

#include "geometry/geometry.h"

namespace charmander
{
  struct SimParams {
    Geometry geom;
    int histories = 10000;
    int inactive = 50;
    int batches = 100;
  };

  class Simulation
  {
  public:
    Simulation(SimParams& params);
  
    void Run() const;
  };
  
} // namespace charmander


#endif