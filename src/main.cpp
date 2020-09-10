#include <soci/soci.h>
#include <soci/mysql/soci-mysql.h>
#include <exception>
#include <iostream>
#include <string>

#include "Server.hpp"
#include "ServerSQL.hpp"

using namespace std::string_literals;

// int main(int argc, char** argv) {
int main() {
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

    std::string get1 = R"({ "cmd": "GET", "args": { "number": 1 }})"; 
    std::cout << server.processQuery(get1);

    std::string get2 = R"({ "cmd": "GET", "args": { "number": 3 }})"; 
    std::cout << server.processQuery(get2);

    std::string get3 = R"({ "cmd": "GET", "args": { "number": 4 }})"; 
    std::cout << server.processQuery(get3);

    // std::pair<std::string, std::string> p0("status", "ok");
    // std::pair<std::string, std::string> p1("Bobek", "Pierwszy");
    // std::pair<std::string, std::string> p2("Lobel", "Drugi");

    // std::cout << server.generateAnswer({p2, p1, p0});

    

    // try {

    // }
    // catch (const std::exception& e) {
    //     std::cerr << "Error: " << e.what() << endl; 
    // }

    return 0;
}