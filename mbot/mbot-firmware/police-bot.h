/* 
 * Auto-generated from mBlock
 */


#ifndef POLICE_BOT_H
#define POLICE_BOT_H

#include <MeEncoderOnBoard.h>
#include <MeRGBLed.h>
#include <MeBuzzer.h>
#include <MeUltrasonicSensor.h>

#include "mode.h";

class PoliceBot : public Mode {
    public:
        PoliceBot();

        virtual void loop();
        virtual void setup();

        // Static pointer so we can use attachInterrupt
        static MeEncoderOnBoard *pEncoder_1;
        static MeEncoderOnBoard *pEncoder_2;

    private:
        MeEncoderOnBoard Encoder_1;
        MeEncoderOnBoard Encoder_2;
        
        MeRGBLed rgbled_0;
        MeBuzzer buzzer;
        MeUltrasonicSensor ultrasonic_10;

        static void isr_process_encoder1();
        static void isr_process_encoder2();
        
        void move(int direction, int speed);

        void _loop();
        void _delay(float seconds);
};

#endif // POLICE_BOT_H
