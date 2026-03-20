#ifndef CHARMANDER_SIMULATION_SIMULATION_H_
#define CHARMANDER_SIMULATION_SIMULATION_H_

#include <map>
#include <vector>
#include "char_math.h"
#include "geometry/geometry.h"
#include "simulation/particle.h"

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
    Simulation(const Geometry& geom, const Settings& settings): geom_(geom), settings_(settings) {
      for (const auto& mt_rxn : {MT(-1), MT(2), MT(4), MT(18), MT(102)})
      {
        tallies_[mt_rxn].resize(settings.batches);
      }
    };
  
    void Run();

    bool TransportParticle(Particle& p, LinearCongruentialGenerator& lcg) const;
    MT CollideParticle(Particle& p, LinearCongruentialGenerator& lcg) const;
    void ScatterParticle(Particle& p, LinearCongruentialGenerator& lcg, double A) const;
    void TallyParticle(size_t batch, const Particle& p, MT reaction);
    void FinalizeTallies();

  private: 
    const Geometry geom_;
    const Settings settings_;

    std::map<MT, std::vector<double>> tallies_;
  };
  
} // namespace charmander


#endif