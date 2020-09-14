#include <boost/asio.hpp>
#include <soci/soci.h>
#include <soci/mysql/soci-mysql.h>
#include <exception>
#include <iostream>
#include <string>

#include "Server.hpp"
#include "ServerSQL.hpp"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Not enough arguments\n";
        std::cerr << "Proper usage: ./server <connection_port>\n";
        return -1;
    }

    try {
        // std::string login, password, database;
        // std::cout << "Enter your LOGIN to MySQL server; ";
        // std::cin >> login;
        // std::cout << "Enter your PASSWORD to MySQL server; ";
        // std::cin >> password;
        // std::cout << "Enter the name of the database you'd like to access: ";
        // std::cin >> database;
        // std::cout << "Enter the name of the table. Please note the table must have\n";
        // std::cout << "... at least two columns: 'id' (for a string) and 'value' (for an integer).\n";
        // std::cout << "Table name: ";
        // std::cin >> tableName_;

        boost::asio::io_context io_context;

        session sql(mysql, "db=db user=root password=stokrotka");
        ServerSQL serverSQL(sql);

        Server server(serverSQL, io_context, std::atoi(argv[1]));

        io_context.run();

    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}

/*
TESTS:

    // std::string write = R"({ "cmd": "WRITE", "args": { "key": "test key", "value": 11 }})"; 
    // std::cout << server.processQuery(write);

    // std::string read = R"({ "cmd": "READ", "args": { "key": "test key" }})"; 
    // std::cout << server.processQuery(read);

    // std::string del = R"({ "cmd": "DEL", "args": { "key": "test id" }})"; 
    // std::cout << server.processQuery(del);

    // std::string get1 = R"({ "cmd": "GET", "args": { "number": 1 }})"; 
    // std::cout << server.processQuery(get1);

    // std::string inc = R"({ "cmd": "INC", "args": { "number": 4 }})"; 
    // std::cout << server.processQuery(inc);

    // std::string stat = "STAT"; 
    // std::cout << server.processQuery(stat);
*/