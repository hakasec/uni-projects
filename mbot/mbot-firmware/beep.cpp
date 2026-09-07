
#include "beep.h"

Beep::Beep() :
    buzzer(45)
{
    
}

void Beep::setup() {
    this->buzzer.setpin(45);
    this->note_idx = 0;
}

void Beep::loop() {
    if (this->note_idx >= 32)
        this->note_idx = 0;

    uint16_t note = this->notes[this->note_idx];
    if (note > 0) {
      this->buzzer.tone(this->notes[this->note_idx], 250);
    }
    delay(100);
    this->note_idx++;
}

