#include "stdafx.h"
#include "CPong.h"

#include "cvui.h"

CPong::CPong(cv::Size canvas_size, int comport) 
   {
   _control.init_com(comport);

   cvui::init(CANVAS_NAME);

   _canvas = cv::Mat::zeros(canvas_size, CV_8UC3);
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
   _reset = _control.get_button(BUTTON1);

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

   return true;
   }


bool CPong::draw()
   {
   if (_reset)
      {
      _canvas.setTo(cv::Scalar(0, 0, 0));
      _reset = false;
      }

   cv::circle(_canvas, SCREEN_CENTER, ORIGINAL_BALL_RADIUS, cv::Scalar(255, 255, 255), -1);

   cvui::update();

   cv::imshow(CANVAS_NAME, _canvas);

   return true;
   }
