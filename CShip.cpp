#include "stdafx.h"
#include "CShip.h"

CShip::CShip()
{
	_radius = 10;
	_lives = 10;
	_position = (BOARD_CENTER);
	_velocity = cv::Point2f(0.0, 0.0);
	_acceleration = cv::Point2f(0.0, 0.0);

}

CShip::~CShip()
{

}
