#ifndef BUTTONHANDLERBASE_H
#define BUTTONHANDLERBASE_H

// External includes
#include "../Button.hpp"

class ButtonHandlerBase {
    template<class T>
    friend class Button;

    public:
        void enable() {
            if (!_enabled) {
                _enabled = true;
                _onEnabled();
            }
        }
        void disable() {
            if (_enabled) {
                _enabled = false;
                _onDisabled();
            }
        }
        void setState(bool enabled) {
            if (enabled) {
                enable();
            } else {
                disable();
            }
        }
        bool isEnabled() {
            return _enabled;
        }
    protected:
        virtual ~ButtonHandlerBase() = default;

        virtual void _onEnabled() {};
        virtual void _onDisabled() {};

        virtual void _onValueRaised() {};
        virtual void _onValueLowered() {};
        
        virtual void _onUpdate() {};
    private:
        bool _enabled = true;
};

#endif
