#include "ServerSQL.hpp"

#include <iostream>
#include <algorithm>
#include <sstream>
#include <iterator>

#include "Parser.hpp"

using namespace std::string_literals;

ServerSQL::ServerSQL(session& sql) : sql_(sql) {}

void ServerSQL::insertNewElement(std::unique_ptr<std::string>& output, const std::string& key, int value) {
    sql_ << "INSERT INTO " << tableName_ << " " << "VALUES(:k, :v)",
        use(key, "k"), use(value, "v");

    Parser::parseToClient(output, true);
}

void ServerSQL::getValue(std::unique_ptr<std::string>& output, const std::string& key) {
    int valueExists = 0;
    sql_ << "SELECT COUNT(*) FROM " << tableName_ << " WHERE id = '" << key << "'", into(valueExists);

    if (!valueExists) {
        Parser::parseToClient(output, "READ", "");
        return;
    }

    //std::stringstream ss;
    // // ss << "SELECT * FROM " << tableName_ << " WHERE id='" << key << "'";
    //ss << "SELECT (value) FROM " << tableName_ << " WHERE id = \"" << key << "\"";
    // //ss << "SELECT (value) FROM " << tableName_ ;
    //auto str = ss.str();

    //rowset<row> rs = (sql_.prepare << str);

    rowset<row> rs = (sql_.prepare << "SELECT (value) FROM " << tableName_ << " WHERE id = \"" << key << "\"");
    
    // if (rs.begin() == rs.end()) {
    //     Parser::parseToClient(output, false);
    //     return;
    // }

    // rowset<int>::const_iterator it = rs.begin();
    // std::advance(it, 1);
    // //tokens_->push_back(std::to_string(*it));

    int counter = 0;
    rowset<row>::const_iterator it = rs.begin();
    const row& r = *it;
    counter += r.get<int>(0);

    // for (rowset<row>::const_iterator it = rs.begin(); it != rs.end(); ++it) {
    //     const row& r = *it;
    //     counter += r.get<int>(0);
    // }

    // *output = std::to_string(counter);

    Parser::parseToClient(output, "READ", {std::to_string(counter)});
}

void ServerSQL::deleteElement(std::unique_ptr<std::string>& output, const std::string& key) {
    // int valueExists = 0;
    // sql_ << "SELECT COUNT(*) FROM " << tableName_ << " WHERE value = " << value, into(valueExists);

    // if (!valueExists) {
    //     Parser::parseToClient(output, false);
    //     return;
    // }

    sql_ << "DELETE FROM " << tableName_ << " " << "WHERE id = '" << key << "'";

    Parser::parseToClient(output, true);
}

void ServerSQL::getOccurences(std::unique_ptr<std::string>& output, int value) const {
    int count = 0;
    sql_ << "SELECT COUNT(*) FROM " << tableName_ << " WHERE value = " << value, into(count);

    Parser::parseToClient(output, "GET", {std::to_string(count)});
}

void ServerSQL::incrementValue(std::unique_ptr<std::string>& output, int value) {
    int valueExists = 0;
    sql_ << "SELECT COUNT(*) FROM " << tableName_ << " WHERE value = " << value, into(valueExists);

    if (!valueExists) {
        Parser::parseToClient(output, false);
        return;
    }
    sql_ << "UPDATE my_table SET value = (value + 1) WHERE value = " << value;

    int occurences = 0;
    sql_ << "SELECT COUNT(*) FROM " << tableName_ << " WHERE value = " << value + 1, into(occurences);

    std::stringstream ss;
    ss << "Licznik " << value + 1 << " to: " << occurences;

    Parser::parseToClient(output, "INC", {std::to_string(occurences)});
}
