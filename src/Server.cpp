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
        std::cout << "SZUKANA: " << obj_["args"]["number"].asUInt() << std::endl;
        answer = serverSQL_.getOccurences(obj_["args"]["number"].asUInt());
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

    // for (int i = 0; i < answer.size(); ++i) {
    //     std::cout << "i = " << i << std::endl;
    //     val[answer[i].first] = answer[i].second;
    // }

    Json::FastWriter fast;
    std::string sFast = fast.write(val);

    //std::cout << sFast << '\n';
    return sFast;
}
