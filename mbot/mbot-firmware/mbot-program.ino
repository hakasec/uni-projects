#include <SoftwareSerial.h>
#include <MeAuriga.h>

#include "mode.h"

#include "scaredy-bot.h"
#include "police-bot.h"
#include "beep.h"

ScaredyBot scaredy_bot;
PoliceBot police_bot;
Beep beep;

int mode = 0;
bool is_setup = false;


void setup() {
    Serial.begin(115200);
}


void loop() {
    if (Serial.available()) {
        mode = Serial.read();
        is_setup = false;
    }
  
    Mode *m;
    switch (mode) {
        default:
        case 0:
            m = &scaredy_bot;
            break;
        case 1:
            m = &police_bot;
            break;
        case 2:
            m = &beep;
            break;
    }

    if (!is_setup) {
        m->setup();
        is_setup = true;
    }
    m->loop();
}
