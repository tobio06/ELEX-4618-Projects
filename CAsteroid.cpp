#include "stdafx.h"
#include "CAsteroid.h"

CAsteroid::CAsteroid()
{
	srand(time(NULL));

    _acceleration = cv::Point2f(0.0, 0.0);

	_radius = ((rand() % 50) + 20);

    _fast_speed = rand() % 200 + 50;
    _slow_speed = rand() % 10 + 50;

	_spawn_side = rand() % 4;

    if (rand() % 2)
        _random_direction = -1;
    else
        _random_direction = 1;

    switch (_spawn_side)
    {
    case 0: // top of screen
        _position = cv::Point2f(rand() % BOARD_SIZE.width, 0);
        _velocity = cv::Point2f(_random_direction * _slow_speed, _fast_speed);
        break;

    case 1: // bottom of screen
        _position = cv::Point2f(rand() % BOARD_SIZE.width, BOARD_SIZE.height);
        _velocity = cv::Point2f(_random_direction * _slow_speed, -_fast_speed);
        break;

    case 2: // left side of screen
        _position = cv::Point2f(0, rand() % BOARD_SIZE.height);
        _velocity = cv::Point2f(_fast_speed, _random_direction * _slow_speed);
        break;

    case 3: // right side of screen
        _position = cv::Point2f(BOARD_SIZE.width, rand() % BOARD_SIZE.height);
        _velocity = cv::Point2f(-_fast_speed, _random_direction * _slow_speed);
        break;
    }
}

CAsteroid::~CAsteroid()
{

}
