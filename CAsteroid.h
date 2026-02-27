#pragma once
#include "CGameObject.h"
#include <stdlib.h>
#include <time.h>

#define BOARD_SIZE cv::Size(1280, 720) ///< size of board

/**
*
* @brief Holds all code for the asteroid for the Asteroids game in ELEX 4618
*
* This class is meant to contain all common variables and functions needed to
* run the asteroids for the game Asteroids for ELEX 4618.
*
* @author Tobio Yeung
*
*/
class CAsteroid : public CGameObject
    {
    private:
        int _spawn_side; ///< side for asteroids to spawn on
        float _fast_speed; ///< fast speed of asteroids
        float _slow_speed; ///< slow speed of asteroids
        int _random_direction = 1; ///< randomize direction of asteroids

    public:
        CAsteroid();
        ~CAsteroid();

    };

