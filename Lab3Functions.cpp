
#include "stdafx.h"
#include "Lab3Functions.h"
#include <thread>

//CControl control;

//void CLab3Functions::init()
//{
//	init_com(4);
//}

void CLab3Functions::print_lab3_menu()
{
	std::cout << "\n**********************************************";
	std::cout << "\n* ELEX4618 Lab 3: Embedded Systems Control";
	std::cout << "\n**********************************************";
	std::cout << "\n(A) Analog Test";
	std::cout << "\n(D) Digital Test";
	std::cout << "\n(B) Button Debounce Test";
	std::cout << "\n(S) Servo test";
	std::cout << "\n(Q) Quit";
	std::cout << "\nCMD> ";
}


void CLab3Functions::analog_test()
{
	std::cout << "\n************************************";
	std::cout << "\n* ANALOG TEST, press ESC to exit";
	std::cout << "\n************************************";

	int x_val = 0, y_val = 0;

	while ( !(_kbhit() && _getch() == ESC) )
	{
		Sleep(20);

		float x_percent = get_analog(ANALOG, JOYSTICK_X, x_val); 
		float y_percent = get_analog(ANALOG, JOYSTICK_Y, y_val);

		std::cout << std::setprecision(2) << std::fixed;
		std::cout << "\nANALOG TEST: CH" << JOYSTICK_X << " = " << x_val << " (" << x_percent << "%)";
		std::cout << " CH" << JOYSTICK_Y << " = " << y_val << " (" << y_percent << "%)";

	}
}


void CLab3Functions::digital_test()
{
	std::cout << "\n************************************";
	std::cout << "\n* DIGITAL TEST, press ESC to exit";
	std::cout << "\n************************************";

	int val = 1;

	while (!(_kbhit() && _getch() == ESC))
	{
		Sleep(20);

		get_data(DIGITAL, BUTTON2, val);

		std::cout << "\nDIGITAL TEST: CH" << BUTTON2 << " = " << val;

		set_data(DIGITAL, BLUE_LED, !val);
	}
}


void CLab3Functions::button_test()
{
	std::cout << "\n************************************";
	std::cout << "\n* BUTTON TEST, press ESC to exit";
	std::cout << "\n************************************";

	int count = 0;

	while (!(_kbhit() && _getch() == ESC))
	{
		if (get_button(BUTTON2))
		{
			count++;
			std::cout << "\nBUTTON TEST: " << count;
		}
	}
}


void CLab3Functions::servo_test()
{
	std::cout << "\n************************************";
	std::cout << "\n* SERVO TEST, press ESC to exit";
	std::cout << "\n************************************";

	int servo_val = 0;
	bool exit = false;

	set_data(SERVO, SERVO_CHANNEL, SERVO_MAX);

	while (!exit)
	{
		if (_kbhit() && _getch() == ESC)
			break;

		get_data(SERVO, SERVO_CHANNEL, servo_val);

		if (servo_val == SERVO_MAX)
		{
			while (servo_val >= SERVO_MIN && !exit)
			{
				if (_kbhit() && _getch() == ESC)
					exit = true;

				Sleep(1);
				set_data(SERVO, SERVO_CHANNEL, servo_val--);
				std::cout << "\nSERVO TEST: CH" << SERVO_CHANNEL << " = " << servo_val;
			}
		}
		else if (servo_val == SERVO_MIN)
		{
			while (servo_val <= SERVO_MAX && !exit)
			{
				if (_kbhit() && _getch() == ESC)
					exit = true;

				Sleep(1);
				set_data(SERVO, SERVO_CHANNEL, servo_val++);
				std::cout << "\nSERVO TEST: CH" << SERVO_CHANNEL << " = " << servo_val;
			}
		}
	}
}
