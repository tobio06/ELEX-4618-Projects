#include "stdafx.h"
#include "CPong.h"

#include "cvui.h"

CPong::CPong(cv::Size canvas_size, int comport)
   {
   _control.init_com(comport);

   cvui::init(CANVAS_NAME);

   _canvas = cv::Mat::zeros(canvas_size, CV_8UC3);

   _time_last_frame = cv::getTickCount();
   }

CPong::~CPong()
   {

   }


double CPong::gpio(int type, int channel)
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


bool CPong::update()
   {
   // get the time at the start of the frame for calculating elapsed time and FPS
   _time_start_frame = cv::getTickCount();

   float dt = (_time_start_frame - _time_last_frame) / cv::getTickFrequency();
   _time_last_frame = _time_start_frame;

   // control reset with button 1
   _reset = _control.get_button(BUTTON1);

   // get joystick position as a percentage
   _joystick_percent = gpio(ANALOG, JOYSTICK_Y);

   /////////////////////
   // JOYSTICK CONTROL
   if (_joystick_percent > JOYSTICK_Y_CENTER + JOYSTICK_DEADZONE)
      {
      if (_joystick_percent > SPEED_THRESHOLD)
         {
         _y_incrementer = _joystick_percent * FAST_SPEED_SCALE;
         }
      else
         _y_incrementer = _joystick_percent * SLOW_SPEED_SCALE;
      }
   else if (_joystick_percent < JOYSTICK_Y_CENTER - JOYSTICK_DEADZONE)
      {
      if (_joystick_percent < 100 - SPEED_THRESHOLD)
         {
         _y_incrementer = (_joystick_percent - 100) * FAST_SPEED_SCALE;
         }
      else
         _y_incrementer = (_joystick_percent - 100) * SLOW_SPEED_SCALE;
      }
   else
      _y_incrementer = 0;

   //_draw_position = cv::Point(JOYSTICK_X_SCALER * (_previous_x_draw_position + _x_incrementer) - 75,
   //   JOYSTICK_Y_SCALER * (100 - (_previous_y_draw_position + _y_incrementer)) - 180);

   _previous_y_draw_position += _y_incrementer;

   // keep the draw position within the bounds of the canvas
   while (_draw_position.x < 0 || _draw_position.x > _canvas.cols - 2 ||
      _draw_position.y < 0 || _draw_position.y > _canvas.rows - 2)
      {
      if (_draw_position.x < 0)
         {
         _draw_position.x = 0;
         }
      else if (_draw_position.x > _canvas.cols - 2)
         {
         _draw_position.x = _canvas.cols - 2;
         }
      else if (_draw_position.y < 0)
         {
         _draw_position.y = 0;
         }
      else if (_draw_position.y > _canvas.rows - 2)
         {
         _draw_position.y = _canvas.rows - 2;
         }
      }

   // translate the joystick position to an area to be coloured
   _position_to_colour = cv::Rect(_draw_position, cv::Size(2, 2));

   //////////////////////////////////////
   // UPDATE BALL POSITION AND VELOCITY
   double ball_velocity_magnitude = sqrt(pow(_ball_velocity.x, 2) + pow(_ball_velocity.y, 2));

   // scale ball velocity with speed while maintaining direction
   if (ball_velocity_magnitude != 0)
   _ball_velocity = cv::Point( (_ball_velocity.x / ball_velocity_magnitude) * _ball_speed, (_ball_velocity.y / ball_velocity_magnitude) * _ball_speed);

   _ball_position += _ball_velocity * dt;

   // X wall bounce
   if (_ball_position.x - _ball_radius < 0)
      {
      _ball_position.x = _ball_radius;
      _ball_velocity.x = -_ball_velocity.x;
      }
   else if (_ball_position.x + _ball_radius > _canvas.cols)
      {
      _ball_position.x = _canvas.cols - _ball_radius;
      _ball_velocity.x = -_ball_velocity.x;
      }

   // Y wall bounce
   if (_ball_position.y - _ball_radius < 0)
      {
      _ball_position.y = _ball_radius;
      _ball_velocity.y = -_ball_velocity.y;
      }
   else if (_ball_position.y + _ball_radius > _canvas.rows)
      {
      _ball_position.y = _canvas.rows - _ball_radius;
      _ball_velocity.y = -_ball_velocity.y;
      }

   ///////////////////////////////////////////
   // MEASURE TIME ELAPSED AND LOCK FPS AT 30

   if (dt < _30_FPS_DELAY)
      {
      int time_elapsed = (int)((cv::getTickCount() - _time_start_frame) / cv::getTickFrequency() * 1000);

      int delay_time = _30_FPS_DELAY - time_elapsed;
      if (delay_time > 0)
         {
         std::this_thread::sleep_for(std::chrono::milliseconds(delay_time));
         }

    dt = ( cv:: getTickCount() - _time_start_frame) / cv::getTickFrequency();
      }

   _fps = 1.0 / dt;
   _fps_string = std::to_string(_fps);

   return true;
   }


bool CPong::draw()
   {
   if (_reset)
      {

      _reset = false;
      }

   // reset every frame
   _canvas.setTo(cv::Scalar(0, 0, 0));

   //////////////////
   // GUI ELEMENTS
   // window
   cv::Point gui_position(10, 10);
   cvui::window(_canvas, gui_position.x, gui_position.y, 250, 250, " ");

   // fps display
   gui_position += cv::Point(10, 5);
   cvui::text(_canvas, gui_position.x, gui_position.y, "Pong (FPS: ");
   gui_position += cv::Point(80, 0);
   cvui::text(_canvas, gui_position.x, gui_position.y, _fps_string + ")");

   // ball size trackbar
   gui_position += cv::Point(10, 30);
   cvui::text(_canvas, gui_position.x, gui_position.y, "Radius");
   gui_position += cv::Point(-80, 5);
   cvui::trackbar(_canvas, gui_position.x, gui_position.y, 200, &_ball_radius, 5.0, 100.0 );

   // ball speed trackbar
   gui_position += cv::Point(80, 70);
   cvui::text(_canvas, gui_position.x, gui_position.y, "Speed");
   gui_position += cv::Point(-80, 5);
   cvui::trackbar(_canvas, gui_position.x, gui_position.y, 200, &_ball_speed, 100.0, 400.0);

   ////////////
   // DRAWING
   cv::circle(_canvas, _ball_position, _ball_radius, cv::Scalar(255, 255, 255), -1);


   cvui::update();

   cv::imshow(CANVAS_NAME, _canvas);



   return true;
   }
