#include "ServerSQL.hpp"

#include <iterator>

ServerSQL::ServerSQL(session& sql) : sql_(sql)
{
    createLogTable();
}

bool ServerSQL::deleteElement(const std::string& key) {
    int valueExists = 0;
    sql_ << "SELECT COUNT(*) FROM " << tableName_ << " WHERE id = '" << key << "'", into(valueExists);

    if (!valueExists) {
        return false;
    }

    sql_ << "DELETE FROM " << tableName_ << " " << "WHERE id = '" << key << "'";

    return true;
}

bool ServerSQL::insertNewElement(const std::string& key, int value) {
    sql_ << "INSERT INTO " << tableName_ << " " << "VALUES(:k, :v)",
        use(key, "k"), use(value, "v");

    return true;
}

std::pair<bool, int> ServerSQL::getValue(const std::string& key) {
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
    int valueExists = 0;
    sql_ << "SELECT COUNT(*) FROM " << tableName_ << " WHERE value = " << value, into(valueExists);

    if (!valueExists) {
        return {false, 0};
    }
    sql_ << "UPDATE my_table SET value = (value + 1) WHERE value = " << value;

    int occurences = 0;
    sql_ << "SELECT COUNT(*) FROM " << tableName_ << " WHERE value = " << value + 1, into(occurences);


    return {true, occurences};
}

int ServerSQL::getOccurences(int value) const {
    int count = 0;
    sql_ << "SELECT COUNT(*) FROM " << tableName_ << " WHERE value = " << value, into(count);

    return count;
}

void ServerSQL::createLogTable() {

}
