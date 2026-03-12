#include <gtest/gtest.h>
#include <stdexcept>

#include "basic_types.h"
#include "constants.h"
#include "geometry/region.h"
#include "geometry/surface.h"
#include "geometry/cylinder.h"
#include "geometry/plane.h"

namespace charmander
{
  TEST(Halfspace, Constructor) {
    // non initialized ptr 
    EXPECT_THROW(Halfspace(nullptr, false), std::runtime_error);

    Cylinder cyl(1.0, {1, 0, 0}, {0.0, 0.0, 0.0});
    Halfspace* hs;
    EXPECT_NO_THROW(hs = new Halfspace(&cyl, true));
    // compares addresses, should be the same
    EXPECT_EQ(&hs->GetSurface(), &cyl);
    // + operator overload
    EXPECT_NO_THROW(+cyl);
    // - operator overload
    EXPECT_NO_THROW(-cyl);
  }

  TEST(Halfspace, Sense) {
    Cylinder cyl(1.0, {1, 0, 0}, {0.0, 0.0, 0.0});
    Point in(0.0, 0.0, 0.0);
    Point on(0.0, 1.0, 0.0);
    Point out(5.0, 5.0, 5.0);

    Halfspace pos = +cyl;
    EXPECT_TRUE(pos.Sense(out));
    EXPECT_TRUE(pos.Sense(on));
    EXPECT_FALSE(pos.Sense(in));

    Halfspace neg = -cyl;
    EXPECT_TRUE(neg.Sense(in));
    EXPECT_FALSE(neg.Sense(out));
  }

  TEST(Halfspace, Complement) {
    Cylinder cyl(1.0, {1, 0, 0}, {0.0, 0.0, 0.0});
    Point in(0.0, 0.0, 0.0);
    Point out(5.0, 5.0, 5.0);

    Halfspace pos = +cyl;
    Halfspace neg = ~pos;
    EXPECT_TRUE(neg.Sense(in));
    EXPECT_FALSE(neg.Sense(out));
  }

  TEST(Region, Constructor) {
    Cylinder cyl(1.0, {1, 0, 0}, {0.0, 0.0, 0.0});

    // missing or empty clauses
    EXPECT_THROW(Region({}), std::runtime_error);
    EXPECT_THROW(Region({{+cyl}, {}}), std::runtime_error);

    Region* reg;
    EXPECT_NO_THROW(reg = new Region({{+cyl}}));
    EXPECT_EQ(reg->GetClauses().size(), 1);
    EXPECT_EQ(reg->GetClauses().front().size(), 1);
    EXPECT_EQ(&(reg->GetClauses().front().front().GetSurface()),&cyl);
  }

  TEST(Region, Contains) {
    Cylinder cyl(1.0, {0, 0, 1}, {0.0, 0.0, 0.0});
    ZPlane top(5.0);
    ZPlane bottom(-5.0);

    Point in(0.0, 0.0, 0.0);
    Point on(1.0, 0.0, 5.0);
    Point outz(0.0, 0.0, 5.0 + COINCIDENT_SURF);
    Point outr(1.0 + COINCIDENT_SURF, 0.0, 0.0);

    Region inreg({{-cyl, -top, +bottom}});
    EXPECT_TRUE(inreg.Contains(in));
    // on surface evaluates to false for negative halfspaces
    EXPECT_FALSE(inreg.Contains(on));
    EXPECT_FALSE(inreg.Contains(outz));
    EXPECT_FALSE(inreg.Contains(outr));

    Region outreg({{+cyl}, {+top}, {-bottom}});
    EXPECT_FALSE(outreg.Contains(in));
    // on surface evaluates to true for positive halfspaces
    EXPECT_TRUE(outreg.Contains(on));
    EXPECT_TRUE(outreg.Contains(outz));
    EXPECT_TRUE(outreg.Contains(outr));
  }
  
  TEST(Region, Distance) {
    Cylinder cyl(1.0, {0, 0, 1}, {0.0, 0.0, 0.0});
    ZPlane top(5.0);
    ZPlane bottom(-5.0);

    Region inreg({{-cyl, -top, +bottom}});
    Region outreg({{+cyl}, {+top}, {-bottom}});

    // Zplane is closest (top)
    Point z_closest(0.0, 0.0, 6.0);
    Direction z_down(0.0, 0.0, -1.0);
    Direction z_up(0.0, 0.0, 1.0);
    EXPECT_DOUBLE_EQ(inreg.Distance(z_closest, z_down), 1.0);
    EXPECT_DOUBLE_EQ(outreg.Distance(z_closest, z_down), 1.0);
    EXPECT_DOUBLE_EQ(inreg.Distance(z_closest, z_up), INF);
    EXPECT_DOUBLE_EQ(outreg.Distance(z_closest, z_up), INF);

    // cylinder closest
    Point r_closest(2.0, 0.0, 2.5);
    Direction r_in(-1.0, 0.0, 0.0);
    Direction r_out(1.0, 0.0, 0.0);
    EXPECT_DOUBLE_EQ(inreg.Distance(r_closest, r_in), 1.0);
    EXPECT_DOUBLE_EQ(outreg.Distance(r_closest, r_in), 1.0);
    EXPECT_DOUBLE_EQ(inreg.Distance(r_closest, r_out), INF);
    EXPECT_DOUBLE_EQ(outreg.Distance(r_closest, r_out), INF);

    // outside of cylinder, but intersect with bottom (no region entry)
    EXPECT_DOUBLE_EQ(inreg.Distance(r_closest, z_down), INF); 
    // above zplane, intersect with top (no region exit)
    EXPECT_DOUBLE_EQ(outreg.Distance(r_closest + z_closest, z_down), INF);

    // below bottom, out of cylinder, ignore bottom crossing, first real
    // intersection is the cylinder
    Point outside_below(2.0, 0.0, -6.0);
    Direction up_in = normalize({-3.0, 0.0, 4.0});
    EXPECT_DOUBLE_EQ(inreg.Distance(outside_below, up_in), 5.0/3.0);
    EXPECT_DOUBLE_EQ(outreg.Distance(outside_below, up_in), 5.0/3.0);
  }
} // namespace charmander
