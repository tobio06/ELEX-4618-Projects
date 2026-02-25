#pragma once
#include "CGameObject.h"

#define BOARD_SIZE cv::Size(1280, 720)
#define BOARD_CENTER cv::Point2f(BOARD_SIZE.width / 2, BOARD_SIZE.height / 2)

class CShip : public CGameObject
    {
    protected:

    public:
        CShip();
        ~CShip();

    };

