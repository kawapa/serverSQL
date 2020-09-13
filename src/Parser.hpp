#pragma once

#include <jsoncpp/json/json.h>
#include <string>
#include <utility>

constexpr size_t MAX_ = 512;

class Parser {
public:
    char* getmessage();
    // static void generateAnswer(char* message, bool status);
    // static void generateAnswer(char* message, std::pair<std::string, std::string>);
    // static void generateAnswer(std::string message1, int64_t seconds, std::string message2);

private:
    std::string message_[MAX_];
};


/*

std::string Server::generateAnswer(const Answers& answer) {
    Json::Value val;
    std::for_each(begin(answer), end(answer), [&val](const auto& pair) {
        val[pair.first] = pair.second;
    });

    Json::FastWriter fast;
    return fast.write(val);
}
*/