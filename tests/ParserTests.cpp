#include <gtest/gtest.h>

#include "Parser.hpp"

#include <soci/soci.h>
#include <soci/mysql/soci-mysql.h>

struct ClassCoordinatesTestSuite : public testing::TestWithParam<std::tuple<Coordinates, Coordinates, bool, int>> {
};

TEST_P(ClassParserTestSuite, methodDistance) {
    session sql(mysql, "db=db user=root password=stokrotka");
    // auto [first, second, equal, expectedDistance] = GetParam();
    // auto distance = Coordinates::distance(first, second);
    
    // EXPECT_EQ(distance, expectedDistance);
}


INSTANTIATE_TEST_SUITE_P(MyInstantiationName,
                         ClassParserTestSuite,
                         testing::Values(
                             std::make_tuple(Coordinates{0, 5}, Coordinates{0, 5}, true, 0),
                             std::make_tuple(Coordinates{1, 6}, Coordinates{4, 10}, false, 7),
                             std::make_tuple(Coordinates{4, 10}, Coordinates{0, 0}, false, 14),
                             std::make_tuple(Coordinates{10, 10}, Coordinates{2, 5}, false, 13)
                         )
);