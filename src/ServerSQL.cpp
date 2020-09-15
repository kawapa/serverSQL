#include "ServerSQL.hpp"

#include <iterator>
#include <soci/soci.h>
#include <soci/mysql/soci-mysql.h>
#include <sstream>

using namespace std::string_literals;

ServerSQL::ServerSQL(const std::string& connectString,
                     const std::string& databaseName,
                     const std::string& tableName)
    : sql_(mysql, connectString),
      databaseName_(databaseName),
      tableName_(tableName)
{
    logTableName_ = tableName_ + "_log";
    createTables();
}

ServerSQL::~ServerSQL() {
    deleteTables();
}

bool ServerSQL::deleteElement(const std::string& key) {
    updateLogTable("DEL"s, {key, ""});

    int valueExists = 0;
    sql_ << "SELECT COUNT(*) FROM " << tableName_ << " WHERE id = '" << key << "'", into(valueExists);

    if (!valueExists) {
        return false;
    }
    sql_ << "DELETE FROM " << tableName_ << " " << "WHERE id = '" << key << "'";

    return true;
}

bool ServerSQL::insertNewElement(const std::string& key, int value) {
    updateLogTable("WRITE"s, {key, std::to_string(value)});

    sql_ << "INSERT INTO " << tableName_ << " " << "VALUES(:k, :v)",
        use(key, "k"), use(value, "v");

    return true;
}

std::pair<bool, int> ServerSQL::getValue(const std::string& key) {
    updateLogTable("READ"s, {key, ""});

    int valueExists = 0;
    sql_ << "SELECT COUNT(*) FROM " << tableName_ << " WHERE id = '" << key << "'", into(valueExists);

    if (!valueExists) {
        return {false, 0};
    }

    rowset<row> rs = (sql_.prepare << "SELECT (value) FROM " << tableName_ << " WHERE id = \"" << key << "\"");
    
    int counter = 0;
    rowset<row>::const_iterator it = rs.begin();
    const row& r = *it;
    counter = r.get<int>(0);

    return {true, counter};
}

std::pair<bool, int> ServerSQL::incrementValue(int value) {
    updateLogTable("INC"s, {std::to_string(value), ""});

    int valueExists = 0;
    sql_ << "SELECT COUNT(*) FROM " << tableName_ << " WHERE value = " << value, into(valueExists);

    if (!valueExists) {
        return {false, 0};
    }
    sql_ << "UPDATE " << tableName_ << " SET value = (value + 1) WHERE value = " << value;

    int occurences = 0;
    sql_ << "SELECT COUNT(*) FROM " << tableName_ << " WHERE value = " << value + 1, into(occurences);

    return {true, occurences};
}

int ServerSQL::getOccurences(int value) {
    updateLogTable("GET"s, {std::to_string(value), ""});
    
    int count = 0;
    sql_ << "SELECT COUNT(*) FROM " << tableName_ << " WHERE value = " << value, into(count);

    return count;
}

void ServerSQL::createTables() {
    sql_ << "CREATE TABLE IF NOT EXISTS " << tableName_ << " (id VARCHAR(50), value INT)";

    sql_ << "CREATE TABLE IF NOT EXISTS " << logTableName_ << " ( id int AUTO_INCREMENT, command varchar(75), PRIMARY KEY (id))";
    sql_ << "ALTER TABLE " << logTableName_ <<" AUTO_INCREMENT=1";
}

void ServerSQL::deleteTables() {
    sql_ << "DROP TABLE " << tableName_;
    sql_ << "DROP TABLE " << logTableName_;
}

void ServerSQL::updateLogTable(const std::string& cmd, const std::pair<std::string, std::string>& parameters) {
    std::stringstream ss;
    ss << cmd << " " << parameters.first;
    if (parameters.second != "")
        ss << " " << parameters.second;
    
    sql_ << "INSERT INTO " << logTableName_ << " (command) VALUES(:v)", use(ss.str(), "v");
}
