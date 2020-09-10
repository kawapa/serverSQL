#pragma once

#include "ServerSQL.hpp"

#include <chrono>
#include <jsoncpp/json/json.h>
#include <utility>
#include <vector>

class Server {
public:
    Server(ServerSQL& serverSQL);

    std::string processQuery(const std::string& query);

private:
    Json::Reader reader_;
    Json::Value obj_;
    Json::StyledStreamWriter writer_;

    void generateAnswer(const std::vector<std::pair<std::string, std::string>>& answer);
    // std::chrono::steady_clock::time_point whenServerStarted_;
    ServerSQL& serverSQL_;
    const std::string ERROR_ = R"({ status: "error" })";
    const std::string OK_ = R"({ "status": "ok" })";   
};
