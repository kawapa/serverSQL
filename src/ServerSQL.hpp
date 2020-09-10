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

    Answers insertNewElement(const std::string& key, int value); // WRITE
    Answers getValue(const std::string& key); // READ
    Answers deleteElement(const std::string& key);  // DEL
    Answers getOccurences(int value); // GET
    Answers incrementValue(int value); // INC

private:
    session& sql_;

    std::string login_;
    std::string password_;
    std::string databaseName_;
    std::string tableName_ = "my_table";

    const std::pair<std::string, std::string> OK_ = {"status", "ok"};
    const std::pair<std::string, std::string> NOT_OK_ = {"status", "error"};
};
