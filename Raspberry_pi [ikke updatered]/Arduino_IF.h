#ifndef Arduino_IF_H
#define Arduino_IF_H

#include <array>

class ArduinoIF
{
private:
    int fd;

public:
    ArduinoIF();
    ~ArduinoIF();

    bool init(const char* device, int baudrate);
    void close();

    int read();
    int data_handle(unsigned char* data_ptr, int length);

    void set_temp(int value);
    void check_terminal();

    void set_led(int led, int state);
    void set_all_led();

    void rotate_motor(int degree);
    void scan();
};

// --- Constante Variabler

constexpr const char* UART_DEVICE = "/dev/ttyACM1";
constexpr int UART_BAUDRATE = 9600;

#define Start_byte 0xAA
#define END_byte   0xBB
#define DATA_size  10

#define Definition_Byte_Temp   'T'
#define Definition_Byte_Sweep  'S'
#define Definition_Byte_Rotate 'R'
#define Definition_Byte_LED    'V'

enum DefByte {
    Def_Null = 0,
    Def_Temp,
    Def_Sweep,
    Error
};

// Globale variabler

extern volatile int temp;
extern volatile int old_temp;
extern volatile bool ChangeTemp;
extern std::array<volatile int, 4> chairStates;

#endif
