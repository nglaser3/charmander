#include "basic_types.h"
#include "constants.h"
#include "char_math.h"
#include "materials/nuclide.h"
#include "simulation/particle.h"

#include "simulation/simulation.h"

namespace charmander
{
  void Simulation::Run() const {
    for (int i = 0; i < settings_.batches; i++)
    {
      for (size_t j = 0; j < settings_.histories; j++)
      {
        // init rng
        LinearCongruentialGenerator lcg(i*settings_.histories + j);

        // init particle
        Particle p{
          settings_.source.point, 
          SampleDirection(lcg),
          settings_.source.energy
        };

        // transport
        while (p.alive)
        {
          // update to next position, returns true if leaked
          if (TransportParticle(p, lcg)) break;
          
          // check reaction
          CollideParticle(p, lcg);
        }
        
      }
    }
  }

  bool Simulation::TransportParticle(Particle& p, LinearCongruentialGenerator& lcg) const {
    p.position = geom_.NewPosition(p.position, p.direction, SampleMFP(lcg), p.energy);
    return p.position == INF_POINT;
  }

  void Simulation::CollideParticle(Particle& p, LinearCongruentialGenerator& lcg) const {
    auto [reaction, mass] = geom_.CollisionType(p.position, p.energy, lcg(), lcg());
    switch (reaction)
    {
    case MT::INELASTIC:
      // treat inelastic as elastic for now
      _LIBCPP_FALLTHROUGH();
    case MT::ELASTIC:
      p.direction = SampleDirection(lcg);
      p.energy = p.energy * 0.5;
      break;
    case MT::FISSION:
      _LIBCPP_FALLTHROUGH();
    case MT::CAPTURE:
      p.alive = false;
      break;
    case MT::MISSED:
      p.alive = false;
      break;
    }
  }
} // namespace charmander
