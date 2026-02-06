#pragma once

#include "Serial.h"
#include "opencv2/core.hpp"

enum _type
{
	DIGITAL,
	ANALOG,
	SERVO
};

/**
* 
* @brief Communicates with Tiva Launchpad 
* 
* This class is meant to contain all variables and functions needed to
* communicate with embedded systems.
* 
* @author Tobio Yeung
* 
*/
class CControl {
private:
	Serial _com; ///< Port control
public:
	CControl();
	~CControl();

	/** @brief Initializes the serial port to the com port passed
	*
	* @param comport The com port to which the serial port is initialized
	* @return Nothing returned
	*/
	void init_com(int comport);

	/** @brief Receives data from the embedded system
	*
	* @param type The type of data to be received (Digital, Analog, or Servo)
	* @param channel The channel to receive data from
	* @param result The received data value
	* 
	* @return True or false
	*/
	bool get_data(int type, int channel, int& result);

	/** @brief Sends data to the embedded system
	*
	* @param type The type of data to be sent (Digital, Analog, or Servo)
	* @param channel The channel to send data to
	* @param result The data value being sent
	*
	* @return True or false
	*/
	bool set_data(int type, int channel, int val);

	/** @brief Calls the get_data function and returns the analog input as a percentage
	* 
	* @return The percentage of the full scale
	*/
	float get_analog(int type, int channel, int& result);

	/** @brief Reads a digital input and debounces it 
	*
	* @param press_time The time when the button is pressed
	* 
	* @return True or false
	*/
	bool get_button(double press_time);
};