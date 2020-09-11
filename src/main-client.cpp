#include <algorithm>
#include <boost/asio.hpp>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <iterator>
#include <jsoncpp/json/json.h>
#include <sstream>
#include <string>
#include <vector>

constexpr size_t MAX_MESSAGE_LENGTH = 512;

void prepareMessage(char* messageString);

using boost::asio::ip::udp;

int main(int argc, char* argv[]) {
    try {
        if (argc != 3) {
            std::cerr << "Not enough arguments\n";
            std::cerr << "Proper usage: ./client <host_IP> <connection_port>\n";
            return -1;
        }

        boost::asio::io_context io_context;

        udp::socket socket(io_context, udp::endpoint(udp::v4(), 0));

        udp::resolver resolver(io_context);
        udp::resolver::results_type endpoints =
            resolver.resolve(udp::v4(), argv[1], argv[2]);

        for (;;) {
            std::cout << "Command line: ";
            char request[MAX_MESSAGE_LENGTH];
            std::cin.getline(request, MAX_MESSAGE_LENGTH);
            prepareMessage(request);
            std::cout << "Client cpp before sending: " << request << std::endl;

            size_t request_length = std::strlen(request);
            socket.send_to(boost::asio::buffer(request, request_length), *endpoints.begin());

            char reply[MAX_MESSAGE_LENGTH];
            udp::endpoint sender_endpoint;
            size_t reply_length = socket.receive_from(boost::asio::buffer(reply, MAX_MESSAGE_LENGTH), sender_endpoint);
            std::cout << "Reply is: (lclient cpp after receiving)";
            std::cout.write(reply, reply_length);
            std::cout << "\n";
        }

    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}

void prepareMessage(char* messageString) {
    Json::Value root;
    Json::FastWriter fast;

    std::istringstream iss(messageString);
    std::vector<std::string> tokens;

    std::copy(std::istream_iterator<std::string>(iss),
              std::istream_iterator<std::string>(),
              back_inserter(tokens));

    root["cmd"] = tokens[0];
    for (int i = 1; i < tokens.size(); ) {
        root[tokens[i]] = tokens[i + 1];
        i += 2;
    }

    auto jsonString = fast.write(root);
    strcpy(messageString, jsonString.c_str());
}
