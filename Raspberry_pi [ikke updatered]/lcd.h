#ifndef LCD_H
#define LCD_H

#include <cstdint>
#include <string>

#define LCD_ADDR 0x27

#define LCD_Clear 0x01
#define LCD_Backlight 0x08
#define LCD_Enable 0x04
#define LCD_Degree 0xDF
#define Bit_mask 0xF0

#define Linje1 0x80
#define Linje2 0xC0
#define Linje3 0x94
#define Linje4 0xD4

class LCD
{
private:
    int fd;

    void lcd_write(uint8_t data);
    void pulse(uint8_t data);
    void send_byte(uint8_t val, uint8_t mode);

public:
    LCD();
    ~LCD();

    bool init();

    void print(const std::string& text, uint8_t start);

    void Menu_chair(int temp);
    void Menu_State();

    int Next_menu_chair(int chair);
    int Next_menu_state(int state);
};

enum class MenuSelect {
    Chair,
    State,
};

enum ChairState {
    None = 0,
    OFF,
    LOW,
    MID,
    HIGH
};

enum LCDMODE {
    COMMAND = 0,
    DATA = 1
};

#endif