#ifndef BEEP_H
#define BEEP_H

#include <MeBuzzer.h>

#include "mode.h"

class Beep : public Mode {
    public:
        Beep();

        virtual void loop();
        virtual void setup();

    private:
        MeBuzzer buzzer;
        uint16_t notes[32] = {
            1175,1319,1047,880,988,784,587,659,
            523,440,494,392,294,330,262,220,
            247,220,208,196,392,294,311,330,
            523,330,523,330,523,0,0,0
        };
        int note_idx;
};

#endif // BEEP_H
