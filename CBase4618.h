#pragma once
#include "opencv2/core.hpp"
#include "Control.h"

#define _30_FPS 33 ///< Delay in milliseconds to achieve approximately 30 frames per second

/**
*
* @brief Holds common code for all labs in ELEX 4618
*
* This class is meant to contain all common variables and functions needed to
* create and run programs for ELEX 4618. 
* It is meant to be inherited by all lab classes, and should not be instantiated on its own.
*
* @author Tobio Yeung
*
*/
class CBase4618
    {
    protected:
       CControl _control; ///< Object for communicating with embedded systems
       cv::Mat _canvas; ///< Canvas for drawing, can be used for displaying images, etc.

    public:
       CBase4618();
       ~CBase4618();

       /** @brief Communicates with the embedded system to get the value of a digital or analog input
       *
       * @param type The type of data to be set or received (Digital, Analog, or Servo)
       * @param channel The channel to communicate with
       *
       * @return Value of the digital or analog input, or 0 if an invalid type is passed
       */
       virtual double gpio(int type, int channel) = 0;

       /** @brief Updates the state of the program based on the state of the game and inputs from the microcontroller via gpio
       *
       * @return True or false
       */
       virtual bool update() = 0;

       /** @brief Performs all drawing functions onto the canvas and displays the canvas on the screen
       *
       * @return True or false
       */
       virtual bool draw() = 0;

       /** @brief calls update, gpio and draw to run the program, and quits when the user presses the 'q' key
       *
       * @return Returns nothing
       */
       void run();
    };

