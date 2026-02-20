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

   _frame_start = std::chrono::high_resolution_clock::now();

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

      _reset = false;
      }

   // reset every frame
   _canvas.setTo(cv::Scalar(0, 0, 0));

   //////////////////
   // GUI ELEMENTS
   // window
   cv::Point gui_position(10, 10);
   cvui::window(_canvas, gui_position.x, gui_position.y, 300, 200, "Pong");

   // fps display
   gui_position += cv::Point(5, 40);
   cv::putText(_canvas, "FPS:", gui_position, cv::FONT_HERSHEY_DUPLEX, 0.5, cv::Scalar(255, 255, 255), 1);
   gui_position += cv::Point(0, 20);
   cv::putText(_canvas, _fps_string, gui_position, cv::FONT_HERSHEY_DUPLEX, 0.5, cv::Scalar(255, 255, 255), 1);

   // ball size trackbar
   gui_position += cv::Point(0, 20);
   cvui::trackbar(_canvas, gui_position.x, gui_position.y, 140, &_ball_radius, 5.0, 100.0);

   ////////////
   // DRAWING
   cv::circle(_canvas, SCREEN_CENTER, _ball_radius, cv::Scalar(255, 255, 255), -1);


   cvui::update();

   cv::imshow(CANVAS_NAME, _canvas);

   ///////////////////////////////////////
   // FPS CALCULATION AND LOCK TO 30 FPS
   auto frame_end = std::chrono::high_resolution_clock::now();

   double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(frame_end - _frame_start).count();

   if (elapsed > 0)
      _fps = 1000.0 / elapsed;
   _fps_string = std::to_string(_fps);

   // Lock to 30 FPS
   int sleep_time = _30_FPS - elapsed;
   if (sleep_time > 0)
      {
      std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
      }

   return true;
   }
