#ifndef UART_H
#define UART_H
#include <array>

int uart_open(const char* device, int baudrate);
void uart_close(int fd);

void Set_LED(int fd,int Led,int state);
int Read_uart(int fd);
void set_Temp(int ReadTemp);
void SET_ALL_LED (int fd) ;


extern volatile int temp;
extern volatile int old_temp;

extern std::array<volatile int, 4> chairStates;


constexpr const char* UART_DEVICE = "/dev/ttyACM0";
constexpr int UART_BAUDRATE = 9600;



enum DefByte {
    Def_Null = 0,
    Def_Temp,
    Def_Sweep,
    Error
};

#define Start_byte 0xAA
#define END_byte 0xBB
#define DATA_size 10
#define Definition_Byte_Temp 'T'
#define Definition_Byte_Sweep 'S'
#define Definition_Byte_LED 'V'




#endif // UART_H
