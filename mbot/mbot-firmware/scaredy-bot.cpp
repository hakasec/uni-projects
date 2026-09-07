#include <MeEncoderOnBoard.h>
#include <MeSoundSensor.h>
#include <MeLightSensor.h>
#include <MeUltrasonicSensor.h>

#include "scaredy-bot.h"

#define LDR_1_PORT PORT_11
#define LDR_2_PORT PORT_12
#define MIC_PORT PORT_14

#define SENSOR_UPDATE_INTERVAL 200
#define SCURRY_UPDATE_INTERVAL 250
#define SCURRY_TIME 20000

#define MIN_DISTANCE 30
#define MAX_SPEED 1000

ScaredyBot::ScaredyBot() : 
    motor_1(SLOT_1),
    motor_2(SLOT_2),
    mic(MIC_PORT),
    proximity(PORT_10)
{ 
    this->ldrs[2] = {0};

    this->sound_levels[16] = {0};
    this->light_levels[16] = {0};

    this->last_updated_time = 0;
    this->scurry_start_time = 0;
    this->scurry_stop_time = 0;
    this->scurry_updated_time = 0;

    this->action = 2;
    this->is_ready = false;
    this->is_scared = false;

    this->motor_1_speed = 0;
    this->motor_2_speed = 0;
}

/*
 * Returns mean value of a given array
 * Params:
 *  values  - The array
 *  len     - Length of the array
 * Returns:
 *  mean values of the array
 */
int16_t ScaredyBot::get_mean_value(int16_t *values, int len)
{
    int sum = 0;
    for (int i = 0; i < len; i++)
    {
        sum += values[i];
    }
    return (int16_t)(sum / len);
}

/*
 * Shifts all elements in an array to the left
 * Params:
 *  arr     - The array to shift
 *  len     - Length of the array
 *  places  - Amount of times to shift
 */
void ScaredyBot::shift_array_left(int16_t *arr, int len, int places)
{
    for (int i = places; i < len; i++)
    {
        if (i > (len - places))
        {
            arr[i] = 0;
        }
        else
        {
            arr[i - places] = arr[i];
        }
    }
}

/*
 * Updates the value buffers and average value
 */
void ScaredyBot::update_sensor_values()
{
    this->shift_array_left(this->light_levels, 16, 1);
    this->shift_array_left(this->sound_levels, 16, 1);

    int16_t ldr_values[2] = {this->ldrs[0].read(), this->ldrs[1].read()};
    int16_t ldr_mean = this->get_mean_value(ldr_values, 2);

    int16_t sound_value = this->mic.strength();

    this->light_levels[15] = ldr_mean;
    this->sound_levels[15] = sound_value;

    this->light_avg = this->get_mean_value(this->light_levels, 16);
    this->sound_avg = this->get_mean_value(this->sound_levels, 16);
}

void ScaredyBot::stop_motors()
{
    this->motor_1_speed = 0;
    this->motor_2_speed = 0;
}

void ScaredyBot::run_forward()
{
    this->motor_1_speed = -MAX_SPEED;
    this->motor_2_speed = MAX_SPEED;
}

void ScaredyBot::turn_left()
{
    this->motor_1_speed = -MAX_SPEED;
    this->motor_2_speed = -MAX_SPEED;
}

void ScaredyBot::turn_right()
{
    this->motor_1_speed = MAX_SPEED;
    this->motor_2_speed = MAX_SPEED;
}

void ScaredyBot::setup()
{
    // manually set pin
    this->proximity.setpin(65);
  
    // Initialise motors
    this->motor_1.setPosPid(1.8, 0, 1.2);
    this->motor_2.setPosPid(1.8, 0, 1.2);
    this->motor_1.setSpeedPid(0.18, 0, 0);
    this->motor_2.setSpeedPid(0.18, 0, 0);

    // Initialise light sensors
    this->ldrs[0] = MeLightSensor(LDR_1_PORT);
    this->ldrs[1] = MeLightSensor(LDR_2_PORT);

    this->update_sensor_values();
}

void ScaredyBot::loop()
{
    unsigned long cur_time = millis();
    double distance = this->proximity.distanceCm();
    int16_t brightness_1 = this->ldrs[0].read();
    int16_t brightness_2 = this->ldrs[1].read();

    // Set threshold before bot becomes scared
    float mic_peak = ((float)this->sound_avg) * 2.2f;

    // Set threshold for when a light level can be classed as "dark"
    float darkness_peak = ((float)this->light_avg) * 0.5f;
    bool is_dark = brightness_1 < (int)darkness_peak && brightness_2 < (int)darkness_peak;

    // Collect sensor readouts at SENSOR_UPDATE_INTERVAL
    if ((cur_time - last_updated_time) > SENSOR_UPDATE_INTERVAL)
    {
        update_sensor_values();
        last_updated_time = cur_time;
        if ((cur_time / SENSOR_UPDATE_INTERVAL) >= 10)
        {
            is_ready = true;
        }
    }

    if (this->mic.strength() > ((int)mic_peak) && this->is_ready && !this->is_scared)
    {
        this->is_scared = true;
        this->scurry_start_time = cur_time;
    }

    if (this->is_scared)
    {
        if ((cur_time - this->scurry_updated_time) > SCURRY_UPDATE_INTERVAL)
        {
            switch (action)
            {
                case 0:
                    this->turn_left();
                    break;
                case 1:
                    this->turn_right();
                    break;
                default:
                    this->run_forward();
            }
            this->action = random(0, 5);
            this->scurry_updated_time = cur_time;
        }
        if (distance <= MIN_DISTANCE)
        {
            this->turn_left();
            this->action = 2;
        }
        if ((cur_time - this->scurry_start_time) > SCURRY_TIME || is_dark)
        {
            is_scared = false;
            stop_motors();
            scurry_stop_time = cur_time;
        }
    }

    this->motor_1.runSpeed(motor_1_speed);
    this->motor_2.runSpeed(motor_2_speed);

    this->motor_1.loop();
    this->motor_2.loop();
}
