#include "lcd.h"

#include <chrono>
#include <thread>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

// ----------------------
// Constructor / Destructor
// ----------------------

LCD::LCD() : fd(-1)
{
}

LCD::~LCD()
{
    if (fd >= 0)
    {
        close(fd);
    }
}

void LCD::lcd_write(uint8_t data)
{
    data |= LCD_Backlight;

    write(fd, &data, 1);

    std::this_thread::sleep_for(
        std::chrono::microseconds(1000)
    );
}

void LCD::pulse(uint8_t data)
{
    lcd_write(data | LCD_Enable);
    lcd_write(data & ~LCD_Enable);
}

void LCD::send_byte(uint8_t val, uint8_t mode)
{
    uint8_t high = (val & Bit_mask) | mode;
    uint8_t low  = ((val << 4) & Bit_mask) | mode;

    pulse(high);
    pulse(low);
}

bool LCD::init()
{
    fd = open("/dev/i2c-1", O_RDWR);

    if (fd < 0)
        return false;

    ioctl(fd, I2C_SLAVE, LCD_ADDR);

    std::this_thread::sleep_for(
        std::chrono::microseconds(50000)
    );

    pulse(0x30);
    pulse(0x30);
    pulse(0x30);
    pulse(0x20);

    send_byte(0x28, COMMAND);
    send_byte(0x08, COMMAND);
    send_byte(LCD_Clear, COMMAND);

    std::this_thread::sleep_for(
        std::chrono::microseconds(2000)
    );

    send_byte(0x06, COMMAND);
    send_byte(0x0C, COMMAND);

    return true;
}

// ----------------------
// Print
// ----------------------

void LCD::print(const std::string& text, uint8_t start)
{
    send_byte(start, COMMAND);

    for (char c : text)
    {
        send_byte(c, DATA);
    }
}

// ----------------------
// Menu
// ----------------------

void LCD::Menu_chair(int temp)
{
    send_byte(LCD_Clear, COMMAND);

    std::string t = std::to_string(temp);

    print(t, Linje1 + 15);
    send_byte(LCD_Degree, DATA);

    print(" [1] [2]", Linje2);
    print(" [3] [4]", Linje4);
}

int LCD::Next_menu_chair(int chair)
{
    chair++;

    switch (chair)
    {
        case 1:
            print("x", Linje2 + 4);
            return chair;

        case 2:
            print("1", Linje2 + 4);
            print("x", Linje2 + 12);
            return chair;

        case 3:
            print("2", Linje2 + 12);
            print("x", Linje4 + 4);
            return chair;

        case 4:
            print("3", Linje4 + 4);
            print("x", Linje4 + 12);
            return chair;

        case 5:
            print("4", Linje4 + 12);
            return 0;
    }

    return chair;
}

void LCD::Menu_State()
{
    send_byte(LCD_Clear, COMMAND);

    print("OFF",  Linje1);
    print("Low",  Linje2);
    print("Mid",  Linje3);
    print("High", Linje4);
}

int LCD::Next_menu_state(int state)
{
    state = (state + 1) % 5;

    Menu_State();

    switch (state)
    {
        case OFF:
            print("> OFF", Linje1);
            return state;

        case LOW:
            print("> Low", Linje2);
            return state;

        case MID:
            print("> Mid", Linje3);
            return state;

        case HIGH:
            print("> High", Linje4);
            return state;
    }

    return state;
}