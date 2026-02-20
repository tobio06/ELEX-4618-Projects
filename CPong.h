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
#define SLOW_SPEED_SCALE 0.05 ///< Scale for slow speed 
#define FAST_SPEED_SCALE 0.2 ///< Scale for fast speed

#define PADDLE_WIDTH 25 ///< Width of the paddle
#define PADDLE_HEIGHT 150 ///< Height of the paddle
#define ORIGINAL_BALL_RADIUS 50 ///< Original radius of the ball


#define _30_FPS_DELAY 1000 / 30 ///< Delay in milliseconds to achieve approximately 30 frames per second

class CPong : public CBase4618
   {
   private:
      int _reset = 0; ///< Whether the canvas should be cleared
      int _game_over = 0; ///< Whether the game is over and should be reset

      double _joystick_percent = 0; ///< Percentage of joystick deflection
      double _y_incrementer = 0; ///< Increments or decrements the draw position in the x direction

      double _paddle_position; ///< Position of paddle 
      double _previous_paddle_position = PONG_WINDOW_SIZE.height / 2; ///< Previous position of paddle

      cv::Rect _player_paddle = cv::Rect(); ///< Rectangle representing the player's paddle
      cv::Rect _computer_paddle = cv::Rect(); ///< Rectangle representing the computer's paddle
      cv::Rect _ball_hitbox = cv::Rect(); ///< Rectangle representing the hitbox of the ball, used for collision detection with paddles

      double _fps = 0.0; ///< Frames per second
      std::string _fps_string = ""; ///< String representation of the frames per second to be displayed in the GUI

      double _ball_radius = 50; ///< Original radius of the ball
      double _ball_speed = 250; ///< Speed of the ball in pixels per frame
      cv::Point _ball_velocity = cv::Point(_ball_speed, _ball_speed); ///< Velocity of the ball in pixels per frame
      cv::Point _ball_position = SCREEN_CENTER; ///< Position of the ball

      int64 _time_start_frame; ///< Time point for the start of the frame, used for calculating elapsed time and FPS
      double _time_last_frame; ///< Time point for the last frame, used for calculating elapsed time and FPS

      int _player_score = 0; ///< Player's score
      int _computer_score = 0; ///< Computer's score

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

