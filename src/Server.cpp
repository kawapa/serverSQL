#include "Server.hpp"

#include <thread>

#include "Parser.hpp"

constexpr size_t MAX_MESSAGE_LENGTH = 512;

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
                queriesReceived_++;
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
        bool inserted = serverSQL_.insertNewElement(obj_["args"]["key"].asString(),
                                    obj_["args"]["value"].asInt());
        Parser::parseToClient(output_, inserted);
    }
    else if (cmd == "READ") {
        auto exists = serverSQL_.getValue(obj_["args"]["key"].asString());
        Parser::parseToClient(output_, cmd, std::to_string(exists.second));
    }
    else if (cmd == "DEL") {
        bool deleted = serverSQL_.deleteElement(obj_["args"]["key"].asString());
        Parser::parseToClient(output_, deleted);
    }
    else if (cmd == "GET") {
        int occurences = serverSQL_.getOccurences(obj_["args"]["number"].asUInt());
        Parser::parseToClient(output_, cmd, std::to_string(occurences));
    }
    else if (cmd == "INC") {
        auto exists = serverSQL_.incrementValue(obj_["args"]["number"].asUInt());
        if (!exists.first) {
            Parser::parseToClient(output_, false);
        }
        else {
            Parser::parseToClient(output_, cmd, std::to_string(exists.second));
        }
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

void Server::showStatistics() { Parser::parseToClient(output_, getTimeFromStart(), getQueriesReceived()); }

int64_t Server::getTimeFromStart() const {
    auto now = std::chrono::steady_clock::now();
    auto howLongActive = std::chrono::duration_cast<std::chrono::seconds>
                                (now - whenServerStarted_).count();

    return howLongActive;
}

int Server::getQueriesReceived() const { return queriesReceived_; }
