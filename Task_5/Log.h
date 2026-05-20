#ifndef LOG_H
#define LOG_H

#include <string>
#include <vector>
#include <ctime>
#include <iomanip>
#include <iostream>

static const size_t MAX_EVENTS = 10;

enum LogLevel {
    LOG_NORMAL,
    LOG_WARNING,
    LOG_ERROR
};

struct Event {
    std::time_t time;
    LogLevel level;
    std::string message;
};

class Log {
private:    
    std::vector<Event> events;    

public:
    static Log* Instance();
    Log() = default;
    void message(const LogLevel level, const std::string& msg);
    void print() const;
    Log(const Log&) = delete;
    Log& operator=(const Log&) = delete;
};

#endif