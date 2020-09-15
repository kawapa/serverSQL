#pragma once

#include <soci/soci.h>
#include <soci/mysql/soci-mysql.h>
#include <string>
#include <utility>

using namespace soci;

class ServerSQL {
public:
    ServerSQL(session& sql);

    bool deleteElement(const std::string& key);
    bool insertNewElement(const std::string& key, int value);
    std::pair<bool, int> getValue(const std::string& key);
    std::pair<bool, int> incrementValue(int value);
    int getOccurences(int value) const;

private:
    void createLogTable();

    session& sql_;

    std::string login_;
    std::string password_;
    std::string databaseName_;
    std::string tableName_ = "my_table";
    std::string logTableName_;
};
