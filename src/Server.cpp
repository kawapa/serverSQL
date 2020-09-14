#include "Server.hpp"

#include <algorithm>
#include <cctype>
#include <cstring>
#include <fstream>
#include <iostream>
#include <thread>

#include "Parser.hpp"

constexpr size_t MAX_MESSAGE_LENGTH = 512;

using namespace std::chrono_literals;
using namespace std::string_literals;

Server::Server(ServerSQL& serverSQL, boost::asio::io_context& io_context, short port)
    : serverSQL_(serverSQL),
      socket_(io_context, udp::endpoint(udp::v4(), port))
{
    whenServerStarted_ = std::chrono::steady_clock::now();
    receive();
}

void Server::receive() {
    input_ = std::make_unique<char[]>(MAX_MESSAGE_LENGTH);

    socket_.async_receive_from(
        boost::asio::buffer(input_.get(), MAX_MESSAGE_LENGTH), sender_endpoint_,
        [this](boost::system::error_code error, std::size_t bytesReceived) {
            if (!error && bytesReceived > 0) {
                processQuery();
                send();
                queriesReceived_++;
            } else {
                receive();
            }
    });
}

void Server::send() {
    socket_.async_send_to(
        boost::asio::buffer(*output_), sender_endpoint_,
        [this](boost::system::error_code, std::size_t) {
            receive();
        });
}

void Server::processQuery() {
    output_ = std::make_unique<std::string>();
    auto inputCMD = input_.get();

    if (strcmp(inputCMD, "END") == 0) {
        Parser::parseToClient(output_, true);
        return;
    }
    else if (strcmp(input_.get(), "STAT") == 0) {
        showStatistics();
        return;
    }

    reader_.parse(input_.get(), obj_);
    std::string cmd = obj_["cmd"].asString();

    if (cmd == "WRITE") {
        serverSQL_.insertNewElement(output_,
                                    obj_["args"]["key"].asString(),
                                    obj_["args"]["value"].asInt());       
    }
    else if (cmd == "READ") {
        serverSQL_.getValue(output_,
                            obj_["args"]["key"].asString());
    }
    else if (cmd == "DEL") {
        serverSQL_.deleteElement(output_,
                                 obj_["args"]["key"].asString());
    }
    else if (cmd == "GET") {
        serverSQL_.getOccurences(output_,
                                 obj_["args"]["number"].asUInt());
    }
    else if (cmd == "INC") {
        serverSQL_.incrementValue(output_,
                                  obj_["args"]["number"].asUInt());
    }
    else if (cmd == "SLEEP") {
        goSleepFor(obj_["args"]["delay"].asInt());
        Parser::parseToClient(output_, true);
    }
    else {
        Parser::parseToClient(output_, false);
    }
}

void Server::goSleepFor(int seconds) const {
    std::this_thread::sleep_for(std::chrono::milliseconds(seconds * 1000));
}

int64_t Server::getTimeFromStart() const {
    auto now = std::chrono::steady_clock::now();
    auto howLongActive = std::chrono::duration_cast<std::chrono::seconds>
                                (now - whenServerStarted_).count();

    return howLongActive;
}

int Server::getQueriesReceived() const { return queriesReceived_; }

void Server::showStatistics() { Parser::parseToClient(output_, getTimeFromStart(), getQueriesReceived()); }