#ifndef LCD_H
#define LCD_H

#include <cstdint>
#include <string>

// LCD I2C address
#define LCD_ADDR 0x27

//commands
#define LCD_Clear 0x01
#define LCD_Backlight 0x08
#define LCD_Enable 0x04
#define LCD_Degree 0xDF

#define Bit_mask 0xF0

//location
#define Linje1 0x80
#define Linje2 0xC0
#define Linje3 0x94
#define Linje4 0xD4

// init
int  lcd_init();

//writing funktion
void lcd_write(int fd, uint8_t data);
void pulse(int fd, uint8_t data);
void send_byte(int fd, uint8_t val, uint8_t mode);
void lcd_print(int fd, const std::string&,uint8_t start);


// menu display
void Menu_chair(int fd, int temp);
void Menu_State(int fd);

// select menu display
int Next_menu_chair(int fd,int chair);
int Next_menu_state(int fd,int state);



enum class MenuSelect {
    Chair,
    State,
};

extern MenuSelect menu;

enum ChairState {
    OFF = 0,
    LOW,
    MID,
    HIGH
};


enum LCDMODE {
    COMMAND = 0,
    DATA = 1
};



#endif // LCD_H
