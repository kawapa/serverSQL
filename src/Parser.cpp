#include "Parser.hpp"

#include <algorithm>
#include <cstring>
#include <iterator>
#include <sstream>
#include <vector>

using namespace std::string_literals;

std::string Parser::OK_ = R"({"status":"ok"})";
std::string Parser::notOK_ = R"({"status":"error"})";

void Parser::parseToServer(char* messageString) {
    Json::Value root;
    Json::FastWriter fast;

    std::istringstream iss(messageString);
    std::vector<std::string> tokens;

    std::copy(std::istream_iterator<std::string>(iss),
              std::istream_iterator<std::string>(),
              back_inserter(tokens));

    root["cmd"] = tokens[0];
    if (tokens[0] == "WRITE") {
        for (size_t i = 1; i < tokens.size(); ) {
            root["args"]["key"] = tokens[i];
            root["args"]["value"] = std::stoi(tokens[i + 1]);
            i += 2;
        }
    }
    else if (tokens[0] == "SLEEP") {
        root["args"]["delay"] = std::stoi(tokens[1]);
    }
    else if (tokens[0] == "INC" || tokens[0] == "GET") {
        root["args"]["number"] = std::stoi(tokens[1]);
    }
    else if (tokens[0] == "READ" || tokens[0] == "DEL") {
        root["args"]["key"] = tokens[1];
    }

    auto jsonString = fast.write(root);
    strcpy(messageString, jsonString.c_str());
}

void Parser::parseToClient(std::unique_ptr<std::string>& output, bool status) {
    if (status) {
        *output = OK_;
    }
    else {
        *output = notOK_;
    }
}

void Parser::parseToClient(std::unique_ptr<std::string>& output, uint64_t seconds, int queriesReceived) {
    std::stringstream ss;
    ss << "The server has started " << seconds << " seconds ago and replied to ";
    ss << queriesReceived << " queries so far.";

    *output = ss.str();
}

void Parser::parseToClient(std::unique_ptr<std::string>& output,
                           const std::string& cmd,
                           const std::string& argument)
{
    Json::Value root;
    Json::FastWriter fast;

    root["status"] = "ok";
    if (cmd == "READ") {
        root["args"]["value"] = argument;
    }
    else {
        root["args"]["hits"] = std::stoi(argument);
    }

    *output = fast.write(root);
    output->pop_back();
}
