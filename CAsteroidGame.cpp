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
    // move asteroids
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
        _thrust_scale.x = FAST_ACCEL_SCALE;
    else
        _thrust_scale.x = SLOW_ACCEL_SCALE;

    if (_joystick_percent.y > SPEED_THRESHOLD ||
        _joystick_percent.y < (100 - SPEED_THRESHOLD))
        _thrust_scale.y = FAST_ACCEL_SCALE;
    else
        _thrust_scale.x = SLOW_ACCEL_SCALE;

    // calculate incrementer in pixel space
    _incrementer.x = _joystick_movement.x * _thrust_scale.x;
    _incrementer.y = _joystick_movement.y * _thrust_scale.y;

    // update ship acceleration
    ship.set_accel(_incrementer);

    // move ship
    ship.move();

    // set max velocity for ship
    _ship_velocity = ship.get_vel();
    if (abs(_ship_velocity.x) > _max_velocity.x)
        if (_ship_velocity.x > 0)
            ship.set_vel(cv::Point2f(_max_velocity.x, _ship_velocity.y));
        else if(_ship_velocity.x < 0)
            ship.set_vel(cv::Point2f(-_max_velocity.x, _ship_velocity.y));
    if (abs(_ship_velocity.y) > _max_velocity.y)
        if (_ship_velocity.y > 0)
            ship.set_vel(cv::Point2f(_ship_velocity.x, _max_velocity.y));
        else if (_ship_velocity.y < 0)
            ship.set_vel(cv::Point2f(_ship_velocity.x, -_max_velocity.y));

    // let ship loop through screen
    _ship_position = ship.get_pos();
    if (_ship_position.y - _ship_radius < 0)
    {
        ship.set_pos(cv::Point2f(BOARD_SIZE.width - _ship_position.x, BOARD_SIZE.height - _ship_radius));
    }
    if (_ship_position.y + _ship_radius > BOARD_SIZE.height)
    {
        ship.set_pos(cv::Point2f(BOARD_SIZE.width - _ship_position.x, _ship_radius));
    }
    if (_ship_position.x - _ship_radius < 0)
    {
        ship.set_pos(cv::Point2f(BOARD_SIZE.width - _ship_radius, BOARD_SIZE.height - _ship_position.y));
    }
    if (_ship_position.x + _ship_radius > BOARD_SIZE.width)
    {
        ship.set_pos(cv::Point2f(_ship_radius, BOARD_SIZE.height - _ship_position.y));
    }

    _ship_speed = sqrt(pow(_ship_velocity.x, 2) + pow(_ship_velocity.y, 2));
    // create missiles 
    if (_control.get_button(BUTTON2))
    {
        _missile_list.emplace_back();
        _missile_list.at(_missile_list.size() - 1).set_vel(cv::Point2f(_ship_velocity.x / _ship_speed * _missile_speed,
                                                                       _ship_velocity.y / _ship_speed * _missile_speed));
        _missile_list.at(_missile_list.size() - 1).set_pos(_ship_position);
    }

    // move missiles
    for (auto& m : _missile_list)
        m.move();

    return true;
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

    //// Draw missiles
    for (auto& m : _missile_list)
        m.draw(_canvas);

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