#include "Log.h"

Log* Log::Instance() {
    static Log instance;
    return &instance;
}

void Log::message(const LogLevel level, const std::string& msg) {
    Event event;
    event.time = std::time(nullptr);
    event.level = level;
    event.message = msg;

    events.push_back(event);
    if (events.size() > MAX_EVENTS) {
        events.erase(events.begin());
    }
}

void Log::print() const {
    std::cout << "Last 10 events:" << std::endl;
    for (const auto& event: events) {
        std::cout << "Time: " << event.time << std::endl;
        std::cout << "Level of event: ";
        switch (event.level) {
        case LOG_NORMAL:
            std::cout << "NORMAL";
            break;
        case LOG_WARNING:
            std::cout << "WARNING";
            break;        
        default:            
            std::cout << "ERROR";            
            break;
        }
        std::cout << std::endl;
        std::cout << "Message: " << event.message << std::endl;
    }
}