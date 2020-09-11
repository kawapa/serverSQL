#include "Server.hpp"

#include <algorithm>
#include <cctype>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>

using Answers = std::vector<std::pair<std::string, std::string>>;
using namespace std::chrono_literals;

Server::Server(ServerSQL& serverSQL, boost::asio::io_context& io_context, short port)
    : serverSQL_(serverSQL),
      socket_(io_context, udp::endpoint(udp::v4(), port))
{
    whenServerStarted_ = std::chrono::steady_clock::now();
    receive();
}

void Server::receive() {
    socket_.async_receive_from(
        boost::asio::buffer(data_, MAX_MESSAGE_LENGTH), sender_endpoint_,
        [this](boost::system::error_code error, std::size_t bytesReceived) {
            if (!error && bytesReceived > 0) {
                data_ = processQuery(data_);
                send(strlen(data_));
            } else {
                receive();
            }
        });
}

void Server::send(std::size_t length) {
    socket_.async_send_to(
        boost::asio::buffer(data_, length), sender_endpoint_,
        [this](boost::system::error_code /*ec*/, std::size_t /*bytes_sent*/) {
            receive();
        });
}

void Server::processQuery() {
    if (std::equal(std::begin(data_), std::end(data_), "STAT")) {
        displayStatus();
    }

    reader_.parse(query, obj_);
    std::string command = obj_["cmd"].asString();
    char answer[MAX_MESSAGE_LENGTH];

    if (command == "WRITE") {
        answer = serverSQL_.insertNewElement(obj_["args"]["key"].asString(),
                                             obj_["args"]["value"].asUInt());
    } else if (command == "READ") {
        answer = serverSQL_.getValue(obj_["args"]["key"].asString());
    } else if (command == "DEL") {
        answer = serverSQL_.deleteElement(obj_["args"]["key"].asString());
    } else if (command == "GET") {
        answer = serverSQL_.getOccurences(obj_["args"]["number"].asUInt());
    } else if (command == "INC") {
        answer = serverSQL_.incrementValue(obj_["args"]["number"].asUInt());
    } else {
        return ERROR_;
    }

    return generateAnswer(answer);
}

std::string Server::generateAnswer(const Answers& answer) {
    Json::Value val;
    std::for_each(begin(answer), end(answer), [&val](const auto& pair) {
        val[pair.first] = pair.second;
    });

    Json::FastWriter fast;
    return fast.write(val);
}

void Server::displayStatus(char* message) {
    auto now = std::chrono::steady_clock::now();
    auto result = std::chrono::duration_cast<std::chrono::seconds>(now - whenServerStarted_).count();

    std::stringstream ss;
    ss << "Server has started " << result << " seconds ago";
    data_ = ss.str().c_str();
}

void Server::terminateConnection(char* message) {
    data_ = OK_;
    socket_.close();
}

void Server::sleepFor(char* message, int seconds) {
    std::this_thread::sleep_for(30s);
    data_ = OK_;
}
