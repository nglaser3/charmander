#include "input.h"
#include "simulation/simulation.h"

int main() {
  auto [geom, settings] = charmander::UserInput();
  auto sim = charmander::Simulation(geom, settings);
  sim.Run();
}