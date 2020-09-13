#include "Server.hpp"

#include <algorithm>
#include <cctype>
#include <cstring>
#include <fstream>
#include <iostream>
#include <thread>

#include "Parser.hpp"

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

    if (strcmp(input_.get(), "END") == 0) {
        *output_ = R"({ "status": "ok" })";
        return;
    }
    else if (strcmp(input_.get(), "STAT") == 0) {
        *output_ = R"({ "status": "ok" })";
        return;
    }

    reader_.parse(input_.get(), obj_);
    std::string command = obj_["cmd"].asString();

    if (command == "WRITE") {
        serverSQL_.insertNewElement(output_,
                                    obj_["args"]["key"].asString(),
                                    obj_["args"]["value"].asInt());       
    }
    // else if (command == "READ") {
    //     serverSQL_.getValue(output_,
    //                         obj_["args"]["key"].asString());
    // } else if (command == "DEL") {
    //     serverSQL_.deleteElement(data_,
    //                              obj_["args"]["key"].asString());
    // } else if (command == "GET") {
    //     serverSQL_.getOccurences(data_,
    //                              obj_["args"]["number"].asUInt());
    // } else if (command == "INC") {
    //     serverSQL_.incrementValue(data_,
    //                               obj_["args"]["number"].asUInt());
    else if (command == "END") {
        *output_ = R"({ "status": "ok" })";
        // boost::system::error_code ignored_ec;
        // socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_receive, ignored_ec);
        // socket_.close();       
    }
    else {
        *output_ = R"({ "status": "error" })";
    }
}

void Server::displayStatus() {
    auto now = std::chrono::steady_clock::now();
    auto result = std::chrono::duration_cast<std::chrono::seconds>(now - whenServerStarted_).count();

    //Parser::generateAnswer(data_, "Server has started ", result, " seconds ago");
}

// void Server::terminateConnection() {
//     socket_.close();
//     Parser::generateAnswer(data_, true);
// }

// void Server::sleepFor(int seconds) {
//     std::this_thread::sleep_for(30s);
//     Parser::generateAnswer(data_, true);
// }
