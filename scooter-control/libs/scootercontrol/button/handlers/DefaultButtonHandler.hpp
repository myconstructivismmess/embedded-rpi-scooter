#ifndef DEFAULTBUTTONHANDLER_H
#define DEFAULTBUTTONHANDLER_H

// External includes
#include "ButtonHandlerBase.hpp"

class DefaultButtonHandler : public ButtonHandlerBase {
    public:
        DefaultButtonHandler(void(*onValueChangeCallback)(bool));
    protected:
        void _onEnabled() override;
        void _onDisabled() override;

        void _onValueRaised() override;
        void _onValueLowered() override;
    private:
        void(*_onValueChangeCallback)(bool);
        
        bool _realValue;
        bool _value;
};

#endif
