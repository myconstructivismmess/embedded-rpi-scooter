#ifndef GPIOBUTTONINTERCEPTOR_H
#define GPIOBUTTONINTERCEPTOR_H

// Standard includes
#include <stdexcept>
using std::runtime_error;
using std::invalid_argument;

// External libraries includes
#include <wiringPi.h>

// External includes
#include "ButtonInterceptorBase.hpp"

class GPIOButtonInterceptor : public ButtonInterceptorBase {
    public:
        GPIOButtonInterceptor(int gpioPinNumber, bool reverseSignal = false)
            : ButtonInterceptorBase(reverseSignal),
              _gpioPinNumber(gpioPinNumber)
        {
            if (wiringPiSetupGpio() == -1) {
                throw runtime_error("Failed to setup GPIO");
            }

            bool isValid = false;
            for (unsigned int i = 0; i < sizeof(VALID_GPIO_PIN_NUMBERS) / sizeof(int); i++) {
                if (gpioPinNumber == VALID_GPIO_PIN_NUMBERS[i]) {
                    isValid = true;
                    break;
                }
            }

            if (!isValid) {
                throw invalid_argument("Invalid GPIO pin number");
            }

            pinMode(_gpioPinNumber, INPUT);
            pullUpDnControl(_gpioPinNumber, PUD_DOWN);
        }
        
        int getGpioPinNumber() {
            return _gpioPinNumber;
        }
    protected:
        bool _isPressed() override {
            return digitalRead(_gpioPinNumber) == HIGH;
        }
    private:
        const int _gpioPinNumber;

        static const int VALID_GPIO_PIN_NUMBERS[16];
};

const int GPIOButtonInterceptor::VALID_GPIO_PIN_NUMBERS[] = { 4, 5, 6, 12, 13, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26 };

#endif
