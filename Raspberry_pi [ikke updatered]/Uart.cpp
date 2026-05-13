#include "Uart.h"
#include <wiringSerial.h>
#include <chrono>
#include <thread>
#include <iostream>
#include <array>


int uart_open(const char* device, int baudrate)
{
    int fd = serialOpen(device, baudrate);
    if (fd < 0)
    {
        std::cerr << "Failed to open " << device << std::endl;
    }
    return fd;
}

void uart_close(int fd)
{
    if (fd >= 0)
    {
        serialClose(fd);
    }
}


void set_Temp(int ReadTemp){
    old_temp = temp;
    temp = ReadTemp;
}


int Read_uart(int fd)
{
    unsigned char data[DATA_size]{}; 
    int c;
    int index = 0;

    // Wait for start byte
    do {
        if (serialDataAvail(fd) <= 0)    return Def_Null;
        c = serialGetchar(fd);
    } while (c != Start_byte);

    // Read payload
    while (true)
    {
        if (serialDataAvail(fd) <= 0)    return Error;

        c = serialGetchar(fd);

        if (c == END_byte)
            break;

        if (index >= DATA_size)
        {
            std::cout << "Overflow\n";
            return Error;
        }

        data[index++] = c;
    }

    if (index == 0) return Error;

    return DATA_Handle(fd, data, index);
}


int DATA_Handle(int fd, unsigned char* data_ptr, int length)
{
    if (data_ptr[0] == Definition_Byte_Temp)
    {
        if (length >= 2)
        {
            int value = data_ptr[1];
            set_Temp(value);
            return Def_Temp;
        }
    }
    else if (data_ptr[0] == Definition_Byte_Sweep)
    {
        if (length < chairStates.size() + 1)    return Error;

        for (size_t i = 0; i < chairStates.size(); i++)
        {
            chairStates[i] = data_ptr[i + 1] * 4;
        }

        return Def_Sweep;
    }

    return Error;
}



void Set_LED(int fd,int Led,int state)
{
    serialPutchar(fd, Start_byte);
    serialPutchar(fd, Definition_Byte_LED);
    serialPutchar(fd, Led);
    serialPutchar(fd, state);
    serialPutchar(fd, END_byte);
}

void SET_ALL_LED (int fd) 
{
    for (int i = 0; i < chairStates.size(); i++){
        
        Set_LED(fd,i,chairStates[i]);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

    }	
}
