#include <vector>
namespace st = std;

#include <cantino.h>

namespace cn = cantino;

struct IO {
    int pinIndex = 0;
    bool state = false;

    IO (int pinIndex);
    operator bool ();
};

struct Input: IO {
    Input (int pinIndex);
    read ();
    operator bool ();
};

struct Output: IO {
    Output (int pinIndex);
    write ();
    operator bool ();
    Output &operator = (bool state);
};

struct Plc {
    Input leftButtonIn = 0;
    Input rightButtonIn = 2;
    Input topButtonIn = 3;
    Input bottomButtonIn = 1;
    Output ledOut = 4;

    bool raMotor = false;
    bool const toggleDuration = 500;
    long unsigned int lightToggleTime = 0;
    long unsigned int followToggleTime = 0;
    
    vector <Input> inputs;
    vector <Output> outputs;
    
    Plc ();    
    void readInputs ();
    void sweep (long unsigned int time);
    void writeOutputs ();
};

// ======

IO::IO (int pinIndex):
    pinIndex (pinIndex)
{}

IO::operator bool () {
    return state;
}

// ======

Input::Input (int pinIndex):
    IO (pinIndex)
{
    pinMode (pinIndex, INPUT_PULLUP);
    
}

Input::read () {
    state = !digitalRead (pinIndex);  // Pullup, so inverted
}

// ======

Output::Output (int pinIndex):
    IO (pinIndex)
{
    pinMode (pinIndex, OUTPUT);
}

Output::write () {
    while (digitalRead (pinIndex) != state) {
        digitalWrite (pinIndex, state);
    }
}

Output &Output::operator = (bool state) {
    this->state = state;
    return *this;
}

Plc::Plc () {
    while (true) {
        readInputs ();
        sweep (millis ());
        writeOutputs ();
    }
}

void Plc::readInputs () {
    for (auto input in inputs) {
        input->read ();
    }
}

void Plc::sweep (long unsigned int time) {
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

void Plc::writeOutputs {
    for (output in outputs) {
        output->write ();
    }
}

int main () {
    

    }

    return 0;
}
