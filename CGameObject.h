#pragma once

#include "opencv2/core.hpp"

class CGameObject 
   {
   protected:
      cv::Point2f _position;
      cv::Point2f _velocity;
      cv::Point2f _acceleration;

      int _radius;

      int _lives;

      std::chrono::steady_clock::time_point _last_frame_time = std::chrono::steady_clock::now(); ///< Time point of the last frame

   public:
      void move();

      bool collide(CGameObject& obj);

      bool collide_wall(cv::Size board);

      void hit();

      int get_lives() { return _lives; }
      void set_lives(int lives) { _lives = lives; }

      void set_pos(cv::Point2f pos) { _position = pos; }
      cv::Point2f get_pos() { return _position; }

      void set_vel(cv::Point2f vel) { _velocity = vel; }
      cv::Point2f get_vel() { return _velocity; }

      void set_accel(cv::Point2f accel) { _acceleration = accel; }
      cv::Point2f get_accel() { return _acceleration; }

      void draw(cv::Mat& im);
   };

