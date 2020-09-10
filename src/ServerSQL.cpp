#include "ServerSQL.hpp"

#include <iostream>
#include <soci/soci.h>
#include <soci/mysql/soci-mysql.h>

ServerSQL::ServerSQL(session& sql) : sql_(sql) {}

bool ServerSQL::insertNewElement(const std::string& key, int value) {
    sql_ << "INSERT INTO " << tableName_ << " " << "VALUES(:k, :v)",
        use(key, "k"), use(value, "v");
    return true;
}

bool ServerSQL::getValue(const std::string& key) {
    rowset<row> rs = (sql_.prepare << "SELECT * FROM " << tableName_ <<
                                      " WHERE id = '" << key << "'");
    
    // if (rs.begin() == rs.end()) {
    //     std::cout << R"({ status: "error" })";
    //     return false;
    // }
    for (rowset<row>::const_iterator it = rs.begin(); it != rs.end(); ++it) {
        const row& r = *it;
        std::cout << r.get<std::string>(0) << std::endl;
        std::cout << r.get<int>(1) << std::endl;
    }

    return true;
}



    // rowset<row> rs = (sql.prepare << "SELECT * FROM users"); 
    // // Iterate through the result set 
    // for (rowset<row>::const_iterator it = rs.begin(); 
    //          it != rs.end(); ++it) { 
    //     const row& r = *it;
    //     std::cout << "ID: " << r.get<int>(0) << endl 
    //         << "First Name: " << r.get<string>(1) << endl 
    //         << "Last Name: " << r.get<string>(2) << endl 
    //         << "Email: " << r.get<string>(3) << endl 
    //         << "Active: " << r.get<int>(4) << endl << endl; 

/*

WRITE
WRITE <klucz typu "string"> <wartość> - zapisuje do bazy wartość w danym kluczy (lub zmienia istniejący)

{ "cmd": "WRITE", "args": { "key": "test key", "value": "test value" }} -> { "status": "ok" }

READ
READ <klucz> - odsyła wartość klucza zapisaną w bazie (lub łańcuch pusty gdy nie ma)

{ "cmd": "READ", "args": { "key": "test key" }} -> { "status": "ok", "value": "test value" }

DEL
DEL <klucz> - usuwa wartość klucza zapisaną w bazie

{ "cmd": "DEL", "args": { "key": "test key" }} -> { "status": "ok" }

Inne nieznane
Jeżeli serwer otrzyma nieznane polecenie, powinień odesłać informację o błędzie:,

np. { status: "error" }

*/