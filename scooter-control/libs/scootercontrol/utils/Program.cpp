// Class Definition
#include "Program.hpp"

Program::Program() {
    if (_instance != nullptr) {
        throw runtime_error("Program: Only one instance of the Program class can be created.");
    }

    _instance = this;
}
Program::~Program() {
    _instance = nullptr;
}

void Program::update() {
    Timer::update();
    _update();
}

void Program::runUntilStopped() {
    _shouldExit = false;
    
    while (!_shouldExit) {
        update();
    }
}

void Program::exit() {
    _shouldExit = true;
}

void Program::resetExit() {
    _shouldExit = false;
}
        
void Program::registerSignalHandlers() {
    signal(SIGINT, _onSignal);
    signal(SIGTERM, _onSignal);
    signal(SIGKILL, _onSignal);
}
void Program::unregisterSignalHandlers() {
    signal(SIGINT, SIG_DFL);
    signal(SIGTERM, SIG_DFL);
    signal(SIGKILL, SIG_DFL);
}

void Program::_onSignal(int sigNo) {
    if (_instance == nullptr) {
        cerr << endl << "Received signal " << sigNo << " but no Program instance exists." << endl;
        return;
    }
    
    cout << endl << "Received signal " << sigNo << endl << "Exiting..." << endl;
    _instance->exit();
}

Program* Program::_instance = nullptr;
