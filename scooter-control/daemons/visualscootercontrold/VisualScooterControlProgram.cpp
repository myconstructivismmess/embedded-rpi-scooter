#include "VisualScooterControlProgram.hpp"

// Standard includes
#include <memory>
using std::make_unique;
using std::make_shared;

#include <vector>
using std::vector;

#include <iostream>
using std::cout;
using std::endl;

// Scooter control library includes
#include "button/ButtonOptionType.h"
#include "LedLayer.hpp"

VisualScooterControlProgram* VisualScooterControlProgram::_instance = nullptr;

VisualScooterControlProgram::VisualScooterControlProgram(Pins pins, Properties properties)
    : Program(),
      _lightOnOffButtonHandler(_onLightOnOffStateChange),
      _lightOnOffButtonInterceptor(pins.lightOnOffButtonPin),
      _turnLeftSignalButtonHandler(_onTurnLeftStateChange),
      _turnLeftSignalButtonInterceptor(pins.turnLeftSignalButtonPin),
      _turnRightSignalButtonHandler(_onTurnRightStateChange),
      _turnRightSignalButtonInterceptor(pins.turnRightSignalButtonPin)
    // No brake for now
    //   _brakeDetectorHandler(),
    //   _brakeDetectorInterceptor(pins.brakeDetectorPin)
{
    _instance = this;

    // Setup timer
    Timer::setup();

    // Setup light on/off button
    _lightOnOffButton = make_unique<Button<DefaultButtonHandler>>(
        &_lightOnOffButtonInterceptor,
        &_lightOnOffButtonHandler,
        vector<ButtonOptionType>{ButtonOptionType::ForceInitReleased}
    );

    // Setup turn left signal button
    _turnLeftSignalButton = make_unique<Button<DefaultButtonHandler>>(
        &_turnLeftSignalButtonInterceptor,
        &_turnLeftSignalButtonHandler,
        vector<ButtonOptionType>{ButtonOptionType::ForceInitReleased}
    );

    // Setup turn right signal button
    _turnRightSignalButton = make_unique<Button<DefaultButtonHandler>>(
        &_turnRightSignalButtonInterceptor,
        &_turnRightSignalButtonHandler,
        vector<ButtonOptionType>{ButtonOptionType::ForceInitReleased}
    );

    // Setup brake detector (No brake for now)
    // _brakeDetector = make_unique<Button<DefaultButtonHandler>>(
    //     &_brakeDetectorInterceptor,
    //     &_brakeDetectorHandler,
    //     vector<ButtonOptionType>{ButtonOptionType::ForceInitReleased}
    // );

    // Setup Layers
    _frontLightOnOffLedLayer = make_shared<SolidColorLedLayer>(
        0,
        59,
        0,
        Color::WHITE
    );
    _frontLightOnOffLedLayer->enable();
    _frontTurnSignalLedLayer = make_shared<TurnLightLedLayer>(
        0,
        29,
        0,
        TurnLightLedLayer::Properties {
            color: Color(properties.turnSignalColor),
            animationProperties: TurnLightLedLayer::AnimationProperties {
                duration: 2
            }
        }
    );

    // Setup LEDs controller
    LedController::setup(
        properties.ledStripsPtr,
        properties.ledStripsLedCount,
        vector<shared_ptr<LedLayer>>{
            _frontLightOnOffLedLayer,
            _frontTurnSignalLedLayer
        }
    );
}
VisualScooterControlProgram::~VisualScooterControlProgram() {
    _instance = nullptr;
}

void VisualScooterControlProgram::_onLightOnOffStateChange(bool newState) {
    cout << "VisualScooterControlProgram::_onLightOnOffStateChange: New State is " << (newState ? "On" : "Off") << "." << endl;
}
void VisualScooterControlProgram::_onTurnLeftStateChange(bool newState) {
    cout << "VisualScooterControlProgram::_onTurnLeftStateChange: New State is " << (newState ? "Turning Left" : "Not Turning Left") << "." << endl;

    if (_instance == nullptr) {
        return;
    }

    _instance->_frontTurnSignalLedLayer->setState(newState);
}
void VisualScooterControlProgram::_onTurnRightStateChange(bool newState) {
    cout << "VisualScooterControlProgram::_onTurnRightStateChange: New State is " << (newState ? "Turning Right" : "Not Turning Right") << "." << endl;
}

void VisualScooterControlProgram::_update() {
    // Update buttons
    _lightOnOffButton->update();
    _turnLeftSignalButton->update();
    _turnRightSignalButton->update();
    // No brake for now
    //_brakeDetector->update();

    // Update LEDs controller
    LedController::update();
}
