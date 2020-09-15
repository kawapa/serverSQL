#include <gtest/gtest.h>

#include "ServerSQL.hpp"

// Note!
// 1 - USER_NAME and PASSWORD must be real credentials to MySQL server
// 2 - DATABASE (DATABASE_NAME) must exist

const std::string USER_NAME = "root";
const std::string PASSWORD = "stokrotka";
const std::string DATABASE_NAME = "db_tests";
const std::string TABLE_NAME = "my_table_tests";

struct ServerSQLTests : public ::testing::Test {
public:
    ServerSQLTests() {
        std::stringstream ss;
        ss << "db=" << DATABASE_NAME << " user=" << USER_NAME << " password=" << PASSWORD;

        server = std::make_unique<ServerSQL>(ss.str(), DATABASE_NAME, TABLE_NAME);
    }

    std::unique_ptr<ServerSQL> server;
};

TEST_F(ServerSQLTests, insertNewElementInsertsANewEntry) {
    // GIVEN


    // WHEN
    bool result = server->insertNewElement("Jane", 18);

    // THEN
    ASSERT_TRUE(result);
}

TEST_F(ServerSQLTests, getValueReturnsValueAssignedToItsKey) {
    server->insertNewElement("Bob", 45);

    auto [key, value] = server->getValue("Bob");

    ASSERT_TRUE(key);
    ASSERT_EQ(45, value);
}

TEST_F(ServerSQLTests, deleteElementRemovesEntry) {
    server->insertNewElement("Keith", 35);

    bool result  = server->deleteElement("Keith");

    ASSERT_TRUE(result);
    auto [found, value] = server->getValue("Keith");
    ASSERT_FALSE(found);
}

TEST_F(ServerSQLTests, getOccurencesReturnsNumberOfEntriesWithTheSameValue) {
    server->insertNewElement("Keith", 35);
    server->insertNewElement("Barbara", 35);
    server->insertNewElement("George", 35);

    int result  = server->getOccurences(35);

    ASSERT_EQ(3, result);
}

TEST_F(ServerSQLTests, incrementValueAddsOneToValuesAndReturnsOccurences) {
    server->insertNewElement("Keith", 11);
    server->insertNewElement("Thomas", 12);

    auto [found_1, hits_1] = server->incrementValue(10);
    ASSERT_FALSE(found_1);
    
    auto [found_2, hits_2] = server->incrementValue(11);

    ASSERT_TRUE(found_2);
    ASSERT_EQ(2, hits_2);
}
