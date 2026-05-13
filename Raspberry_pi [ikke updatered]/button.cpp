#include "button.h"
#include <gpiod.h>
#include <iostream>
#include <thread>
#include <chrono>

static const char* chip_path = "/dev/gpiochip0";

static unsigned int lines[2] = {17, 27};

static gpiod_chip* chip = nullptr;
static gpiod_line_request* req = nullptr;

// store latest states here
static bool pressed_state[2] = {false, false};

// open GPIO
bool button_init() {
    chip = gpiod_chip_open(chip_path);
    if (!chip) {
        std::cerr << "Failed to open GPIO\n";
        return false;
    }

    gpiod_line_settings* settings = gpiod_line_settings_new();                  // Objekt called settings
    if (!settings) return false;

    gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_INPUT);    // Objekt as input
    gpiod_line_settings_set_bias(settings, GPIOD_LINE_BIAS_PULL_UP);            // objekt as pull up

    gpiod_line_config* config = gpiod_line_config_new();                        // nyt objekt
    if (!config) return false;

    gpiod_line_config_add_line_settings(config, lines, 2, settings);            //(object, array of offset, array size, object)

    req = gpiod_chip_request_lines(chip, nullptr, config);                      // nullptr means default settings.
    
    if (!req) {
        std::cerr << "Failed to request pins\n";
        return false;
    }

    return true;
}

void button_run() {
    while (true) {
        gpiod_line_value values[2];                                     // array values holds the state 0 inactive (low)[trykked] 1 active (high) [ikke trykked]
        gpiod_line_request_get_values(req, values);

        for (int i = 0; i < 2; i++) {
            bool pressed = (values[i] == GPIOD_LINE_VALUE_INACTIVE);    //retunere en bool 1 værdi vis knappen er trykket //inactive
            // update state array
            pressed_state[i] = pressed;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }
}

bool button_is_pressed(int gpio) {
    
    switch (gpio) {
        case 17: return pressed_state[0];           // retunere state af knap 17
        case 27: return pressed_state[1];           // retunere state af knap 27
    }
    return false;
}



void button_cleanup() {
    if (req) {
        gpiod_line_request_release(req);
        req = nullptr;
    }

    if (chip) {
        gpiod_chip_close(chip);
        chip = nullptr;
    }
}
