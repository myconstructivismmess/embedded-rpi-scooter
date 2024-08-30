#ifndef BUTTONHANDLERBASE_H
#define BUTTONHANDLERBASE_H

// External includes
#include "../Button.hpp"

class ButtonHandlerBase {
    template<class T>
    friend class Button;

    public:
        void enable();
        void disable();
        void setState(bool enabled);
        bool isEnabled();
    protected:
        virtual ~ButtonHandlerBase() = default;

        virtual void _onEnabled() {};
        virtual void _onDisabled() {};

        virtual void _onValueRaised() {};
        virtual void _onValueLowered() {};
        
        virtual void _onUpdate() {};
    private:
        void onValueRaised();
        void onValueLowered();

        void onUpdate();

        bool _enabled = true;
};

#endif
