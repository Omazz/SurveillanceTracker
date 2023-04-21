#include "AirplaneHandler.h"

AirplaneHandler::AirplaneHandler() {
    _messageHandler = new MessageHandler(this);
}

