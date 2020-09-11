#pragma once

#include "ServerSQL.hpp"

#include <boost/asio.hpp>
#include <chrono>
#include <cstdlib>
#include <jsoncpp/json/json.h>
#include <utility>
#include <vector>

using Answers = std::vector<std::pair<std::string, std::string>>;
using boost::asio::ip::udp;

constexpr size_t MAX_MESSAGE_LENGTH = 512;

class Server {
public:
    Server(ServerSQL& serverSQL, boost::asio::io_context& io_context, short port);

    void receive();
    void send(std::size_t length);

    void processQuery();
    void displayStatus();
    void terminateConnection();
    void sleepFor(int seconds);

private:
    Json::Reader reader_;
    Json::Value obj_;
    Json::StyledStreamWriter writer_;

    udp::socket socket_;
    udp::endpoint sender_endpoint_;
    char data_[MAX_MESSAGE_LENGTH];

    std::string generateAnswer(const Answers& answer);
    std::chrono::steady_clock::time_point whenServerStarted_;
    // int connectionCounter = 0;
    // int activeConnections = 0;

    ServerSQL& serverSQL_;
    char* OK_ = "ok";
    char* notOK_ = "error";
};
