#include "Log.h"

int main(int argc, char** argv) {
    Log* Logger = Log::Instance();

    Logger->message(LOG_NORMAL, "hehehe");
    Logger->message(LOG_NORMAL, "hohoho");
    Logger->message(LOG_WARNING, "Don't do this!");
    Logger->message(LOG_ERROR, "Oh no");

    Logger->print();   
    
    return 0;
}