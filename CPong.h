#pragma once

#include "opencv2/core.hpp"
#include "CBase4618.h"
#include <thread>

#define CANVAS_NAME "Pong" ///< Name of the canvas and window for the Pong game
#define PONG_WINDOW_SIZE cv::Size(1000, 800) ///< Size of the canvas 
#define SCREEN_CENTER cv::Point(PONG_WINDOW_SIZE.width / 2, PONG_WINDOW_SIZE.height / 2) ///< Center point of the canvas

#define JOYSTICK_Y_SCALER 11.5 ///< Scaler for the joystick y input, used to translate the joystick position to a position on the canvas
#define JOYSTICK_Y_CENTER 51 ///< Center percentage value for the joystick y input
#define JOYSTICK_DEADZONE 10 ///< Deadzone percentage for the joystick input, used to filter noise

#define SPEED_THRESHOLD 85 ///< Threshold percentage for the joystick input, used to determine whether to use slow or fast speed scale 
#define SLOW_SPEED_SCALE 0.005 ///< Scale for slow speed 
#define FAST_SPEED_SCALE 0.02 ///< Scale for fast speed

#define PADDLE_WIDTH 25 ///< Width of the paddle
#define PADDLE_HEIGHT 150 ///< Height of the paddle
#define ORIGINAL_BALL_RADIUS 50 ///< Original radius of the ball


#define _30_FPS 1000 / 30 ///< Delay in milliseconds to achieve approximately 30 frames per second

class CPong : public CBase4618
   {
   private:
      int _reset = 0; ///< Whether the canvas should be cleared

      double _joystick_percent = 0; ///< Percentage of joystick deflection
      double _y_incrementer = 0; ///< Increments or decrements the draw position in the x direction

      double _previous_y_draw_position = JOYSTICK_Y_CENTER; ///< Previous y position that was 

      cv::Point _draw_position; ///< Position to draw 
      cv::Point _previous_draw_position; ///< Previous position of that was drawn, used for smoothing lines
      cv::Rect _position_to_colour; ///< Area to be coloured based on the draw posit

      cv::Rect _player_paddle = cv::Rect(); ///< Rectangle representing the player's paddle
      cv::Rect _computer_paddle = cv::Rect(); ///< Rectangle representing the computer's paddle

      std::chrono::high_resolution_clock::time_point _frame_start; ///< Time point for the start of the frame, used for calculating elapsed time and FPS
      double _fps = 0.0; ///< Frames per second
      std::string _fps_string = ""; ///< String representation of the frames per second to be displayed in the GUI

      double _ball_radius = 50; ///< Original radius of the ball
      cv::Point _ball_velocity = cv::Point(5, 5); ///< Velocity of the ball in pixels per frame
      cv::Point _ball_position = SCREEN_CENTER; ///< Position of the ball

   public:
      CPong(cv::Size size, int comport);
      ~CPong();

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

