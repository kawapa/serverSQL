#include "ServerSQL.hpp"

#include <iostream>

using namespace std::string_literals;

ServerSQL::ServerSQL(session& sql) : sql_(sql) {}

Answers ServerSQL::insertNewElement(const std::string& key, int value) {
    sql_ << "INSERT INTO " << tableName_ << " " << "VALUES(:k, :v)",
        use(key, "k"), use(value, "v");
    return {OK_};
}

Answers ServerSQL::getValue(const std::string& key) {
    rowset<row> rs = (sql_.prepare << "SELECT * FROM " << tableName_ <<
                                      " WHERE id = '" << key << "'");
    
    Answers answer;
    if (rs.begin() == rs.end()) {
        answer.push_back(NOT_OK_);
        return answer;
    }
    answer.push_back(OK_);

    for (rowset<row>::const_iterator it = rs.begin(); it != rs.end(); ++it) {
        const row& r = *it;
        //std::cout << r.get<std::string>(0) << " 1 " << std::endl;
        //std::cout << r.get<int>(1) << " 2 " << std::endl;
        answer.push_back({"value"s, std::to_string(r.get<int>(1))});
    }

    return answer;
}

Answers ServerSQL::deleteElement(const std::string& key) {
    sql_ << "DELETE FROM " << tableName_ << " " << "WHERE id = '" << key << "'";

    return {OK_};
}

Answers ServerSQL::getOccurences(int value) {
    Answers answer;
    int count;

    sql_ << "SELECT COUNT(*) FROM " << tableName_ << " WHERE value = " << value, into(count);

    if (count) {
        answer.push_back(OK_);
        answer.push_back({"hits"s, std::to_string(count)});
    }
    else {
        answer.push_back(NOT_OK_);
    }

    return answer;
}

Answers ServerSQL::incrementValue(int value) {
    Answers answer = getOccurences(value);

    sql_ << "UPDATE " << tableName_ << " SET value = value + 1";
    return answer;
}
