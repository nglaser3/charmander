#include <iostream>
#include <fstream>
#include <iomanip>
#include <string> 

#include "basic_types.h"
#include "constants.h"
#include "char_math.h"
#include "materials/nuclide.h"
#include "simulation/particle.h"

#include "simulation/simulation.h"

namespace charmander
{
  void Simulation::Run() {
    std::string header(80, '=');
    std::cout<<std::endl<<header<<std::endl<<"Beginning Simulation"<<std::endl<<header<<std::endl;
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
            MT rxn = (settings_.implicit_capture) ? CollideImplicitCapture(p, lcg) : CollideParticle(p, lcg);
            // tally interaction
            TallyParticle(i, p, rxn);
          } else {
            TallyParticle(i, p, MT::MISSED);
            break;
          }
        }
      }
    }
    std::cout<<"Finished Simulation"<<std::endl<<header<<std::endl;
    std::cout<<"Finalizing and Writing Tallies"<<std::endl<<header<<std::endl;
    FinalizeTallies();
    WriteOutTallies();
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

  MT Simulation::CollideImplicitCapture(Particle& p, LinearCongruentialGenerator& lcg) const {
    double p_nonabs = geom_.ProbabilityNonAbs(p.position, p.energy);
    double mass = geom_.GetMass(p.position);

    if (p_nonabs <= 0.0) {
      p.alive = false;
      return MT::CAPTURE;
    }

    p.weight *= p_nonabs;
    ScatterParticle(p, lcg, mass);
    return MT::ELASTIC;
  }

  void Simulation::ScatterParticle(Particle& p, LinearCongruentialGenerator& lcg, double A) const {
    p.direction = SampleDirection(lcg);
    double mu_cm = 2.0 * lcg() - 1.0;

    double delta_e = (1.0 + A*A + 2.0*A*mu_cm) / ((1.0+A) * (1.0+A));
    p.energy *= delta_e;
  }

  void Simulation::EnergyTally(const Particle& p) {
    if (p.energy < 1e-5 || p.energy > settings_.source.energy) return;
    double emax = std::log(settings_.source.energy);
    double emin = std::log(1e-5);
    // map to legendre bins
    double e = 2.0 * (std::log(p.energy) - emin) / (emax - emin) - 1.0;

    for (size_t l = 0; l < legendre_coeffs_.size(); ++l)
        legendre_coeffs_[l] += p.weight * LegendreP(l, e);
  }

  void Simulation::TallyParticle(size_t batch, const Particle& p, MT reaction) {
    tallies_[reaction][batch] += p.weight;
    EnergyTally(p);
  }

  void Simulation::FinalizeTallies() {
    // normalize reaction tallies
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

    // normalize legendre
    double norm = legendre_coeffs_[0];
    if (norm > 0.0) {
      for (size_t l = 0; l < legendre_coeffs_.size(); ++l)
        legendre_coeffs_[l] = (2.0 * l + 1.0) * legendre_coeffs_[l] / norm; 
    }

    // calculate error
    for (auto& [mt, result] : tallies_) {
      std::vector<double>& err = errors_[mt];

      double sum = 0.0;
      double sum_sq = 0.0;

      for (size_t i = 0; i < settings_.batches; ++i)
      {
        double xi = result[i];
        sum += xi;
        sum_sq += xi * xi;

        double N = i + 1;
        if (N < 2) {
          err[i] = 0.0;
          continue;
        }

        double mean = sum / N;
        double var = (sum_sq / N - mean * mean);

        if (var < 0.0) var = 0.0;

        err[i] = std::sqrt(var / N);
      }
    }
  }

  void Simulation::WriteOutTallies() {
    WriteOutCollisions();
    WriteOutLegendre();
  }

  void Simulation::WriteOutCollisions() {
    std::string filename = "CHARMANDER_OUT";
    std::ofstream file(filename);
    if (!file)
        throw std::runtime_error("Failed to open tally output file: " + filename);

    file << std::setprecision(10);

    file << "batch,"
         << "elastic,elastic_err,"
         << "inelastic,inelastic_err,"
         << "fission,fission_err,"
         << "capture,capture_err,"
         << "missed,missed_err"
         <<std::endl;

    for (size_t i = 0; i < settings_.batches; ++i)
    {
        file << i << ",";
        for (const auto& mt : {MT(-1), MT(2), MT(4), MT(18), MT(102)})
        {
          file << tallies_.at(mt).at(i) << "," << errors_.at(mt).at(i);
          switch (mt)
          {
            case MT(102):
              file<<std::endl;
              break;
            default:
              file<<",";
              break;
          }
        }
    }
  }

  void Simulation::WriteOutLegendre() {
    std::string filename = "CHARMANDER_LEGENDRE";
    std::ofstream file(filename);
    if (!file)
      throw std::runtime_error("Failed to open Legendre output file: " + filename);

    file << std::setprecision(10);

    file << "order,coefficient\n";

    for (size_t l = 0; l < legendre_coeffs_.size(); ++l)
    {
      file << l << "," << legendre_coeffs_[l] << std::endl;
    }
  }
} // namespace charmander
