#ifndef CHARMANDER_SIMULATION_SIMULATION_H_
#define CHARMANDER_SIMULATION_SIMULATION_H_

#include "geometry/geometry.h"

namespace charmander
{
  struct Source {
    Point point;
    double energy;
  };

  struct Settings {
    Source source;
    int histories = 10000;
    int batches = 100;
  };

  class Simulation
  {
  public:
    Simulation(const Geometry& geom, const Settings& settings): geom_(geom), settings_(settings) {};
  
    void Run() const;

    bool TransportParticle(Particle& p, LinearCongruentialGenerator& lcg) const;
    void CollideParticle(Particle& p, LinearCongruentialGenerator& lcg) const;

  private: 
    const Geometry geom_;
    const Settings settings_;
  };
  
} // namespace charmander


#endif