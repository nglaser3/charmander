#include "basic_types.h"
#include "constants.h"
#include "char_math.h"
#include "materials/nuclide.h"
#include "simulation/particle.h"

#include "simulation/simulation.h"

namespace charmander
{
  void Simulation::Run() {
    for (size_t i = 0; i < settings_.batches; i++)
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
          if (!TransportParticle(p, lcg)) {
            // check reaction
            MT rxn = CollideParticle(p, lcg);
            // tally interaction
            TallyParticle(i, p, rxn);
          } else {
            TallyParticle(i, p, MT::MISSED);
            break;
          }
        }
      }
    }
    FinalizeTallies();
  }

  bool Simulation::TransportParticle(Particle& p, LinearCongruentialGenerator& lcg) const {
    p.position = geom_.NewPosition(p.position, p.direction, SampleMFP(lcg), p.energy);
    return p.position == INF_POINT;
  }

  MT Simulation::CollideParticle(Particle& p, LinearCongruentialGenerator& lcg) const {
    auto [reaction, mass] = geom_.CollisionType(p.position, p.energy, lcg(), lcg());
    switch (reaction)
    {
    case MT::INELASTIC:
      // treat inelastic as elastic for now
      [[fallthrough]];
    case MT::ELASTIC:
      ScatterParticle(p, lcg, mass);
      break;
    case MT::FISSION:
      // treat fission as capture for now
      [[fallthrough]];
    case MT::CAPTURE:
      p.alive = false;
      break;
    case MT::MISSED:
      p.alive = false;
      break;
    }
    return reaction;
  }

  void Simulation::ScatterParticle(Particle& p, LinearCongruentialGenerator& lcg, double A) const {
    p.direction = SampleDirection(lcg);
    double mu_cm = 2.0 * lcg() - 1.0;

    double delta_e = (1.0 + A*A + 2.0*A*mu_cm) / ((1.0+A) * (1.0+A));
    p.energy *= delta_e;
  }

  void Simulation::TallyParticle(size_t batch, const Particle& p, MT reaction) {
    tallies_[reaction][batch] += p.weight;
  }

  void Simulation::FinalizeTallies() {
    std::vector<double> total_weight(settings_.batches);
    for (auto& [mt, result] : tallies_)
    {
      for (auto i = 0; i < settings_.batches; i++)
      {
        total_weight[i] += result[i];
      }
    }
    for (auto& [mt, result] : tallies_)
    {
      for (auto i = 0; i < settings_.batches; i++)
      {
        if (total_weight[i] > 0.0)
          result[i] /= total_weight[i];
      }
    }
  }
} // namespace charmander
