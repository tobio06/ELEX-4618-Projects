#include "stdafx.h"
#include "CAsteroidGame.h"

#include "cvui.h"

CAsteroidGame::CAsteroidGame(cv::Size canvas_size, int comport)
   {
    _control.init_com(comport);

    cvui::init(CANVAS_NAME);

    _canvas = cv::Mat::zeros(canvas_size, CV_8UC3);

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
    for (auto& a : _asteroid_list)
        a.move();

    /////////////////////
    // SHIP CONTROL
    _joystick_percent = cv::Point2f(gpio(ANALOG, JOYSTICK_X), gpio(ANALOG, JOYSTICK_Y));

    _joystick_movement.x = _joystick_percent.x - JOYSTICK_X_CENTER;
    _joystick_movement.y = JOYSTICK_Y_CENTER - _joystick_percent.y; // invert Y so up is positive

    // apply deadzone
    if (abs(_joystick_movement.x) < JOYSTICK_DEADZONE)
        _joystick_movement.x = 0;

    if (abs(_joystick_movement.y) < JOYSTICK_DEADZONE)
        _joystick_movement.y = 0;

    // determine speed scale
    if (_joystick_percent.x > SPEED_THRESHOLD ||
        _joystick_percent.x < (100 - SPEED_THRESHOLD))
        _speed_scale.x = FAST_SPEED_SCALE;

    if (_joystick_percent.y > SPEED_THRESHOLD ||
        _joystick_percent.y < (100 - SPEED_THRESHOLD))
        _speed_scale.y = FAST_SPEED_SCALE;

    // calculate incrementer in pixel space
    _incrementer.x = _joystick_movement.x * _speed_scale.x;
    _incrementer.y = _joystick_movement.y * _speed_scale.y;

    // update draw position smoothly (float space)
    _draw_position = _previous_draw_position + _incrementer;

    // boundary clamping (accounting for 2x2 draw size)
    if (_draw_position.x < 0)
        _draw_position.x = 0;
    if (_draw_position.x > _canvas.cols - 2)
        _draw_position.x = _canvas.cols - 2;

    if (_draw_position.y < 0)
        _draw_position.y = 0;
    if (_draw_position.y > _canvas.rows - 2)
        _draw_position.y = _canvas.rows - 2;

    // store for next frame
    _previous_draw_position = _draw_position;


    return true;
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

    //// Draw missiles
    //for (auto& m : _missile_list)
    //    m.draw(_canvas);

    cvui::update();

    cv::imshow(CANVAS_NAME, _canvas);

    return true;
    }

void CAsteroidGame::run()
{
    while (true)
    {
        auto now = std::chrono::steady_clock::now();

        // time since last asteroid spawned
        float time_elapsed = std::chrono::duration<float>(now - _last_asteroid_spawn).count();

        // spawn every 1-3 seconds
        if (time_elapsed > 1.0f + static_cast<float>(rand()) / RAND_MAX * SPAWN_DELAY)
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