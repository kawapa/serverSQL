#pragma once

#include <chrono>
#include <jsoncpp/json/json.h>

class Server {
public:
    Server();

    void processJSON();
    void print();

private:
    Json::Reader reader_;
    Json::Value obj_;
    Json::StyledStreamWriter writer_;

    std::chrono::steady_clock::time_point whenServerStarted_;
};
