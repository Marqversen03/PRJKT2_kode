#include "button.h"
#include "lcd.h"
#include "Uart.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <signal.h>
#include <array>

volatile bool running = true;

void handle_sigint(int)
{
    running = false;
}

int main()
{
    std::cout << "\n--- Varmelampe System ---\n";

    Button button;
    LCD lcd;
    ArduinoIF uart;

    // INIT
    if (!uart.init(UART_DEVICE, UART_BAUDRATE))
    {

        std::cout << "Uart Failed. |Check usb|\n";
        return 1;
        
    }

    if (!lcd.init())
    {
        std::cout << "LCD Failed. |Check Wires|\n";
        return 1;
        
    }

    if (!button.init())
    {
        std::cout << "Button Failed. |Check chip_path|\n";
        std::cout << "Button path prob not cleaned\n";
        return 1;
        
    }

    // BUTTON THREAD
    std::thread t(&Button::run, &button);


    // VAR
    int temp = 0;
    

    constexpr int BUTTON_NEXT  = 17;
    constexpr int BUTTON_CLICK = 27;

    MenuSelect menu = MenuSelect::Chair;

    int selectedChair = 0;
    int selectedState = None;

    bool next_old = false;
    bool click_old = false;

    lcd.Menu_chair(temp);

    signal(SIGINT, handle_sigint);

    while (running)
    {
        int result = uart.read();

        if (result == Def_Temp)
        {
            std::cout << "Temp updated\n";
        }

        uart.check_terminal();

        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        bool next_now  = button.is_pressed(BUTTON_NEXT);
        bool click_now = button.is_pressed(BUTTON_CLICK);

        // NEXT
        if (next_now && !next_old)
        {
            if (menu == MenuSelect::Chair)
                selectedChair = lcd.Next_menu_chair(selectedChair);
            else
                selectedState = lcd.Next_menu_state(selectedState);
        }

        // CLICK
        if (click_now && !click_old)
        {
            if (menu == MenuSelect::Chair && selectedChair > 0)
            {
                lcd.Menu_State();
                menu = MenuSelect::State;
            }
            else if (menu == MenuSelect::State && selectedState > 0)
            {
                std::cout << "  Next      button pressed \n";
                uart.set_led(selectedChair, selectedState);
                
                std::cout <<'\n';
                std::cout <<"-----------------------\n";
                std::cout << "Selected chair:  "   << selectedChair-1  <<'\n';
                std::cout << "Selected State:  "   << selectedState    <<'\n';
                std::cout <<"-----------------------\n";
                std::cout <<'\n';

                selectedChair = 0;
                selectedState = 0;
                menu = MenuSelect::Chair;

                lcd.Menu_chair(temp);
            }
        }

        next_old  = next_now;
        click_old = click_now;
    }    
        button.stop();
        if (t.joinable())
            t.join();

    return 0;
}