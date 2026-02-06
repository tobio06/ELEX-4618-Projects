
#include "stdafx.h"
#include "Lab3Functions.h"
#include <thread>


#define JOYSTICK_X 2 
#define JOYSTICK_Y 26
#define BUTTON2 32
#define ESC 27
#define BLUE_LED 37
#define SERVO_CHANNEL 0
#define SERVO_MAX 180
#define SERVO_MIN 0

CControl control;

void init()
{
	control.init_com(4);
}

void print_lab3_menu()
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


void analog_test()
{
	std::cout << "\n************************************";
	std::cout << "\n* ANALOG TEST, press ESC to exit";
	std::cout << "\n************************************";

	int x_val = 0, y_val = 0;

	while ( !(_kbhit() && _getch() == ESC) )
	{
		Sleep(20);

		float x_percent = control.get_analog(ANALOG, JOYSTICK_X, x_val); 
		float y_percent = control.get_analog(ANALOG, JOYSTICK_Y, y_val);

		std::cout << std::setprecision(2) << std::fixed;
		std::cout << "\nANALOG TEST: CH" << JOYSTICK_X << " = " << x_val << " (" << x_percent << "%)";
		std::cout << " CH" << JOYSTICK_Y << " = " << y_val << " (" << y_percent << "%)";

	}
}


void digital_test()
{
	std::cout << "\n************************************";
	std::cout << "\n* DIGITAL TEST, press ESC to exit";
	std::cout << "\n************************************";

	int val = 1;

	while (!(_kbhit() && _getch() == ESC))
	{
		Sleep(20);

		control.get_data(DIGITAL, BUTTON2, val);

		std::cout << "\nDIGITAL TEST: CH" << BUTTON2 << " = " << val;

		control.set_data(DIGITAL, BLUE_LED, !val);
	}
}


void button_test()
{
	std::cout << "\n************************************";
	std::cout << "\n* BUTTON TEST, press ESC to exit";
	//std::cout << "\n************************************";

	int val = 1;
	int previous_val = 1;
	int count = 0;

	while (!(_kbhit() && _getch() == ESC))
	{
		control.get_data(DIGITAL, BUTTON2, val);
		if (previous_val == 1 && val == 0) // detects falling edge
		{
			double time_pressed = cv::getTickCount();
			if (control.get_button(time_pressed)) // wait for debounce time
			{
				int check_button;
				control.get_data(DIGITAL, BUTTON2, check_button);
				if (check_button == 0) // if button is still presssed
				{
					count++;
					std::cout << "\nBUTTON TEST: " << count;
				}
			}
		}
		previous_val = val;
	}
}


void servo_test()
{
	std::cout << "\n************************************";
	std::cout << "\n* SERVO TEST, press ESC to exit";
	std::cout << "\n************************************";

	int servo_val = 0;
	bool exit = false;

	control.set_data(SERVO, SERVO_CHANNEL, SERVO_MAX);

	while (!exit)
	{
		if (_kbhit() && _getch() == ESC)
			break;

		control.get_data(SERVO, SERVO_CHANNEL, servo_val);

		if (servo_val == SERVO_MAX)
		{
			while (servo_val >= SERVO_MIN && !exit)
			{
				if (_kbhit() && _getch() == ESC)
					exit = true;

				Sleep(1);
				control.set_data(SERVO, SERVO_CHANNEL, servo_val--);
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
				control.set_data(SERVO, SERVO_CHANNEL, servo_val++);
				std::cout << "\nSERVO TEST: CH" << SERVO_CHANNEL << " = " << servo_val;
			}
		}
	}
}
