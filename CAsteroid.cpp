#include "stdafx.h"
#include "CAsteroid.h"

CAsteroid::CAsteroid()
{
	srand(time(NULL));

	_radius = ((rand() % 50) + 10);
	_position = cv::Point2f(rand() % BOARD_SIZE.width, 0);
	_velocity = cv::Point2f(rand() % 100 + 50, rand() % 200 + 50);
}

CAsteroid::~CAsteroid()
{

}
