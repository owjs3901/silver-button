#ifndef __DATA_H
#define __DATA_H

struct s_data {
    //room_led, hall_led, window, door
    uint8_t state[4];
    float temp, hum;
};

#endif