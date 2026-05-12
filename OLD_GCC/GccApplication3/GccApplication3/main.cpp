#include "uart.h"
#include "Initial_Timer_5.h"
#include "Varmelampe.h"

void Timer();
void Uart();


int main(void)
{
	pwmInit();
	UART0_Init();
	timer5_init();

	while (1)
	{
		Timer();
		Uart();
	}
}


void Timer()
{
	if (Check_temp) // 5 min timer
	{
		SendTemp();
		Check_temp = 0;
	}

	if (Check_Pos) // 15 min timer
	{
		// send position
		Check_Pos = 0;
	}
}

void Uart()
{
	if (!FrameReady) return;

	char ByteDef = Data_Array[0];

	if (ByteDef == 'R')   // Rotate
	{
		int degree = Data_Array[1];
		// Rotate(degree);
	}

	if (ByteDef == 'V')   // Varmelampe
	{
		int lamp = Data_Array[1];
		int state = int_to_state(Data_Array[2]);
		setLed(lamp, state);
	}

	FrameReady = 0;
}