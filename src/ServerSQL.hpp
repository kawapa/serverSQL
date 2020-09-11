#pragma once

#include <soci/soci.h>
#include <soci/mysql/soci-mysql.h>
#include <jsoncpp/json/json.h>
#include <string>

constexpr size_t MAX_MESSAGE_LENGTH = 512;

using Answers = std::vector<std::pair<std::string, std::string>>;
using namespace soci;

class ServerSQL {
public:
    ServerSQL(session& sql);

    char* insertNewElement(const std::string& key, int value);
    Answers getValue(const std::string& key) const;
    Answers deleteElement(const std::string& key);
    Answers getOccurences(int value) const;
    Answers incrementValue(int value);

private:
    session& sql_;

    std::string login_;
    std::string password_;
    std::string databaseName_;
    std::string tableName_ = "my_table";

    char* OK_ = "ok";
    char* notOK_ = "error";
};
