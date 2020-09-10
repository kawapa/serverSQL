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

// bool ServerSQL::insertNewElement(const std::string& key, int value) {
//     sql_ << "INSERT INTO users(first_name, last_name, email) VALUES(:fn, :ln, :e)",
//         use(first_name, "fn"), use(last_name, "ln"), use(email, "e");
//     return true;
// }

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