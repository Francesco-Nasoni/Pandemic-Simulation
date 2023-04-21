#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "epidemic.hpp"

TEST_CASE("Testing Epidemic throws") {
  SUBCASE("Running whit all zeros, throws") {
    CHECK_THROWS(Epidemic{0, 0, 0, 0});
  }

  SUBCASE("Running B=0 e Y=0") { CHECK_THROWS(Epidemic{100, 10, 0, 0}); }

  SUBCASE("Running S<0 ") { CHECK_THROWS(Epidemic{-100, 10, 0.1, 0.1}); }

  SUBCASE("Running B<0") { CHECK_THROWS(Epidemic{100, 10, -0.1, 0.1}); }

  SUBCASE("Running B>1") { CHECK_THROWS(Epidemic{100, 10, 1.1, 0.1}); }

  SUBCASE("Running Y<0") { CHECK_THROWS(Epidemic{100, 10, 0.1, -0.1}); }

  SUBCASE("Running Y>1") { CHECK_THROWS(Epidemic{100, 10, 0.1, 1.1}); }
}
TEST_CASE("Testing Epidemic limit cases") {
  SUBCASE("Runningg whit I=0") {
    Epidemic epd{100, 0, 0.1, 0.1};
    epd.evolve();
    CHECK(epd.round().I == 0);
    CHECK(epd.round().S == 100);
    CHECK(epd.round().R == 0);
    CHECK(epd.is_ended() == 1);
    std::cout << "Finished I=0" << std::endl;
  }

  SUBCASE("Running B=0") {
    Epidemic epd{100, 10, 0, 0.2};
    while (!epd.is_ended())
      epd.evolve();
    CHECK(epd.round().S == 100);
    CHECK(epd.round().I == 0);
    CHECK(epd.round().R == 10);
    std::cout << "Finished B=0" << std::endl;
  }

  SUBCASE("Running Y=0") {
    Epidemic epd{100, 10, 0.2, 0};
    while (!epd.is_ended())
      epd.evolve();
    CHECK(epd.round().S == 0);
    CHECK(epd.round().I == 110);
    CHECK(epd.round().R == 0);
    std::cout << "Finished Y=0" << std::endl;
  }

  SUBCASE("Running S=0") {
    Epidemic epd{0, 10, 0.2, 0.2};
    while (!epd.is_ended())
      epd.evolve();
    CHECK(epd.round().S == 0);
    CHECK(epd.round().I == 0);
    CHECK(epd.round().R == 10);
    std::cout << "Finished S=0" << std::endl;
  }
}

TEST_CASE("Testing Epidemic function") {
  SUBCASE("Testing corret constat population") {
    Epidemic epd{100000, 50, 0.2, 0.1};
    int i = 0;
    while (!epd.is_ended()) {
      epd.evolve();
      i++;
      CHECK(epd.round().S + epd.round().I + epd.round().R == 100050);
    }
    std::cout << "Finished N test with " << i << " interaction" << std::endl;
  }
}