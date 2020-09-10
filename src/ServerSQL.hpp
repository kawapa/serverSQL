#pragma once

#include <soci/soci.h>
#include <soci/mysql/soci-mysql.h>
#include <jsoncpp/json/json.h>
#include <string>

using Answers = std::vector<std::pair<std::string, std::string>>;
using namespace soci;

class ServerSQL {
public:
    ServerSQL(session& sql);

    Answers insertNewElement(const std::string& key, int value);
    Answers getValue(const std::string& key); // READ
    Answers deleteElement(const std::string& key);  // DEL
    Answers incrementValue(int value); // INC
    Answers getOccurences(const std::string& query); // GET

private:
    session& sql_;

    std::string login_;
    std::string password_;
    std::string databaseName_;
    std::string tableName_ = "my_table";
};
