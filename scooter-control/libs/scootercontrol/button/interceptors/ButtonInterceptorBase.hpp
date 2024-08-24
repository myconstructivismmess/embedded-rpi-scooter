#ifndef BUTTONINTERCEPTORBASE_H
#define BUTTONINTERCEPTORBASE_H

class ButtonInterceptorBase {
    public:
        ButtonInterceptorBase(bool reverseSignal = false);
        virtual ~ButtonInterceptorBase() = default;

        bool isPressed();
    protected:
        virtual bool _isPressed() = 0;
    private:
        bool _reverseSignal;
};

#endif