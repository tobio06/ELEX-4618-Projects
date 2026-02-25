#include "stdafx.h"
#include "CGameObject.h"

void CGameObject::move()
   {
   /////////////////////////
   // MEASURE TIME ELAPSED 
   auto time_now = std::chrono::steady_clock::now();

   std::chrono::duration<double> elapsed_time = time_now - _last_frame_time;

   double dt = elapsed_time.count();

   _last_frame_time = time_now;

   ////////////////////
   // MOVE THE OBJECT
   double velocity_magnitude = sqrt(pow(_velocity.x, 2) + pow(_velocity.y, 2));

   // scale velocity with speed while maintaining direction
   if (velocity_magnitude != 0)
      _velocity = cv::Point((_velocity.x / velocity_magnitude) * _speed, (_velocity.y / velocity_magnitude) * _speed);

   // update position with velocity
   _position += _velocity * dt;
   }

bool CGameObject::collide(CGameObject& obj)
   {
   if (abs(_position.x - obj._position.x) < (_radius + obj._radius) ||
      abs(_position.y - obj._position.y) < (_radius + obj._radius))
      {
      return true;
      }
   }

bool CGameObject::collide_wall(cv::Size board)
   {
   // X wall 
   if (_position.x - _radius < 0)
      {
      return true;
      }
   else if (_position.x + _radius > board.width)
      {
      return true;
      }

   // Y wall 
   if (_position.y - _radius < 0)
      {
      return true;
      }
   else if (_position.y + _radius > board.height)
      {
      return true;
      }
   }

void CGameObject::hit()
   {

   }

int CGameObject::get_lives()
   {
   return _lives;
   }

void CGameObject::set_lives(int lives)
   {
    _lives = lives;
   }

void CGameObject::set_pos(cv::Point2f pos)
   {
    _position = pos;
   }

cv::Point2f CGameObject::get_pos()
   {
   return _position;
   }

void CGameObject::draw(cv::Mat& im)
   {
   cv::circle(im, _position, _radius, cv::Scalar(255, 255, 255), -1);
   }
   