#ifndef CHARMANDER_INPUT_H_
#define CHARMANDER_INPUT_H_

#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <memory>

#include "simulation/simulation.h"
#include "materials/nuclide.h"
#include "geometry/geometry.h"
#include "geometry/cell.h"
#include "geometry/cylinder.h"
#include "geometry/plane.h"

namespace charmander
{

  std::map<std::string, std::shared_ptr<Nuclide>> MakeNuclides(std::vector<std::string> names) {
    std::map<std::string, std::shared_ptr<Nuclide>> nucmap;
    std::string header(80, '=');
    std::cout<<header<<std::endl<<"Loading Nuclide Data"<<std::endl<<header<<std::endl;
    for (const auto& name : names)
    {
      std::cout << "\tLoading Nuclide: " + name << std::endl;
      auto nuc = std::make_shared<Nuclide>(name);
      nuc->LoadFromFile();
      std::cout << "\t\tSuccesful" <<std::endl;
      nucmap[name] = nuc;
    }
    return nucmap;
  }

std::pair<Geometry, Settings> UserInput() {
    auto nuclides = MakeNuclides({
      "H1", "O16",
      "Fe54", "Fe56", "Fe57", "Fe58",
      "Pb204", "Pb206", "Pb207", "Pb208"
    });

    CEMaterial water(1, {
      {nuclides["H1"],  2.0/3.0, 1.0},
      {nuclides["O16"], 1.0/3.0, 16.0}
    }, 1.0);

    CEMaterial iron(2, {
      {nuclides["Fe54"], 0.05845, 54.0},
      {nuclides["Fe56"], 0.91754, 56.0},
      {nuclides["Fe57"], 0.02119, 57.0},
      {nuclides["Fe58"], 0.00282, 58.0}
    }, 7.87);

    CEMaterial lead(3, {
      {nuclides["Pb204"], 0.014, 204.0},
      {nuclides["Pb206"], 0.241, 206.0},
      {nuclides["Pb207"], 0.221, 207.0},
      {nuclides["Pb208"], 0.524, 208.0}
    }, 11.34);

    auto x0 = std::make_shared<XPlane>(0.0);
    auto x1 = std::make_shared<XPlane>(5.0);
    auto x2 = std::make_shared<XPlane>(15.0);
    auto x3 = std::make_shared<XPlane>(30.0);

    auto y0 = std::make_shared<YPlane>(-50.0);
    auto y1 = std::make_shared<YPlane>(50.0);
    auto z0 = std::make_shared<ZPlane>(-50.0);
    auto z1 = std::make_shared<ZPlane>(50.0);

    Region box = +y0 & -y1 & +z0 & -z1;

    Cell region1(1, water, +x0 & -x1 & box);
    Cell region2(2, iron,  +x1 & -x2 & box);
    Cell region3(3, lead,  +x2 & -x3 & box);

    Geometry geom({region1, region2, region3});

    Source source{{15.0, 0.0, 0.0}, 1e6};
    Settings settings = {
      source, //source
      100000, // histories
      200, //batches
      false, //implicit capture
      1e4, //roulette energy
      5.0, // roulette diff
    };

    return {geom, settings};
}

} // namespace charmander


#endif