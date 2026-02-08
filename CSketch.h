#pragma 

#include "opencv2/core.hpp"
#include "CBase4618.h"

#define RED cv::Scalar(0, 0, 255)
#define GREEN cv::Scalar(0, 255, 0)
#define BLUE cv::Scalar(255, 0, 0)
#define AMOUNT_OF_COLOURS 3

#define WINDOW_SIZE cv::Size(1000, 750)
#define JOYSTICK_X_SCALER 12.5
#define JOYSTICK_Y_SCALER 12.5

class CSketch : public CBase4618
    {
    private:
       int _reset = 0; ///< Whether the canvas should be cleared
		 cv::Point _joystick_position; ///< Position of the joystick
       cv::Point _previous_joystick_position; ///< Previous position of the joystick, used for smoothing lines

       cv::Scalar _colours[3] = { RED, GREEN, BLUE }; ///< Colours to be used for drawing
       std::string _colour_names[3] = { "RED", "GREEN", "BLUE" }; ///< Names of the colours to be displayed in the GUI
       int _colour_index = 0; ///< Index of the current colour being used for drawing and changing LED colour with button 2

       cv::Rect _position_to_colour; ///< Area to be coloured based on the joystick position, used for drawing
       bool _smoothed = false; ///< Whether to draw smooth lines 

    public:
       CSketch(cv::Size size, int comport);
       ~CSketch();

       /** @brief Communicates with the embedded system to get the value of a digital or analog input
       *
       * @param type The type of data to be set or received (Digital, Analog, or Servo)
       * @param channel The channel to communicate with
       *
       * @return Value of the digital or analog input, or 0 if an invalid type is passed
       */
       double CBase4618::gpio(int type, int channel) override;

       /** @brief Updates the state of the program based on the state of the game and inputs from the microcontroller via gpio
        *
        * @return True or false
        */
       bool CBase4618::update() override;

       /** @brief Performs all drawing functions onto the canvas and displays the canvas on the screen
       *
       * @return True or false
       */
       bool CBase4618::draw() override;

    };

