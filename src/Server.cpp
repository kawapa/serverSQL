#include "Server.hpp"

#include <fstream>
#include <iostream>

using namespace std::chrono_literals;

Server::Server(ServerSQL& serverSQL) : serverSQL_(serverSQL) {
    // whenServerStarted_ = std::chrono::steady_clock::now();
}

std::string Server::processQuery(const std::string& query) {
    reader_.parse(query, obj_);
    std::string command = obj_["cmd"].asString();

    if (command == "WRITE") {
        if (serverSQL_.insertNewElement(obj_["args"]["key"].asString(),
                                        obj_["args"]["value"].asUInt())) {
            return OK_;
        }
    }

    return ERROR_;
}
