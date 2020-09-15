#include <boost/asio.hpp>
#include <cstring>
#include <iostream>

#include "Parser.hpp"

constexpr size_t MAX_MESSAGE_LENGTH = 512;

using namespace std::string_literals;
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
        std::system("clear");

        bool keepGoing = true;
        while (keepGoing) {
            char request[MAX_MESSAGE_LENGTH];
            
            std::cout << "USER   >> ";
            std::cin.getline(request, MAX_MESSAGE_LENGTH);

            if (strcmp(request, "END") == 0) {
                keepGoing = false;
            }
            if (strcmp(request, "STAT") != 0 && strcmp(request, "END") != 0) {
                Parser::parseToServer(request);
            }
            socket.send_to(boost::asio::buffer(request, MAX_MESSAGE_LENGTH), *endpoints.begin());

            udp::endpoint sender_endpoint;
            char reply[MAX_MESSAGE_LENGTH];
            size_t reply_length = socket.receive_from(boost::asio::buffer(reply), sender_endpoint);
        
            std::cout << "SERVER >> ";
            std::cout.write(reply, reply_length);
            std::cout << "\n";
        }
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    std::cout << "Exiting the client...\n";

    return 0;
}
