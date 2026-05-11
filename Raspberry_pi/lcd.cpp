
#include "lcd.h"
#include <chrono>
#include <thread>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

void lcd_write(int fd, uint8_t data) {
    data |= LCD_Backlight;  // backlight ON
    write(fd, &data, 1);    // 1 = char count
    std::this_thread::sleep_for(std::chrono::microseconds(1000));

}

void pulse(int fd, uint8_t data) {
    lcd_write(fd, data | LCD_Enable);   // EN = 1
    lcd_write(fd, data & ~LCD_Enable);  // EN = 0
}

void send_byte(int fd, uint8_t val, uint8_t mode) {
    uint8_t high = (val & Bit_mask) | mode;
    uint8_t low  = ((val << 4) & Bit_mask) | mode;

    pulse(fd, high);
    pulse(fd, low);
}

void lcd_print(int fd, const std::string& text,uint8_t start) {
    send_byte(fd, start, COMMAND); // start lokalition

    for (char c : text)
        send_byte(fd, c, DATA);
}

int lcd_init() {
    int fd = open("/dev/i2c-1", O_RDWR);
    if (fd < 0) return -1;

    ioctl(fd, I2C_SLAVE, LCD_ADDR);
    
    std::this_thread::sleep_for(std::chrono::microseconds(50000));

    //4 bit system init
    pulse(fd, 0x30);    //8-bit init
    pulse(fd, 0x30);    //8-bit init
    pulse(fd, 0x30);    //8-bit init
    pulse(fd, 0x20);    //4-bit init

    send_byte(fd, 0x28, COMMAND);           // 4-bit, 2-line
    send_byte(fd, 0x08, COMMAND);           // display ON
    send_byte(fd, LCD_Clear, COMMAND);      // clear

    std::this_thread::sleep_for(std::chrono::microseconds(2000));

    send_byte(fd, 0x06, COMMAND);   // entry mode
    send_byte(fd, 0x0C, COMMAND);   // display ON

    return fd;
}


// -- LAYOUT --

void Menu_chair(int fd, int temp) {
    send_byte(fd, Clear, COMMAND);
    
    std::string string_temp = std::to_string(temp);

    lcd_print(fd, string_temp, Linje1+15);

    send_byte(fd, LCD_Degree, DATA);    // degree symbol

    lcd_print(fd,"   [1]     [2]",Linje2);

    lcd_print(fd,"   [3]     [4]",Linje4);
}

int Next_menu_chair(int fd,int chair) {
    chair += 1;
    
    switch (chair) {
        case 1:
            lcd_print(fd,"x",Linje2+4);     // + offset
            return chair;
            
        case 2:
            lcd_print(fd,"1",Linje2+4);
            lcd_print(fd,"x",Linje2+12);
            return chair;
            
        case 3:
            lcd_print(fd,"2",Linje2+12);
            lcd_print(fd,"x",Linje4+4);
            return chair;
            
        case 4:
            lcd_print(fd,"3",Linje4+4);
            lcd_print(fd,"x",Linje4+12);
            return chair;
        case 5:
            lcd_print(fd,"4",Linje4+12);
            chair = 0;
            return chair;
        }
    return chair;
}

void Menu_State(int fd) {
    send_byte(fd, Clear, COMMAND);

    lcd_print(fd,"OFF",Linje1);

    lcd_print(fd,"Low",Linje2);

    lcd_print(fd,"Mid",Linje3);
    
    lcd_print(fd,"High",Linje4);

}

int Next_menu_state(int fd,int state) {
    state = (state+1)%4;

    Menu_State(fd);

    switch (state) {
        case OFF:
            lcd_print(fd,"> OFF",Linje1);
            return state;
            
        case LOW:
            lcd_print(fd,"> Low",Linje2);
            return state;
            
        case MID:
            lcd_print(fd,"> Mid",Linje3);
            return state;
            
        case HIGH:
            lcd_print(fd,"> High",Linje4);
            return state;

        }
    return state;
}


