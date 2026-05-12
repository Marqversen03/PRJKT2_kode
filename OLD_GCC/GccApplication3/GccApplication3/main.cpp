#include "uart.h"
#include "Varmelampe.h"
#include "Initial_Timer_5.h"

#include <stdlib.h>

void Senttemp()
{	
	int Temp;
	SendChar(0xAA);
	SendChar('T');
	Temp = (rand() % 17) + 5;
	SendChar(Temp);
	SendChar(0xBB);
}


void SentPos(bool *Pos)
{
	SendChar(0xAA);
	
	SendChar(Pos[0]);
	SendChar(Pos[1]);
	SendChar(Pos[2]);
	SendChar(Pos[3]);

	SendChar(0xBB);
}


int main(void)
{
	
	//int Temp{};
	int state;
	int Varmelampe{};
	pwmInit();
	UART0_Init();
	timer5_init();
	
	while (1)
	{
		if (Check_temp == 1 )
		{
			Senttemp();
			Check_temp = 0;
		}
		if (Check_Pos == 3 )
		{
			SentPos();
			Check_Pos = 0;
		}Check_Pos
	
		if (FrameReady)
		{
			
			if (Data_Array[0] == 'T') //GET TEMP
			{

			}
			if (Data_Array[0] == 'S') //Sweep
			{
				
			}
			if (Data_Array[0] == 'V') // Varmelampe
			{
				Varmelampe = Data_Array[1];
				state = int_to_state(Data_Array[2]);
				setLed(Varmelampe,state);
			}
			
			FrameReady = 0;
		}
	}
}