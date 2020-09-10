#include <soci/soci.h>
#include <soci/mysql/soci-mysql.h>
#include <exception>
#include <iostream>
#include <string>

#include "Server.hpp"
#include "ServerSQL.hpp"

using namespace std::string_literals;

int main() {
// int main(int argc, char** argv) {
    // if (argc != 5) {
    //     return -1;
    // }

    // int repetitions = std::stoi(argv[1]);
    // int timeout = std::stoi(argv[2]);

    session sql(mysql, "db=db user=root password=stokrotka");
    ServerSQL serverSQL(sql);
    Server server(serverSQL);



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

    std::string stat = "STAT"; 
    std::cout << server.processQuery(stat);

    // try {

    // }
    // catch (const std::exception& e) {
    //     std::cerr << "Error: " << e.what() << endl; 
    // }

    return 0;
}