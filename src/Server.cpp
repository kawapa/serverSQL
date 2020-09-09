#include "Server.hpp"

#include <fstream>
#include <iostream>

using namespace std::chrono_literals;

Server::Server() {
    whenServerStarted_ = std::chrono::steady_clock::now();
}

void Server::processJSON() {
    std::ifstream ifs("1.json");
    reader_.parse(ifs, obj_);

    auto command = obj_["cmd"].asString();
    if (command == "STATS") {
        std::cout << "znaleziono STATS" << std::endl;
    }
 
    const Json::Value& parameters = obj_["args"];
    for (int i = 0; i < parameters.size(); i++){
        std::cout << "arg[" << i << "] "<< parameters[i]["name"].asString();
        std::cout << " chapter: " << parameters[i]["chapter"].asUInt();
        std::cout << std::endl;
    }
}
