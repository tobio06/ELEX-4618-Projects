#include "stdafx.h"
#include "CAsteroidGame.h"

CAsteroidGame::CAsteroidGame(cv::Size size, int comport)
   {
    _last_asteroid_spawn = std::chrono::steady_clock::now();
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
    //ship.draw(_canvas);

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
        auto now = std::chrono::steady_clock::now();

        // time since last asteroid spawned
        float time_elapsed = std::chrono::duration<float>(now - _last_asteroid_spawn).count();

        // spawn every 2–4 seconds
        if (time_elapsed > 2.0f + static_cast<float>(rand()) / RAND_MAX * SPAWN_DELAY)
        {
            _asteroid_list.emplace_back(); // create asteroid
            _last_asteroid_spawn = now;    // reset timer
        }

        if (cv::waitKey(1) == 'q' || !update() || !draw())
        {
            break;
        }
    }
}