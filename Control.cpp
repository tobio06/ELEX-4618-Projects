
#include "stdafx.h"
#include "Control.h"
#include "Lab3Functions.h"

#define DEBOUNCE_TIME 0.05 // seconds

CControl::CControl() {}

CControl::~CControl() {}

void CControl::init_com(int comport)
{
	std::string com_port(std::to_string(comport));
	_com.open("COM" + com_port);
}


bool CControl::get_data(int type, int channel, int& result)
{
	// transmit data to device
	std::stringstream tx;
	tx << "G" << " " << type << " " << channel << "\n";
	std::string tx_str = tx.str();
	_com.write(tx_str.c_str(), tx_str.length());
	
	// wait for ACK
	char buff[2];
	std::string rx_str;
	do
	{
		rx_str = "";
		// start timeout count
		double start_time = cv::getTickCount();

		buff[0] = 0;
		// Read 1 byte and if an End Of Line then exit loop
		while (buff[0] != '\n' && (cv::getTickCount() - start_time) / cv::getTickFrequency() < 1.0)
		{
			if (_com.read(buff, 1) > 0)
			{
				rx_str = rx_str + buff[0];
			}
		}
		if ((cv::getTickCount() - start_time) / cv::getTickFrequency() >= 1.0) { return false; }
		else if (buff[0] == '\n')
		{
			std::stringstream rx(rx_str);
			char ack;
			int type_rx, channel_rx, value_rx;

			rx >> ack >> type_rx >> channel_rx >> value_rx;
			if (ack != 'A') continue;

			result = value_rx;
			return true;
		}
	} while (1);
}


bool CControl::set_data(int type, int channel, int val)
{
	std::stringstream tx;
	tx << "S" << " " << type << " " << channel << " " << val << "\n";
	std::string tx_str = tx.str();
	_com.write(tx_str.c_str(), tx_str.length());

	// wait for ACK
	char buff[2];
	std::string rx_str;
	do
	{
		rx_str = "";
		// start timeout count
		double start_time = cv::getTickCount();

		buff[0] = 0;
		// Read 1 byte and if an End Of Line then exit loop
		while (buff[0] != '\n' && (cv::getTickCount() - start_time) / cv::getTickFrequency() < 1.0)
		{
			if (_com.read(buff, 1) > 0)
			{
				rx_str = rx_str + buff[0];
			}
		}
		if ((cv::getTickCount() - start_time) / cv::getTickFrequency() >= 1.0) { return false; }
		else if (buff[0] == '\n')
		{
			std::stringstream rx(rx_str);
			char ack;
			int type_rx, channel_rx, value_rx;

			rx >> ack >> type_rx >> channel_rx >> value_rx;
			if (ack != 'A') continue;

			return true;
		}
	} while (1);
}


float CControl::get_analog(int type, int channel, int& result)
{
	CControl::get_data(type, channel, result);
	float result_percent = result / 4096.0 * 100;
	return result_percent;
}


bool CControl::get_button(int channel)
{
	int val = 1;
	if (!get_data(DIGITAL, channel, val))
		return false;

	double now = cv::getTickCount() / cv::getTickFrequency();
	if (channel == BUTTON1)
		{
		if (val == 0)
			{
			if (_time_of_button1_press < 0.0) // first press
				{
				_time_of_button1_press = now;
				}
			else // when button is being held
				{
				if ((now - _time_of_button1_press >= DEBOUNCE_TIME) && (_time_of_valid_button1_debounce < _time_of_button1_press))
					{
					_time_of_valid_button1_debounce = now;
					return true;
					}
				}
			}
		else
			{
			_time_of_button1_press = -1.0;
			}
		}
	else if (channel == BUTTON2)
		{
		if (val == 0)
			{
			if (_time_of_button2_press < 0.0) // first press
				{
				_time_of_button2_press = now;
				}
			else // when button is being held
				{
				if ((now - _time_of_button2_press >= DEBOUNCE_TIME) && (_time_of_valid_button2_debounce < _time_of_button2_press))
					{
					_time_of_valid_button2_debounce = now;
					return true;
					}
				}
			}
		else
			{
			_time_of_button2_press = -1.0;
			}
		}
	return false;
}
