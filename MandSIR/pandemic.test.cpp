#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "pandemic.hpp"

TEST_CASE("Testing Pandemic") {
  SUBCASE("Running whit all zeros, throws") {
    CHECK_THROWS(Pandemic{0, 0, 0, 0});
  }

  SUBCASE("Running B=0 e Y=0") { CHECK_THROWS(Pandemic{0, 0, 100, 10}); }

  SUBCASE("Running S<0 e I<0") { CHECK_THROWS(Pandemic{0, 0, -100, -10}); }

  SUBCASE("Running B<0 e Y<0") { CHECK_THROWS(Pandemic{-0.2, -0.1, 100, 10}); }

  SUBCASE("Runningg whit I=O") {
    Pandemic pan{0.2, 0.1, 100, 0};
    pan.evolve();
    CHECK(pan.round().I == 0);
    CHECK(pan.round().S == 100);
    CHECK(pan.round().R == 0);
    CHECK(pan.isEnded() == 1);
    std::cout << "Finished I=0" << std::endl;
  }

  SUBCASE("Running B=0") {
    Pandemic pan{0, 0.2, 100, 10};
    while (!pan.isEnded())
      pan.evolve();
    CHECK(pan.round().S == 100);
    CHECK(pan.round().I == 0);
    CHECK(pan.round().R == 10);
    std::cout << "Finished B=0" << std::endl;
  }

  SUBCASE("Running Y=0") {
    Pandemic pan{0.2, 0, 100, 10};
    while (!pan.isEnded())
      pan.evolve();
    CHECK(pan.round().S == 0);
    CHECK(pan.round().I == 110);
    CHECK(pan.round().R == 0);
    std::cout << "Finished Y=0" << std::endl;
  }

  SUBCASE("Running S=0") {
    Pandemic pan{0.2, 0.3, 0, 10};
    while (!pan.isEnded())
      pan.evolve();
    CHECK(pan.round().S == 0);
    CHECK(pan.round().I == 0);
    CHECK(pan.round().R == 10);
    std::cout << "Finished S=0" << std::endl;
  }

  SUBCASE("Running S=0") {
    Pandemic pan{0.10, 0.05, 100000, 50};
    int i = 0;
    while (!pan.isEnded()) {
      pan.evolve();
      i++;
      CHECK(pan.round().S + pan.round().I + pan.round().R == 100050);
    }
    std::cout << "Finished N test with " << i << " interaction" << std::endl;
  }
}