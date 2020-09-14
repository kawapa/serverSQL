#pragma once

#include <jsoncpp/json/json.h>
#include <memory>
#include <string>
#include <vector>

class Parser {
public:
    static void parseToServer(char* messageString);

    static void parseToClient(std::unique_ptr<std::string>& output, bool status);   
    static void parseToClient(std::unique_ptr<std::string>& output, uint64_t seconds, int queriesReceived);
    static void parseToClient(std::unique_ptr<std::string>& output,
                              const std::string& cmd,
                              const std::string& argument);

private:
    static std::string OK_;
    static std::string notOK_;
};
