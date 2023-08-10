#include <catch2/catch_test_macros.hpp>

#include <cstdint>

uint32_t factorial2(uint32_t number) {
    return number <= 1 ? number : factorial2(number - 1) * number;
}

TEST_CASE("Factorials are computed", "[factorial2]") 
{
    REQUIRE(factorial2(1) == 1);
    REQUIRE(factorial2(2) == 2);
    REQUIRE(factorial2(3) == 6);
    REQUIRE(factorial2(10) == 3'628'800);
}