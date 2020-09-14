#pragma once

#include <soci/soci.h>
#include <soci/mysql/soci-mysql.h>
#include <jsoncpp/json/json.h>
#include <memory>
#include <string>
#include <vector>

using namespace std::string_literals;
using namespace soci;

class ServerSQL {
public:
    ServerSQL(session& sql);

    void insertNewElement(std::unique_ptr<std::string>& output, const std::string& key, int value);
    void getValue(std::unique_ptr<std::string>& output, const std::string& key);
    void deleteElement(std::unique_ptr<std::string>& output, const std::string& key);
    void getOccurences(std::unique_ptr<std::string>& output, int value) const;
    void incrementValue(std::unique_ptr<std::string>& output, int value);

private:
    session& sql_;

    std::unique_ptr<std::vector<std::string>> tokens_;

    std::string login_;
    std::string password_;
    std::string databaseName_;
    std::string tableName_ = "my_table";
};
