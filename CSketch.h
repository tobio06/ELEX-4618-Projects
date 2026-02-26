#pragma once

#include "opencv2/core.hpp"
#include "CBase4618.h"

#define RED cv::Scalar(0, 0, 255) ///< Red colour 
#define GREEN cv::Scalar(0, 255, 0) ///< Green colour 
#define BLUE cv::Scalar(255, 0, 0) ///< Blue colour 
#define YELLOW cv::Scalar(0, 255, 255) ///< Yellow colour 
#define WHITE cv::Scalar(255, 255, 255) ///< White colour
#define AMOUNT_OF_COLOURS 5 ///< Amount of colour supported for drawing and LED colour changing

#define ETCHASKETCH_WINDOW_SIZE cv::Size(1000, 750) ///< Size of the canvas 
#define SCREEN_CENTER cv::Point2f(ETCHASKETCH_WINDOW_SIZE.width / 2, ETCHASKETCH_WINDOW_SIZE.height / 2)

#define JOYSTICK_X_CENTER 47 ///< Center percentage value for the joystick x input
#define JOYSTICK_Y_CENTER 51 ///< Center percentage value for the joystick y input
#define JOYSTICK_DEADZONE 10 ///< Deadzone percentage for the joystick input, used to filter noise

#define SPEED_THRESHOLD 85 ///< Threshold percentage for the joystick input, used to determine whether to use slow or fast speed scale for drawing
#define SLOW_SPEED_SCALE 0.1 ///< Scale for slow speed drawing
#define FAST_SPEED_SCALE 0.5 ///< Scale for fast speed drawing

#define ACCEL_SHAKE_THRESHOLD 2000 ///< Threshold for the change in acceleration to trigger a shake to reset

/**
*
* @brief Holds all code for the Etch-A-Sketch lab in ELEX 4618
*
* This class is meant to contain all defines, variables and function implementations
* needed to create and run the Etch-A-Sketch lab for ELEX 4618.
*
* @author Tobio Yeung
*
*/
class CSketch : public CBase4618
   {
   private:
      int _reset = 0; ///< Whether the canvas should be cleared

      cv::Point2f _joystick_percent = cv::Point2f(JOYSTICK_X_CENTER, JOYSTICK_Y_CENTER); ///< Percentage of joystick deflection

      cv::Point2f _incrementer = cv::Point2f(0.0, 0.0); ///< Increments or decrements the draw position

      cv::Point2f _joystick_movement; ///< Direction and strength that the joystick is being pushed

      cv::Point2f _speed_scale = cv::Point2f(SLOW_SPEED_SCALE, SLOW_SPEED_SCALE);

      cv::Point2f _draw_position; ///< Position to draw 
      cv::Point2f _previous_draw_position; ///< Previous position that was drawn
      cv::Point2f _last_drawn_point; ///< Previous position that was drawn, used for smoothing lines
      cv::Rect _position_to_colour; ///< Area to be coloured based on the draw position

      cv::Scalar _colours[5] = { RED, GREEN, BLUE, YELLOW, WHITE }; ///< Colours to be used for drawing
      std::string _colour_names[5] = { "RED", "GREEN", "BLUE", "YELLOW", "WHITE" }; ///< Names of the colours to be displayed in the GUI
      int _colour_index = 0; ///< Index of the current colour being used for drawing and changing LED colour with button 2

      int _accel_y = 0; ///< Current x acceleration, used for shake to reset
      int _accel_z = 0; ///< Current y acceleration, used for shake to reset
      int _previous_accel_y = 0; ///< Previous y acceleration, used for shake to reset
      int _previous_accel_z = 0; ///< Previous z acceleration, used for shake to reset
      bool _shake_reset_triggered = false; ///< Whether shake to reset has been triggered, used for shake to reset

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

