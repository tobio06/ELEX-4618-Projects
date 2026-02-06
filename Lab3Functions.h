#pragma once

#include "opencv2/core.hpp"
#include "Control.h"
#include <string>
#include <sstream>
#include <conio.h>

class CLab3Functions : public CControl {
private:

public:

	//void init();

	void print_lab3_menu();

	void analog_test();

	void digital_test();

	void button_test();

	void servo_test();
};

