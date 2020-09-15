#include <gtest/gtest.h>

#include "Parser.hpp"

constexpr size_t MAX_MESSAGE_LENGTH = 512;

struct ParserTests : public ::testing::Test {
public:
    ParserTests() {
        outputString = std::make_unique<std::string>();
        expectedString = std::make_unique<std::string>();
    }

    std::unique_ptr<std::string> outputString;
    std::unique_ptr<std::string> expectedString;

    char outputCharArray[MAX_MESSAGE_LENGTH];
    char expectedCharArray[MAX_MESSAGE_LENGTH];
};

TEST_F(ParserTests, parseToServerTransformsMessageIntoJSON) {
    // GIVEN
    strcpy(outputCharArray, "WRITE Jamie 20");

    // WHEN
    Parser::parseToServer(outputCharArray);

    // THEN
    strcpy(expectedCharArray, "{\"args\":{\"key\":\"Jamie\",\"value\":20},\"cmd\":\"WRITE\"}"); 

    ASSERT_TRUE(strcmp(outputCharArray, expectedCharArray));
}

TEST_F(ParserTests, parseToClientCreatesJSONWithSimpleStatus) {
    *outputString = "";

    Parser::parseToClient(outputString, true);

    *expectedString = R"({"status":"ok"})";
    ASSERT_EQ(*outputString, *expectedString);
}
