#include "stdafx.h"
#include "CAsteroidGame.h"

CAsteroidGame::CAsteroidGame(cv::Size size, int comport)
   {

   }

CAsteroidGame::~CAsteroidGame()
   {

   }

double CAsteroidGame::gpio(int type, int channel)
    {
        if (type == ANALOG)
        {
            int temp_val = 0;

            double joystick_percent = _control.get_analog(type, channel, temp_val);

            return joystick_percent;
        }
        else if (type == DIGITAL)
        {
            int button_result = 0;

            _control.get_data(type, channel, button_result);

            return button_result;
        }
        else
            return 0;
    }

bool CAsteroidGame::update()
    {

    }

bool CAsteroidGame::draw()
    {
    // reset every frame
    _canvas.setTo(cv::Scalar(0, 0, 0));

    // Draw ship
    ship.draw(_canvas);

    // Draw asteroids
    for (auto& a : _asteroid_list)
        a.draw(_canvas);

    // Draw missiles
    for (auto& m : _missile_list)
        m.draw(_canvas);
    }

void CAsteroidGame::run()
{
    while (true)
    {
        // randomly create asteroid
        _asteroid_list.emplace_back();

        if (cv::waitKey(1) == 'q' || !update() || !draw())
        {
            break;
        }
    }
}