#pragma 

#include "opencv2/core.hpp"
#include "CBase4618.h"

#define RED cv::Scalar(0, 0, 255) ///< Red colour 
#define GREEN cv::Scalar(0, 255, 0) ///< Green colour 
#define BLUE cv::Scalar(255, 0, 0) ///< Blue colour 
#define YELLOW cv::Scalar(0, 255, 255) ///< Yellow colour 
#define WHITE cv::Scalar(255, 255, 255) ///< White colour
#define AMOUNT_OF_COLOURS 5 ///< Amount of colour supported for drawing and LED colour changing

#define ETCHASKETCH_WINDOW_SIZE cv::Size(1000, 750) ///< Size of the canvas 

#define JOYSTICK_X_SCALER 12 ///< Scaler for the joystick x input, used to translate the joystick position to a position on the canvas
#define JOYSTICK_Y_SCALER 11.5 ///< Scaler for the joystick y input, used to translate the joystick position to a position on the canvas
#define JOYSTICK_X_CENTER 47 ///< Center percentage value for the joystick x input
#define JOYSTICK_Y_CENTER 51 ///< Center percentage value for the joystick y input
#define JOYSTICK_DEADZONE 10 ///< Deadzone percentage for the joystick input, used to filter noise

#define SPEED_THRESHOLD 85 ///< Threshold percentage for the joystick input, used to determine whether to use slow or fast speed scale for drawing
#define SLOW_SPEED_SCALE 0.005 ///< Scale for slow speed drawing
#define FAST_SPEED_SCALE 0.02 ///< Scale for fast speed drawing

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

      double _joystick_x_percent = 0; ///< Percentage of joystick deflection in the x direction
      double _joystick_y_percent = 0; ///< Percentage of joystick deflection in the y direction

      double _x_incrementer = 0; ///< Increments or decrements the draw position in the x direction 
      double _y_incrementer = 0; ///< Increments or decrements the draw position in the x direction

      double _previous_x_draw_position = JOYSTICK_X_CENTER; ///< Previous x position that was drawn
      double _previous_y_draw_position = JOYSTICK_Y_CENTER; ///< Previous y position that was 

      cv::Point _draw_position; ///< Position to draw 
      cv::Point _previous_draw_position; ///< Previous position of that was drawn, used for smoothing lines
      cv::Rect _position_to_colour; ///< Area to be coloured based on the draw position

      cv::Scalar _colours[5] = { RED, GREEN, BLUE, YELLOW, WHITE }; ///< Colours to be used for drawing
      std::string _colour_names[5] = { "RED", "GREEN", "BLUE", "YELLOW", "WHITE" }; ///< Names of the colours to be displayed in the GUI
      int _colour_index = 0; ///< Index of the current colour being used for drawing and changing LED colour with button 2

      bool _smoothed = false; ///< Whether to draw smooth lines 

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

