#pragma once

#include "ServerSQL.hpp"

#include <atomic>
#include <boost/asio.hpp>
#include <chrono>
#include <memory>
#include <jsoncpp/json/json.h>

using boost::asio::ip::udp;

class Server {
public:
    Server(ServerSQL& serverSQL, boost::asio::io_context& io_context, short port);

    void receive();
    void send();

    void processQuery();
    void goSleepFor(int seconds) const;
    void showStatistics();


private:
    ServerSQL& serverSQL_;

    int getQueriesReceived() const;
    int64_t getTimeFromStart() const;

    udp::socket socket_;
    udp::endpoint sender_endpoint_;

    Json::Reader reader_;
    Json::Value obj_;
    Json::StyledStreamWriter writer_;

    std::unique_ptr<char[]> input_;
    std::unique_ptr<std::string> output_;

    std::atomic<int> queriesReceived_ {0};
    std::chrono::steady_clock::time_point whenServerStarted_;
};
