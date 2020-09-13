#include "ServerSQL.hpp"

#include <iostream>

using namespace std::string_literals;

ServerSQL::ServerSQL(session& sql) : sql_(sql) {}

void ServerSQL::insertNewElement(std::unique_ptr<std::string>& output, const std::string& key, int value) {
    sql_ << "INSERT INTO " << tableName_ << " " << "VALUES(:k, :v)",
        use(key, "k"), use(value, "v");

    *output = OK_;
}

// void ServerSQL::getValue(std::unique_ptr<std::string>& output, const std::string& key) {
//     rowset<row> rs = (sql_.prepare << "SELECT * FROM " << tableName_ <<
//                                       " WHERE id = '" << key << "'");
    
//     if (rs.begin() == rs.end()) {
//         tokens_.push_back(notOK_);
//         return;
//     }

//     tokens_.push_back(OK_);

//     for (rowset<row>::const_iterator it = rs.begin(); it != rs.end(); ++it) {
//         const row& r = *it;
//         tokens_.push_back(std::to_string(r.get<int>(1)));
//     }

// }

// Answers ServerSQL::deleteElement(const std::string& key) {
//     sql_ << "DELETE FROM " << tableName_ << " " << "WHERE id = '" << key << "'";

//     return {OK_};
// }

// Answers ServerSQL::getOccurences(int value) const {
//     Answers answer;
//     int count;

//     sql_ << "SELECT COUNT(*) FROM " << tableName_ << " WHERE value = " << value, into(count);

//     if (count) {
//         answer.push_back(OK_);
//         answer.push_back({"hits"s, std::to_string(count)});
//     }
//     else {
//         answer.push_back(NOT_OK_);
//     }

//     return answer;
// }

// Answers ServerSQL::incrementValue(int value) {
//     Answers answer = getOccurences(value);

//     sql_ << "UPDATE " << tableName_ << " SET value = value + 1";
//     return answer;
// }
