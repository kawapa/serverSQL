#pragma once

#include "ServerSQL.hpp"

#include <chrono>
#include <jsoncpp/json/json.h>
#include <utility>
#include <vector>

using Answers = std::vector<std::pair<std::string, std::string>>;

class Server {
public:
    Server(ServerSQL& serverSQL);

    std::string processQuery(const std::string& query);
    std::string displayStatus();
    std::string terminateConnection();
    std::string sleepFor(int seconds);

private:
    Json::Reader reader_;
    Json::Value obj_;
    Json::StyledStreamWriter writer_;

    std::string generateAnswer(const Answers& answer);
    std::chrono::steady_clock::time_point whenServerStarted_;
    // int connectionCounter = 0;
    // int activeConnections = 0;

    ServerSQL& serverSQL_;
    const std::string ERROR_ = R"({ status: "error" })";
    const std::string OK_ = R"({ "status": "ok" })";   
};
