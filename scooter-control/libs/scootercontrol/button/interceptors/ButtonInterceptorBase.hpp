#ifndef BUTTONINTERCEPTORBASE_H
#define BUTTONINTERCEPTORBASE_H

class ButtonInterceptorBase {
    public:
        ButtonInterceptorBase(bool reverseSignal = false)
            : _reverseSignal(reverseSignal) {}
        virtual ~ButtonInterceptorBase() = default;

        bool isPressed() {
            return _reverseSignal
                ? !_isPressed()
                : _isPressed();
        }
    protected:
        virtual bool _isPressed() = 0;
    private:
        const bool _reverseSignal;
};

#endif