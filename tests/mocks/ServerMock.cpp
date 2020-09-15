#include "gmock/gmock.h"

#include "Server.hpp"

class ServerMock : public Server {
    ServerMock()
        : Server(ServerSQL& serverSQL, boost::asio::io_context& io_context, short port)
    {

    }

    MOCK_METHOD(void, receive, (), (override));
    MOCK_METHOD(void, send, (), (override));
    MOCK_METHOD(void, processQuery, (), (override));
    MOCK_METHOD(void, goSleepFor, (int seconds), (override));
    MOCK_METHOD(void, showStatistics, (), (override));
}
