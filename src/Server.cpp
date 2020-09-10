#include "Server.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>

using Answers = std::vector<std::pair<std::string, std::string>>;
using namespace std::chrono_literals;

Server::Server(ServerSQL& serverSQL) : serverSQL_(serverSQL) {
    whenServerStarted_ = std::chrono::steady_clock::now();
}

std::string Server::processQuery(const std::string& query) {
    if (query == "STAT") {
        return displayStatus();
    }

    reader_.parse(query, obj_);
    std::string command = obj_["cmd"].asString();
    Answers answer;

    if (command == "WRITE") {
        answer = serverSQL_.insertNewElement(obj_["args"]["key"].asString(),
                                             obj_["args"]["value"].asUInt());
    }
    else if (command == "READ") {
        answer = serverSQL_.getValue(obj_["args"]["key"].asString());
    }
    else if (command == "DEL") {
        answer = serverSQL_.deleteElement(obj_["args"]["key"].asString());
    }
    else if (command == "GET") {
        answer = serverSQL_.getOccurences(obj_["args"]["number"].asUInt());
    }
    else if (command == "INC") {
        answer = serverSQL_.incrementValue(obj_["args"]["number"].asUInt());
    }
    else {
        return ERROR_;
    }

    return generateAnswer(answer);
}

std::string Server::generateAnswer(const Answers& answer) {
    Json::Value val;
    std::for_each(begin(answer), end(answer), [&val](const auto& pair){
        val[pair.first] = pair.second;
    });

    Json::FastWriter fast;
    std::string sFast = fast.write(val);

    return sFast;
}

std::string Server::displayStatus() {
    // std::this_thread::sleep_for(5s);
    auto now = std::chrono::steady_clock::now();
    auto result = std::chrono::duration_cast<std::chrono::seconds>(now - whenServerStarted_).count();

    std::stringstream ss;
    ss << "Server has started " << result << " seconds ago";

    return ss.str();
}

std::string Server::terminateConnection() {

    return OK_;
}


std::string Server::sleepFor(int seconds) {
    std::this_thread::sleep_for(30s);

    return OK_;
}