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
        // std::string login, password, database, table;
        // std::cout << "Enter your LOGIN to MySQL server: ";
        // std::cin >> login;
        // std::cout << "Enter your PASSWORD to MySQL server: ";
        // std::cin >> password;
        // std::cout << "Enter the name of the database you'd like to access: ";
        // std::cin >> database;
        // std::cout << "Enter the name of the table:
        // std::cin >> table;
        // std::string table_log = table + "_log";

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
