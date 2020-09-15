#include <boost/asio.hpp>
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
        std::string login, password, databaseName, tableName;

        std::system("clear");
        std::cout << "Enter your LOGIN to MySQL server: ";
        std::cin >> login;
        std::cout << "Enter your PASSWORD to MySQL server: ";
        std::cin >> password;
        std::cout << "Enter the name of the EXISTING database you'd like to access: ";
        std::cin >> databaseName;
        std::cout << "Table name (will be created if not exists): ";
        std::cin >> tableName;

        std::stringstream ss;
        ss << "db=" << databaseName << " user=" << login << " password=" << password;
        ServerSQL serverSQL(ss.str(), databaseName, tableName);

        boost::asio::io_context io_context;
        Server server(serverSQL, io_context, std::atoi(argv[1]));
        std::cout << "Server is running...\n";
        io_context.run();

    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
