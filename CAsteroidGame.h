#pragma once

#include "opencv2/core.hpp"
#include "CBase4618.h"
#include "CSketch.h"
#include "CShip.h"
#include "CAsteroid.h"
#include "CMissile.h"

#define CANVAS_NAME "Asteroids" ///< Name of the canvas 
#define BOARD_SIZE cv::Size(1280, 720)
#define BOARD_CENTER cv::Point2f(BOARD_SIZE.width / 2, BOARD_SIZE.height / 2)
#define SPAWN_DELAY 2

class CAsteroidGame : public CBase4618
    {
    protected:
        std::vector<CAsteroid> _asteroid_list;
        std::chrono::steady_clock::time_point _last_asteroid_spawn;

        // std::vector<CMissile> _missile_list;

        CShip ship;

        cv::Point2f _joystick_percent = (JOYSTICK_X_CENTER, JOYSTICK_Y_CENTER); ///< Percentage of joystick deflection

        cv::Point2f _incrementer = (0.0, 0.0); ///< Increments or decrements the draw position

        cv::Point2f _joystick_movement; ///< Direction and strength that the joystick is being pushed

        cv::Point2f _speed_scale = (SLOW_SPEED_SCALE, SLOW_SPEED_SCALE);

        cv::Point2f _draw_position; ///< Position to draw 
        cv::Point2f _previous_draw_position; ///< Previous position that was drawn

    public: 
       CAsteroidGame(cv::Size size, int comport);
       ~CAsteroidGame();

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
       /** @brief calls update, gpio and draw to run the program, and quits when the user presses the 'q' key
       *
       * @return Returns nothing
       */
       void run();
    };

