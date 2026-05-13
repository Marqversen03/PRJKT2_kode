#include "button.h"
#include "lcd.h"
#include "Uart.h"

#include <iostream>
#include <thread>
#include <unistd.h>
#include <array>
#include <chrono>


volatile int temp = 0;
volatile int old_temp = 0;

std::array<volatile int, 4> chairStates = {1, 1, 1, 1};

constexpr int  BUTTON_NEXT  = 17;
constexpr int  BUTTON_CLICK = 27;

int main()
{

//-----------------------------------------------------------//
    // UART init
    int fd_USB = uart_open(UART_DEVICE, UART_BAUDRATE);
    if (fd_USB < 0)        return 1;

    // LCD init
    int fd_LCD = lcd_init();
    if (fd_LCD < 0) return 1;

    // Button init
    if (!button_init()) return 1;
//-----------------------------------------------------------//
    //menu select chair and state
    int selectedChair = 0;   // 0 = ingen valgt, 1–4 = stol
    int selectedState = 0;   // 0–3

//-----------------------------------------------------------//
    //Uart read definition byte
	int UartResult = Def_Null;

//-----------------------------------------------------------//
    //Last button state
	bool next_old = false;
	bool click_old = false;

//-----------------------------------------------------------//
    // Button thread
    std::thread t(button_run);

//-----------------------------------------------------------//
    // Startmenu
    Menu_chair(fd_LCD, temp);

//-----------------------------------------------------------//



while (true)
{
        UartResult = Read_uart(fd_USB);

		if (UartResult == Def_Temp) Menu_chair(fd_LCD, temp);
		if (UartResult == Def_Sweep) SET_ALL_LED (fd_USB);


		std::this_thread::sleep_for(std::chrono::milliseconds(10));
        bool next_now = button_is_pressed(BUTTON_NEXT);
        bool click_now = button_is_pressed(BUTTON_CLICK);

        if (next_now && !next_old)
        {
			
            if (menu == MenuSelect::Chair)
            {
                selectedChair = Next_menu_chair(fd_LCD, selectedChair);
            }
            else if (menu == MenuSelect::State)
            {
                selectedState = Next_menu_state(fd_LCD, selectedState);
            }

        }


        if (click_now && !click_old)
        {
            if (menu == MenuSelect::Chair)
            {
                // Gå til status-menu hvis en stol er valgt
                if (selectedChair > 0)
                {
                    Menu_State(fd_LCD);
                    menu = MenuSelect::State;
                }
            }
            
            else if (menu == MenuSelect::State)
            {

                if (selectedState > 0){

                // Gem status i array
                chairStates[selectedChair - 1] = selectedState;
                
                // Set LED
				Set_LED(fd_USB,selectedChair,selectedState);
                
                // Reset menu
                selectedChair = 0;
                selectedState = 0;
                menu = MenuSelect::Chair;

                send_byte(fd_LCD, LCD_Clear, COMMAND); //Clear
                Menu_chair(fd_LCD, temp);

                }
            }

        }            
        click_old = click_now;
		next_old = next_now;

    }
    uart_close(fd_USB);
    t.join();
    button_cleanup();
}
