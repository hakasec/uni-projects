/* 
 * Auto-generated from mBlock
 */

#include <MeEncoderOnBoard.h>

#include "police-bot.h"


MeEncoderOnBoard *PoliceBot::pEncoder_1 = NULL;
MeEncoderOnBoard *PoliceBot::pEncoder_2 = NULL;


PoliceBot::PoliceBot() : 
    Encoder_1(SLOT_1),
    Encoder_2(SLOT_2),
    rgbled_0(0, 12),
    ultrasonic_10(PORT_10)
{
    pEncoder_1 = &this->Encoder_1;
    pEncoder_2 = &this->Encoder_2;
}

void PoliceBot::isr_process_encoder1() {
    if (digitalRead(pEncoder_1->getPortB()) == 0) {
        pEncoder_1->pulsePosMinus();
    } else {
        pEncoder_1->pulsePosPlus();
    }
}

static void PoliceBot::isr_process_encoder2() {
    if (digitalRead(pEncoder_2->getPortB()) == 0) {
        pEncoder_2->pulsePosMinus();
    } else {
        pEncoder_2->pulsePosPlus();
    }
}

void PoliceBot::move(int direction, int speed) {
    int leftSpeed = 0;
    int rightSpeed = 0;
    if (direction == 1) {
        leftSpeed = -speed;
        rightSpeed = speed;
    } else if (direction == 2) {
        leftSpeed = speed;
        rightSpeed = -speed;
    } else if (direction == 3) {
        leftSpeed = -speed;
        rightSpeed = -speed;
    } else if (direction == 4) {
        leftSpeed = speed;
        rightSpeed = speed;
    }
    this->Encoder_1.setTarPWM(leftSpeed);
    this->Encoder_2.setTarPWM(rightSpeed);
}

void PoliceBot::_loop() {
    this->Encoder_1.loop();
    this->Encoder_2.loop();
}

void PoliceBot::_delay(float seconds) {
    long endTime = millis() + seconds * 1000;
    while (millis() < endTime)
        _loop();
}

void PoliceBot::setup() {
    TCCR1A = _BV(WGM10);
    TCCR1B = _BV(CS11) | _BV(WGM12);
    TCCR2A = _BV(WGM21) | _BV(WGM20);
    TCCR2B = _BV(CS21);
    attachInterrupt(pEncoder_1->getIntNum(), isr_process_encoder1, RISING);
    attachInterrupt(pEncoder_2->getIntNum(), isr_process_encoder2, RISING);
    this->rgbled_0.setpin(44);
    this->buzzer.setpin(45);
    _delay(2);
}

void PoliceBot::loop() {
    move(1, 75 / 100.0 * 255);
    this->rgbled_0.setColor(0, 255, 0, 0);
    this->rgbled_0.show();
    _delay(0.2);
    this->rgbled_0.setColor(0, 0, 0, 0);
    this->rgbled_0.show();
    this->buzzer.tone(1047, 0.08 > 0 ? 0.08 * 1000 : 0);
    delay(20);
    this->rgbled_0.setColor(0, 30, 0, 255);
    this->rgbled_0.show();
    _delay(0.2);
    this->rgbled_0.setColor(0, 0, 0, 0);
    this->rgbled_0.show();
    this->buzzer.tone(2093, 0.08 > 0 ? 0.08 * 1000 : 0);
    delay(20);
    if ((this->ultrasonic_10.distanceCm() < 20)) {
        move(2, 50 / 100.0 * 255);
        _delay(2);
        move(2, 0);
        move(4, 50 / 100.0 * 255);
        _delay(5.75);
        move(4, 0);
    }

    _loop();
}
