
#include <cantino.h>

namespace cn = cantino;

struct IO {
    IO (int pinIndex);
    int pinIndex;
    bool state;
};

struct Input: IO {
    Input (int pinIndex);
    operator bool ();
};

struct Output: IO {
    Output (int pinIndex);
    operator bool ();
    Output &operator = (bool state);
};

IO::IO (int pinIndex):
    pinIndex (pinIndex)
{}

Input::Input (int pinIndex):
    IO (pinIndex)
{
    pinMode (pinIndex, INPUT_PULLUP);
}

Input::operator bool () {
    return state = !digitalRead (pinIndex);  // Pullup, so inverted
}

Output::Output (int pinIndex):
    IO (pinIndex)
{
    pinMode (pinIndex, OUTPUT);
}

Output::operator bool () {
    return state;
}

Output &Output::operator = (bool state) {
    this->state = state;
    digitalWrite (pinIndex, this->state);
    return *this;
}

int main () {
    Input leftButton (0);
    Input rightButton (2);
    Input topButton (3);
    Input bottomButton (1);
    
    Output led (4);
    auto raMotor = false;

    auto const toggleDuration = 500;
    
    auto time = millis ();
    auto lightToggleTime = time;
    auto followToggleTime = time;
    
    while (true) {
        time = millis ();

        if (!leftButton && !rightButton && topButton && bottomButton && time - lightToggleTime > toggleDuration) {
            led = !led;
            lightToggleTime = time;
        }
        else if (leftButton && rightButton && !topButton && !bottomButton && time - followToggleTime > toggleDuration) {
            raMotor = !raMotor;
            followToggleTime = time;
        }
        
        if (leftButton && !rightButton && !topButton && !bottomButton) {
        }
        else if (!leftButton && rightButton && !topButton && !bottomButton) {
        }
        else if (!leftButton && !rightButton && topButton && !bottomButton) {
        }
        else if (!leftButton && !rightButton && !topButton && bottomButton) {
        }
        else {
        }
    }

    return 0;
}
