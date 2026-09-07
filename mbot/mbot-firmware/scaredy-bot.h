#ifndef SCAREDY_BOT_H
#define SCAREDY_BOT_H

#include <MeEncoderOnBoard.h>
#include <MeSoundSensor.h>
#include <MeLightSensor.h>
#include <MeUltrasonicSensor.h>

#include "mode.h"

class ScaredyBot : public Mode {
    public:
        ScaredyBot();

        virtual void loop();
        virtual void setup();

    private:
        MeEncoderOnBoard motor_1;
        MeEncoderOnBoard motor_2;
        MeSoundSensor mic;
        MeLightSensor ldrs[2];
        MeUltrasonicSensor proximity;

        int16_t sound_levels[16];
        int16_t sound_avg;
        int16_t light_levels[16];
        int16_t light_avg;

        unsigned long last_updated_time;
        unsigned long scurry_start_time;
        unsigned long scurry_stop_time;
        unsigned long scurry_updated_time;

        int action;
        bool is_ready;
        bool is_scared;

        int motor_1_speed;
        int motor_2_speed;

        // function definitions
        int16_t get_mean_value(int16_t *values, int len);
        void shift_array_left(int16_t *arr, int len, int places);
        void update_sensor_values(void);
        void stop_motors(void);
        void run_forward(void);
        void turn_left(void);
        void turn_right(void);
};

#endif // SCAREDY_BOT_H
