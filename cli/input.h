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
    // materials
    auto nuclides = MakeNuclides({"H1", "O16", "U235", "U238"});
    
    CEMaterial uo2(1, {
      {nuclides["O16"], 2.0/3.0, 16.0},
      {nuclides["U235"], 1.0/3.0 * 0.1975, 235.0},
      {nuclides["U238"], 1.0/3.0 * 0.8025, 238.0}
    }, 10.5);

    CEMaterial water(2, {
      {nuclides["H1"], 2.0/3.0, 1.0},
      {nuclides["O16"], 1.0/3.0, 16.0}
    }, 1.0);

    // geometry
    auto inner = std::make_shared<ZCylinder>(10.0, Point{0.0, 0.0, 0.0});
    auto outer = std::make_shared<ZCylinder>(50.0, Point{0.0, 0.0, 0.0});
    auto top = std::make_shared<ZPlane>(1000.0);
    auto bottom = std::make_shared<ZPlane>(-1000.0);
    auto right = std::make_shared<XPlane>(100.0);
    auto left = std::make_shared<XPlane>(-100.0);
    auto back = std::make_shared<YPlane>(100.0);
    auto front = std::make_shared<YPlane>(-100.0);

    Region top_bottom = +bottom & -top;
    Region bbox = +left & -right & +front & -back;
    Cell inwater(1, water, -inner & top_bottom);
    Cell fuel(2, uo2, +inner & -outer & top_bottom);
    Cell outwater(3, water, +outer & top_bottom & bbox);
    Geometry geom({inwater, fuel, outwater});

    // settings
    Source source{{0.0, 0.0, 0.0}, 1e6};
    Settings settings = {
      source,
      10000,
      100
    };

    return {geom, settings};
  }

} // namespace charmander


#endif