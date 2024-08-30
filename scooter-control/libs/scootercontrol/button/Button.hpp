#ifndef BUTTON_H
#define BUTTON_H

// Standard includes
#include <vector>
using std::vector;
#include <type_traits>
using std::is_base_of;
#include <algorithm>
using std::find;
#include <stdexcept>
using std::invalid_argument;
#include <memory>
using std::unique_ptr;
using std::move;

// External includes
#include "interceptors/ButtonInterceptorBase.hpp"
#include "handlers/ButtonHandlerBase.hpp"
#include "ButtonOptionType.h"

// Forward declarations
class ButtonHandlerBase;

template<class T>
class Button {
    static_assert(is_base_of<ButtonHandlerBase, T>::value, "T must inherit from ButtonHandlerBase");

    public:
        explicit Button(ButtonInterceptorBase* interceptor, T* handler, vector<ButtonOptionType> options = {})
            : _interceptorPtr(interceptor),
              _handlerPtr(handler)
        {
            bool forceInitPressed = find(options.begin(), options.end(), ButtonOptionType::ForceInitPressed) != options.end();
            bool forceInitReleased = find(options.begin(), options.end(), ButtonOptionType::ForceInitReleased) != options.end();

            if (forceInitPressed && forceInitReleased) {
                throw invalid_argument("Button::Button: Cannot force init button to both pressed and released");
            }

            if (forceInitPressed) {
                _value = true;
            } else if (forceInitReleased) {
                _value = false;
            } else {
                _value = _isPressed();
            }
        }

        void update() {
            bool newValue = _isPressed();

            if (newValue != _value) {
                _value = newValue;

                if (_value) {
                    _handlerPtr->onValueRaised();
                } else {
                    _handlerPtr->onValueLowered();
                }
            }

            _handlerPtr->onUpdate();
        }

        void enableHandler() {
            _handlerPtr->enable();
        }
        void disableHandler() {
            _handlerPtr->disable();
        }

        T* getHandlerPtr() {
            return _handlerPtr;
        }
    private:
        bool _isPressed() {
            return _interceptorPtr->isPressed();
        }

        ButtonInterceptorBase* _interceptorPtr;
        T* _handlerPtr;

        bool _value;
};

#endif
