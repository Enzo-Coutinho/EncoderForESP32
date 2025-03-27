#ifndef encoder_low_level_h
#define encoder_low_level_h

#include "driver/gpio.h"
#include "driver/pulse_cnt.h"
#include "hal/pcnt_types.h"
#include <freertos/FreeRTOS.h>
#include <rom/gpio.h>

#define UPPER_LIMIT 327666;
#define LOWER_LIMIT -32766;

class EncoderLL
{
    public:
        EncoderLL(gpio_num_t pinA, gpio_num_t pinB);
        void initialize();
        void setFilterInNanoseconds(uint32_t nanoseconds);
        int getCount();
        void resetCount();
        const gpio_num_t aPinNumber;
        const gpio_num_t bPinNumber;
    private:
        pcnt_unit_config_t enc_unit_config;
        pcnt_unit_handle_t enc_unit_handler;
        pcnt_chan_config_t enc_channel_A_config;
        pcnt_channel_handle_t enc_channel_A_handler = NULL;
        pcnt_chan_config_t enc_channel_B_config;
        pcnt_channel_handle_t enc_channel_B_handler = NULL;
        pcnt_glitch_filter_config_t enc_filter_config;

        static const uint32_t DEFAULT_NS_FILTER = 250;

        int counts = 0;
};

#endif