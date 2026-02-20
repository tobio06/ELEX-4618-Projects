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

   // check if any score is 5 or above to end game
   if (_player_score >= 5 || _computer_score >= 5)
      {
      _game_over = true;
      }

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

   //////////////////
   // PADDLE CONTROL

   _paddle_position = PONG_WINDOW_SIZE.height - (_previous_paddle_position + _y_incrementer);
   _previous_paddle_position += _y_incrementer;

   _player_paddle = cv::Rect(_canvas.cols - PADDLE_WIDTH, _paddle_position - PADDLE_HEIGHT / 2, PADDLE_WIDTH, PADDLE_HEIGHT);
   _computer_paddle = cv::Rect(0, _ball_position.y - PADDLE_HEIGHT / 2, PADDLE_WIDTH, PADDLE_HEIGHT);

   // constrain player paddle to stay on the screen; loops around if it goes off one side
   if (_player_paddle.y < 0)
      {
      _player_paddle.y = 0;
      _previous_paddle_position = PADDLE_HEIGHT / 2;
      }
   else if (_player_paddle.y + _player_paddle.height > _canvas.rows)
      {
      _player_paddle.y = _canvas.rows - _player_paddle.height;
      _previous_paddle_position = _canvas.rows - PADDLE_HEIGHT / 2;
      }

   /////////////////////////////////////////////////
   // UPDATE BALL POSITION, VELOCITY, AND COLLISION
   double ball_velocity_magnitude = sqrt(pow(_ball_velocity.x, 2) + pow(_ball_velocity.y, 2));

   // scale ball velocity with speed while maintaining direction
   if (ball_velocity_magnitude != 0)
   _ball_velocity = cv::Point( (_ball_velocity.x / ball_velocity_magnitude) * _ball_speed, (_ball_velocity.y / ball_velocity_magnitude) * _ball_speed);

   // update ball position with velocity
   _ball_position += _ball_velocity * dt;

   // implement ball hitbox for collision detection with paddles
   _ball_hitbox = cv::Rect(_ball_position.x - _ball_radius, _ball_position.y - _ball_radius, _ball_radius * 2, _ball_radius * 2);

   // X wall register
   if (_ball_position.x - _ball_radius < 0)
      {
      _player_score++;
      _ball_position = SCREEN_CENTER;
      }
   else if (_ball_position.x + _ball_radius > _canvas.cols)
      {
      _computer_score++;
      _ball_position = SCREEN_CENTER;
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

   // computer paddle bounce
   if ( (_ball_hitbox & _computer_paddle).area() && _ball_velocity.x < 0 )
      {
      _ball_position.x = _computer_paddle.x + _computer_paddle.width + _ball_radius;
      _ball_velocity.x = -_ball_velocity.x;
      }

   // player paddle bounce
   if ( (_ball_hitbox & _player_paddle).area() && _ball_velocity.x > 0 )
      {
      _ball_position.x = _player_paddle.x - _ball_radius;
      _ball_velocity.x = -_ball_velocity.x;
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
   if (_reset == true)
      {
      _reset = false;
      _game_over = false;
      _ball_position = SCREEN_CENTER;
      _ball_speed = 250;
      _ball_velocity = cv::Point(_ball_speed, _ball_speed);
      _ball_radius = ORIGINAL_BALL_RADIUS;
      _player_score = 0;
      _computer_score = 0;
      }
   // reset every frame
   _canvas.setTo(cv::Scalar(0, 0, 0));

   //////////////////
   // GUI ELEMENTS
   // window
   cv::Point gui_position(50, 10);
   cvui::window(_canvas, gui_position.x, gui_position.y, 250, 250, " ");

   // fps display
   gui_position += cv::Point(10, 5);
   cvui::text(_canvas, gui_position.x, gui_position.y, "Pong (FPS: " + _fps_string + ")");

   // score display
   gui_position += cv::Point(0, 30);
   cvui::text(_canvas, gui_position.x, gui_position.y, "SCORE: " + std::to_string(_computer_score) + " - " + std::to_string(_player_score), 1);

   // ball size trackbar
   gui_position += cv::Point(90, 40);
   cvui::text(_canvas, gui_position.x, gui_position.y, "Radius");
   gui_position += cv::Point(-80, 5);
   cvui::trackbar(_canvas, gui_position.x, gui_position.y, 200, &_ball_radius, 5.0, 100.0 );

   // ball speed trackbar
   gui_position += cv::Point(80, 70);
   cvui::text(_canvas, gui_position.x, gui_position.y, "Speed");
   gui_position += cv::Point(-80, 5);
   cvui::trackbar(_canvas, gui_position.x, gui_position.y, 200, &_ball_speed, 100.0, 400.0);

   // quit button
   gui_position += cv::Point(25, 60);
   if (cvui::button(_canvas, gui_position.x, gui_position.y, 50, 25, "Quit"))
      return false;

   // reset button
   gui_position += cv::Point(100, 0);
   if (cvui::button(_canvas, gui_position.x, gui_position.y, 50, 25, "Reset"))
      {
      _game_over = false;
      _ball_position = SCREEN_CENTER;
      _ball_speed = 250;
      _ball_velocity = cv::Point(_ball_speed, _ball_speed);
      _ball_radius = ORIGINAL_BALL_RADIUS;
      _player_score = 0;
      _computer_score = 0;
      }

   // end screen
   if (_game_over)
      {
      cv::putText(_canvas, "GAME OVER", cv::Point(_canvas.cols / 2 - 350, 200), cv::FONT_HERSHEY_DUPLEX, 4, cv::Scalar(0, 0, 200), 11);
      _ball_position = SCREEN_CENTER;
      _ball_speed = 0;
      _ball_radius = ORIGINAL_BALL_RADIUS;
      _player_score = 0;
      _computer_score = 0;
      }

   ////////////
   // DRAWING
   // pong ball
   cv::circle(_canvas, _ball_position, _ball_radius, cv::Scalar(255, 255, 255), -1);

   // pong paddles
   cv::rectangle(_canvas, _player_paddle, cv::Scalar(255, 255, 255), -1);
   cv::rectangle(_canvas, _computer_paddle, cv::Scalar(255, 255, 255), -1);

   // divider line
   cv::line(_canvas, cv::Point(_canvas.cols / 2, 0), cv::Point(_canvas.cols / 2, _canvas.rows), cv::Scalar(255, 255, 255), 1);

   cvui::update();

   cv::imshow(CANVAS_NAME, _canvas);

   return true;
   }
