#ifndef BUTTON_H
#define BUTTON_H

#include <gpiod.h>
#include <atomic>

class Button
{
private:
    static const char* chip_path;

    static constexpr unsigned int BUTTON_NEXT_GPIO = 17;
    static constexpr unsigned int BUTTON_CLICK_GPIO = 27;

    unsigned int lines[2] = {BUTTON_NEXT_GPIO, BUTTON_CLICK_GPIO};

    gpiod_chip* chip = nullptr;
    gpiod_line_request* req = nullptr;

    std::atomic<bool> pressed_state[2] = {false, false};

    std::atomic<bool> run_flag{true};

public:
    Button();
    ~Button();

    bool init();
    void run();
    void stop();
    void cleanup();
    
    bool is_pressed(int gpio);
};

#endif