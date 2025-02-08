#ifndef GPIOBUTTONINTERCEPTOR_H
#define GPIOBUTTONINTERCEPTOR_H

// Standard includes
#include <stdexcept>
using std::runtime_error;
using std::invalid_argument;

// External includes
#include "ButtonInterceptorBase.hpp"

class GPIOButtonInterceptor : public ButtonInterceptorBase {
    public:
        GPIOButtonInterceptor(int gpioPinNumber, bool reverseSignal = false, bool pullUpResistance = false);
        
        int getGpioPinNumber();
    protected:
        bool _isPressed() override;
    private:
        int _gpioPinNumber;
        bool _pullUpResistance;

        static const int VALID_GPIO_PIN_NUMBERS[16];
};

#endif
