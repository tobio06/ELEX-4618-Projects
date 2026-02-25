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
   _position += _velocity * dt;
   }

bool CGameObject::collide(CGameObject& obj)
   {
    cv::Point2f distance_component = (_position.x - obj._position.x, _position.y - obj._position.y);

    float distance = sqrt(pow(distance_component.x, 2) + pow(distance_component.y, 2));

    return (distance <= (_radius + obj._radius));
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

void CGameObject::draw(cv::Mat& im)
   {
   cv::circle(im, _position, _radius, cv::Scalar(255, 255, 255), -1);
   }
   