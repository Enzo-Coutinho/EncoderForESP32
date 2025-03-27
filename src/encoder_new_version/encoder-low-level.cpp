#include "encoder-low-level.h"


EncoderLL::EncoderLL(gpio_num_t pinA, gpio_num_t pinB) : aPinNumber(pinA), bPinNumber(pinB)
{
    gpio_pad_select_gpio(aPinNumber);
    gpio_pad_select_gpio(bPinNumber);

    ESP_ERROR_CHECK_WITHOUT_ABORT(gpio_set_direction(aPinNumber, GPIO_MODE_INPUT));
    ESP_ERROR_CHECK_WITHOUT_ABORT(gpio_set_direction(bPinNumber, GPIO_MODE_INPUT));

    ESP_ERROR_CHECK_WITHOUT_ABORT(gpio_pulldown_en(aPinNumber));
    ESP_ERROR_CHECK_WITHOUT_ABORT(gpio_pulldown_en(bPinNumber));

    initialize();
}

void EncoderLL::initialize()
{
    enc_unit_config.high_limit = UPPER_LIMIT;
    enc_unit_config.low_limit = LOWER_LIMIT;

    ESP_ERROR_CHECK_WITHOUT_ABORT(pcnt_new_unit(&enc_unit_config, &enc_unit_handler));

    enc_channel_A_config.edge_gpio_num = aPinNumber;
    enc_channel_A_config.level_gpio_num = bPinNumber;

    ESP_ERROR_CHECK_WITHOUT_ABORT(pcnt_new_channel(enc_unit_handler, &enc_channel_A_config, &enc_channel_A_handler));

    enc_channel_B_config.edge_gpio_num = bPinNumber;
    enc_channel_B_config.level_gpio_num = aPinNumber;

    ESP_ERROR_CHECK_WITHOUT_ABORT(pcnt_new_channel(enc_unit_handler, &enc_channel_B_config, &enc_channel_B_handler));

    setFilterInNanoseconds(DEFAULT_NS_FILTER);

    pcnt_unit_enable(enc_unit_handler);

    pcnt_unit_clear_count(enc_unit_handler);

    pcnt_unit_start(enc_unit_handler);
}

void EncoderLL::setFilterInNanoseconds(uint32_t nanoseconds)
{
    pcnt_glitch_filter_config_t filter_config = {.max_glitch_ns = nanoseconds,};
    if(nanoseconds != 0)
        ESP_ERROR_CHECK_WITHOUT_ABORT(pcnt_unit_set_glitch_filter(enc_unit_handler, &filter_config));
    else
        ESP_ERROR_CHECK_WITHOUT_ABORT(pcnt_unit_set_glitch_filter(enc_unit_handler, NULL));
}

int EncoderLL::getCount()
{
    ESP_ERROR_CHECK_WITHOUT_ABORT(pcnt_unit_get_count(enc_unit_handler, &counts));
    return counts;
}

void EncoderLL::resetCount()
{
    ESP_ERROR_CHECK_WITHOUT_ABORT(pcnt_unit_clear_count(enc_unit_handler));
}