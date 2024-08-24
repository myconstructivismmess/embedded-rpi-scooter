#ifndef LATCHBUTTONHANDLER_H
#define LATCHBUTTONHANDLER_H

// External includes
#include "ButtonHandlerBase.hpp"

class LatchButtonHandler : public ButtonHandlerBase {
    public:
        LatchButtonHandler(void(*onValueChangeCallback)(bool), bool startValue = false, bool switchOnLowered = false);

        bool getValue();
        void setValue(bool value, bool triggerCallback = true);
    protected:
        void _onValueRaised() override;
        void _onValueLowered() override;
    private:
        void(*_onValueChangeCallback)(bool);
        const bool _switchOnLowered;

        bool _value;
};

#endif
