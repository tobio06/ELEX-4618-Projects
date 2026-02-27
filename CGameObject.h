#pragma once

#include "opencv2/core.hpp"

/**
*
* @brief Holds common code for objects 
*
* This class is meant to contain all common variables and functions needed to
* create objects for ELEX 4618.
*
* @author Tobio Yeung
*
*/
class CGameObject 
   {
   protected:
      cv::Point2f _position; ///< Position of object
      cv::Point2f _velocity; ///< Velocity of object
      cv::Point2f _acceleration; ///< Acceleration of object

      int _radius; ///< Radius of object

      int _lives; ///< Lives of object

      std::chrono::steady_clock::time_point _last_frame_time = std::chrono::steady_clock::now(); ///< Time point of the last frame

   public:
       /** @brief Moves the object
       *
       * @return Returns Nothing
       */
      void move();

      /** @brief Checks for collision with other objects
       *
       * @param obj The object to check if it collided with
       *
       * @return True or false
       */
      bool collide(CGameObject& obj);

      /** @brief Checks for collision with the screen borders
      *
      * @param board The size of the screen canvas
      *
      * @return True or false
      */
      bool collide_wall(cv::Size board);

      void hit();

      /** @brief Gets lives of object
      *
      * @return Number of lives
      */
      int get_lives() { return _lives; }
      /** @brief Sets lives of object
      *
      * @param lives Number of lives
      * 
      * @return Returns nothing
      */
      void set_lives(int lives) { _lives = lives; }

      /** @brief Sets position of object
      *
      * @param pos Position of object
      *
      * @return Returns nothing
      */
      void set_pos(cv::Point2f pos) { _position = pos; }
      /** @brief Gets position of object
      *
      * @return Position of object
      */
      cv::Point2f get_pos() { return _position; }

      /** @brief Sets velocity of object
      *
      * @param vel Velocity of object
      *
      * @return Returns nothing
      */
      void set_vel(cv::Point2f vel) { _velocity = vel; }
      /** @brief Gets velocity of object
      *
      * @return Velocity of object
      */
      cv::Point2f get_vel() { return _velocity; }

      /** @brief Sets acceleration of object
      *
      * @param pos Acceleration of object
      *
      * @return Returns nothing
      */
      void set_accel(cv::Point2f accel) { _acceleration = accel; }
      /** @brief Gets acceleration of object
      *
      * @return Acceleration of object
      */
      cv::Point2f get_accel() { return _acceleration; }

      /** @brief Draws object
      *
      * @param im Canvas to draw object on 
      * 
      * @return Returns nothing
      */
      void draw(cv::Mat& im, cv::Scalar colour);
   };

