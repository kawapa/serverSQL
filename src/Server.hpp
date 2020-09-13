#pragma once

#include "ServerSQL.hpp"

#include <boost/asio.hpp>
#include <chrono>
#include <cstdlib>
#include <memory>
#include <jsoncpp/json/json.h>
#include <utility>
#include <vector>

using boost::asio::ip::udp;

constexpr size_t MAX_MESSAGE_LENGTH = 512;

class Server {
public:
    Server(ServerSQL& serverSQL, boost::asio::io_context& io_context, short port);

    void receive();
    void send();

    void processQuery();
    void displayStatus();
    // void terminateConnection();
    // void sleepFor(int seconds);

private:
    Json::Reader reader_;
    Json::Value obj_;
    Json::StyledStreamWriter writer_;

    udp::socket socket_;
    udp::endpoint sender_endpoint_;

    std::unique_ptr<char[]> input_;
    std::unique_ptr<std::string> output_;

    std::chrono::steady_clock::time_point whenServerStarted_;
    // int connectionCounter = 0;
    // int activeConnections = 0;

    ServerSQL& serverSQL_;
};
