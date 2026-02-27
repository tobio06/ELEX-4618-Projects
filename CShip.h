#pragma once
#include "CGameObject.h"

#define BOARD_SIZE cv::Size(1280, 720) ///< size of board
#define BOARD_CENTER cv::Point2f(BOARD_SIZE.width / 2, BOARD_SIZE.height / 2) ///< center of board

/**
*
* @brief Holds all code for the ship for the Asteroids game in ELEX 4618
*
* This class is meant to contain all common variables and functions needed to
* run the ship for the game Asteroids for ELEX 4618.
*
* @author Tobio Yeung
*
*/
class CShip : public CGameObject
    {
    protected:

    public:
        CShip();
        ~CShip();

    };

