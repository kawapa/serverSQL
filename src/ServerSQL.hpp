#pragma once

#include <soci/soci.h>
#include <soci/mysql/soci-mysql.h>
#include <string>
#include <utility>

using namespace soci;

class ServerSQL {
public:
    ServerSQL(const std::string& connectString,
              const std::string& databaseName,
              const std::string& tableName);

    ~ServerSQL();

    bool deleteElement(const std::string& key);
    bool insertNewElement(const std::string& key, int value);
    std::pair<bool, int> getValue(const std::string& key);
    std::pair<bool, int> incrementValue(int value);
    int getOccurences(int value);

private:
    void createTables();
    void deleteTables();
    void updateLogTable(const std::string& cmd, const std::pair<std::string, std::string>& parameters);

    session sql_;

    std::string databaseName_;
    std::string tableName_;
    std::string logTableName_;
};
