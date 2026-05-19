#include "Arduino_IF.h"
#include <wiringSerial.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <sys/select.h>
#include <unistd.h>

volatile int temp = 0;
volatile int old_temp = 0;
volatile bool ChangeTemp = false;

std::array<volatile int, 4> chairStates = {1, 1, 1, 1};


ArduinoIF::UART() : fd(-1) {}

ArduinoIF::~UART()
{
    close();
}


bool ArduinoIF::init(const char* device, int baudrate)
{
    fd = serialOpen(device, baudrate);

    if (fd < 0)
    {
        std::cerr << "Failed to open " << device << "\n";
        return false;
    }

    return true;
}

void ArduinoIF::close()
{
    if (fd >= 0)
    {
        serialClose(fd);
        std::cout << "UART closed\n";
        fd = -1;
    }
}

void ArduinoIF::set_temp(int value)
{
    old_temp = temp;
    temp = value;
    ChangeTemp = true;
}


int ArduinoIF::read()
{
    unsigned char data[DATA_size]{};
    int c;
    int index = 0;

    do
    {
        if (serialDataAvail(fd) <= 0)
            return Def_Null;

        c = serialGetchar(fd);
    } while (c != Start_byte);

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        if (serialDataAvail(fd) <= 0)
            return Error;

        c = serialGetchar(fd);

        if (c == END_byte)
            break;

        if (index >= DATA_size)
            return Error;

        data[index++] = c;
    }

    return data_handle(data, index);
}


int ArduinoIF::data_handle(unsigned char* data_ptr, int length)
{
    if (data_ptr[0] == Definition_Byte_Temp)
    {
        if (length >= 2)
        {
            set_temp(data_ptr[1]);
            return Def_Temp;
        }
    }
    else if (data_ptr[0] == Definition_Byte_Sweep)
    {
        if (length < chairStates.size() + 1)
            return Error;

        for (size_t i = 0; i < chairStates.size(); i++)
        {
            chairStates[i] = data_ptr[i + 1] * 4;
        }

        return Def_Sweep;
    }

    return Error;
}


void ArduinoIF::rotate_motor(int degree)
{
    serialPutchar(fd, Start_byte);
    serialPutchar(fd, Definition_Byte_Rotate);
    serialPutchar(fd, degree);
    serialPutchar(fd, END_byte);
}

void ArduinoIF::scan()
{
    serialPutchar(fd, Start_byte);
    serialPutchar(fd, Definition_Byte_Sweep);
    serialPutchar(fd, END_byte);
}

void ArduinoIF::set_led(int led, int state)
{
    serialPutchar(fd, Start_byte);
    serialPutchar(fd, Definition_Byte_LED);
    serialPutchar(fd, led);
    serialPutchar(fd, state);
    serialPutchar(fd, END_byte);
}

void ArduinoIF::set_all_led()
{
    for (int i = 0; i < (int)chairStates.size(); i++)
    {
        set_led(i, chairStates[i]);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

// ---------------- TERMINAL ----------------

void ArduinoIF::check_terminal()
{
    fd_set set;
    struct timeval timeout = {0, 0};

    FD_ZERO(&set);
    FD_SET(STDIN_FILENO, &set);

    if (select(STDIN_FILENO + 1, &set, NULL, NULL, &timeout) > 0)
    {
        char cmd;
        int value;

        if (std::cin >> cmd >> value)
        {
            if (cmd == 'R')
            {
                rotate_motor(value);
            }
            else if (cmd == 'T')
            {
                set_temp(value);
            }
            else if (cmd == 'S')
            {
                scan();
            }
        }
    }
}
