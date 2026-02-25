#pragma once
#include "CGameObject.h"
#include <stdlib.h>
#include <time.h>

#define BOARD_SIZE cv::Size(1280, 720)

class CAsteroid : public CGameObject
    {
    private:
        int _spawn_side;
        float _fast_speed;
        float _slow_speed;
        int _random_direction = 1;

    public:
        CAsteroid();
        ~CAsteroid();

    };

