#pragma once

#include "Serial.h"
#include "opencv2/core.hpp"
#include <string>
#include <sstream>

#define JOYSTICK_X 2 
#define JOYSTICK_Y 26
#define BUTTON1 33
#define BUTTON2 32
#define ACCELEROMETER_X 23
#define ACCELEROMETER_Y 24
#define ACCELEROMETER_Z 25
#define ESC 27
#define RED_LED 39
#define GREEN_LED 38
#define BLUE_LED 37
#define SERVO_CHANNEL 0
#define SERVO_MAX 180
#define SERVO_MIN 0

enum _type ///< Type of data being sent or received
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
   int _previous_val = 1; ///< Previous value of the button, used for debouncing
   double _time_of_button1_press = -1.0; ///< Time when button 1 was pressed, used for debouncing
   double _time_of_valid_button1_debounce = -1.0; ///< Time when button 1 was last validly debounced, used for debouncing
	double _time_of_button2_press = -1.0; ///< Time when button 2 was pressed, used for debouncing
	double _time_of_valid_button2_debounce = -1.0; ///< Time when button 2 was last validly debounced, used for debouncing

public:
	CControl();
	~CControl();

	/** @brief Initializes the serial port to the com port passed
	*
	* @param comport The com port to which the serial port is initialized
	* 
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
	* @param type The type of data to be received (Digital, Analog, or Servo)
	* @param channel The channel to receive data from
	* @param result The received percentage value
	* 
	* @return The percentage of the full scale
	*/
	float get_analog(int type, int channel, int& result);

	/** @brief Reads a digital input and debounces it 
	*
   * @param channel The channel to receive data from
	* 
	* @return True or false
	*/
	bool get_button(int channel);
};