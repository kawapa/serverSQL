#include "Server.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>

using Answers = std::vector<std::pair<std::string, std::string>>;
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
    if (command == "READ") {
        if (serverSQL_.getValue(obj_["args"]["key"].asString())) {
            return OK_;
        }
    }

    return ERROR_;
}

void generateAnswer(const Answers& answer) {
    Json::Value val;
    std::for_each(begin(answer), end(answer), [&val](const auto& pair){
        val[p.first] = p.second;
    });

    Json::FastWriter fast;
    string sFast = fast.write(val);

    cout << sFast << '\n';
}