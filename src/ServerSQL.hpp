#pragma once

#include <soci/soci.h>
#include <soci/mysql/soci-mysql.h>
#include <jsoncpp/json/json.h>
#include <string>

using namespace soci;

class ServerSQL {
public:
    ServerSQL(session& sql);

    bool insertNewElement(const std::string& key, int value); // WRITE
    int getValue(const std::string& key); // READ
    bool deleteElement(const std::string& key);  // DEL
    bool incrementValue(int value); // INC
    int getOccurences(const std::string& query); // GET

private:
    session& sql_;

    std::string login_;
    std::string password_;
    std::string databaseName_;
    std::string tableName_ = "my_table";
};
