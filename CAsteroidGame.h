#pragma once

#include "opencv2/core.hpp"
#include "CBase4618.h"
#include "CSketch.h"
#include "CShip.h"
#include "CAsteroid.h"
#include "CMissile.h"

#define CANVAS_NAME "Asteroids" ///< Name of the canvas 
#define BOARD_SIZE cv::Size(1280, 720) ///< Size of board
#define BOARD_CENTER cv::Point2f(BOARD_SIZE.width / 2, BOARD_SIZE.height / 2) ///< Center of board
#define SPAWN_DELAY 2 ///< Delay for asteroid spawning
#define SLOW_ACCEL_SCALE 10 ///< Scale for slow acceleration
#define FAST_ACCEL_SCALE 20 ///< Scale for fast acceleration

/**
*
* @brief Holds all code for Asteroids game in ELEX 4618
*
* This class is meant to contain all common variables and functions needed to
* run Asteroids for ELEX 4618.
*
* @author Tobio Yeung
*
*/
class CAsteroidGame : public CBase4618
    {
    protected:
        std::vector<CAsteroid> _asteroid_list; ///< vector of asteroids
        std::chrono::steady_clock::time_point _last_asteroid_spawn; ///< last time an asteroid spawned

        std::vector<CMissile> _missile_list; ///< vector of missiles
        float _missile_speed = 800.0; ///< speed of missiles

        CShip _ship; ///< ship for Asteroids
        cv::Point2f _ship_velocity; ///< velocity of ship
        cv::Point2f _max_velocity{ 500, 500 }; ///< max velocity of ship
        float _ship_speed; ///< speed of ship, used to get unit vector of velocity
        cv::Point2f _ship_position; ///< position of ship
        int _ship_radius = 10; ///< radius of ship
        bool _ship_hit = false; ///< status if ship has been hit
        cv::Scalar _ship_colour = WHITE; ///< colour of ship

        std::chrono::steady_clock::time_point _last_time_hit; ///< last time ship was hit
        float _invunerability_time = 3.0f; ///< let ship be temporarily invunerable after being hit

        cv::Point2f _joystick_percent{ JOYSTICK_X_CENTER, JOYSTICK_Y_CENTER }; ///< Percentage of joystick deflection
        cv::Point2f _incrementer{ 0.0, 0.0 }; ///< Increments or decrements the draw position
        cv::Point2f _joystick_movement; ///< Direction and strength that the joystick is being pushed

        cv::Point2f _thrust_scale{ SLOW_ACCEL_SCALE, SLOW_ACCEL_SCALE }; ///< scalar to accelerate ship

        int _points = 0; ///< points of player

        bool _reset = false; ///< reset game
        bool _game_over = false; ///< game over

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

