#include "button.h"
#include <iostream>
#include <thread>
#include <chrono>

const char* Button::chip_path = "/dev/gpiochip0";

Button::Button() //konstruktur
{
}

Button::~Button() //destruktur
{
    cleanup();
}

// Open GPIO
bool Button::init()
{
    chip = gpiod_chip_open(chip_path);

    if (!chip)
    {
        std::cerr << "Failed to open GPIO\n";
        return false;
    }

    gpiod_line_settings* settings = gpiod_line_settings_new();

    if (!settings)
        return false;

    gpiod_line_settings_set_direction(
        settings,
        GPIOD_LINE_DIRECTION_INPUT
    );

    gpiod_line_settings_set_bias(
        settings,
        GPIOD_LINE_BIAS_PULL_UP
    );

    gpiod_line_config* config = gpiod_line_config_new();

    if (!config)
        return false;

    gpiod_line_config_add_line_settings(
        config,
        lines,
        2,
        settings
    );

    req = gpiod_chip_request_lines(
        chip,
        "varmelampe",
        config
    );

    /* free temporary line settings/config (request copies needed data) */
    gpiod_line_settings_free(settings);
    gpiod_line_config_free(config);

    if (!req)
    {
        std::cerr << "Failed to request pins\n";
        return false;
    }

    return true;
}

void Button::run()
{
    while (true)
    {
        gpiod_line_value values[2];

        int ret = gpiod_line_request_get_values(
            req,
            values
        );

        if (ret < 0)
        {
            std::cerr << "GPIO read failed\n";
            continue;
        }

        for (int i = 0; i < 2; i++)
        {
            bool pressed =
                (values[i] == GPIOD_LINE_VALUE_INACTIVE);

            pressed_state[i] = pressed;
        }

        std::this_thread::sleep_for(
            std::chrono::milliseconds(30)
        );
    }
}

void Button::stop()
{
    run_flag = false;
}

bool Button::is_pressed(int gpio)
{
    switch (gpio)
    {
        case BUTTON_NEXT_GPIO:
            return pressed_state[0];

        case BUTTON_CLICK_GPIO:
            return pressed_state[1];
    }

    return false;
}

void Button::cleanup()
{
    std::cout << "Button cleaned\n";

    if (req)
    {
        gpiod_line_request_release(req);
        req = nullptr;
    }

    if (chip)
    {
        gpiod_chip_close(chip);
        chip = nullptr;
    }
}