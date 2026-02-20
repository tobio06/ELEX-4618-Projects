#include "stdafx.h"
#include "CSketch.h"

#include "cvui.h"

#define CANVAS_NAME "Etch-A-Sketch"

CSketch::CSketch(cv::Size canvas_size, int comport)
   {
      _control.init_com(comport);

      cvui::init(CANVAS_NAME);

      _canvas = cv::Mat::zeros(canvas_size, CV_8UC3);
   }

CSketch::~CSketch() 
   {
       _control.set_data(DIGITAL, RED_LED, 0);
       _control.set_data(DIGITAL, GREEN_LED, 0);
       _control.set_data(DIGITAL, BLUE_LED, 0);
   }


double CSketch::gpio(int type, int channel)
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


bool CSketch::update()
   {
    _reset = _control.get_button(BUTTON1);

    _joystick_percent = cv::Point(gpio(ANALOG, JOYSTICK_X), gpio(ANALOG, JOYSTICK_Y));

    /////////////////////
    // JOYSTICK CONTROL
    // X AXIS
    if (_joystick_percent.x > JOYSTICK_X_CENTER + JOYSTICK_DEADZONE)
       {
       if (_joystick_percent.x > SPEED_THRESHOLD)
          {
          _incrementer.x = _joystick_percent.x * FAST_SPEED_SCALE;
          } 
       else
       _incrementer.x = _joystick_percent.x * SLOW_SPEED_SCALE;
       }
    else if (_joystick_percent.x < JOYSTICK_X_CENTER - JOYSTICK_DEADZONE)
       {
       if (_joystick_percent.x < 100 - SPEED_THRESHOLD)
          {
          _incrementer.x = (_joystick_percent.x - 100) * FAST_SPEED_SCALE;
          }
       else
       _incrementer.x = ( _joystick_percent.x - 100) * SLOW_SPEED_SCALE;
       }
    else 
       _incrementer.x = 0;

    // Y AXIS
    if (_joystick_percent.y > JOYSTICK_Y_CENTER + JOYSTICK_DEADZONE)
       {
       if (_joystick_percent.y > SPEED_THRESHOLD)
          {
          _incrementer.y = _joystick_percent.y * FAST_SPEED_SCALE;
          }
       else
          _incrementer.y = _joystick_percent.y * SLOW_SPEED_SCALE;
       }
    else if (_joystick_percent.y < JOYSTICK_Y_CENTER - JOYSTICK_DEADZONE)
       {
       if (_joystick_percent.y < 100 - SPEED_THRESHOLD)
          {
          _incrementer.y = (_joystick_percent.y - 100) * FAST_SPEED_SCALE;
          }
       else
          _incrementer.y = (_joystick_percent.y - 100) * SLOW_SPEED_SCALE;
       }
    else
       _incrementer.y = 0;
      
    _draw_position = cv::Point(JOYSTICK_X_SCALER * (_previous_draw_position.x + _incrementer.x) - 75,
                                   JOYSTICK_Y_SCALER * (100 - (_previous_draw_position.y + _incrementer.y)) - 180);

    _previous_draw_position.x += _incrementer.x;
    _previous_draw_position.y += _incrementer.y;

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

	 // change colour if button 2 is pressed
    if (_control.get_button(BUTTON2))
       _colour_index = (_colour_index + 1) % AMOUNT_OF_COLOURS;
       
    //////////////////
    // GUI elements
    // window
    cv::Point gui_position(10, 10);
    cvui::window(_canvas, gui_position.x, gui_position.y, 150, 100, "Etch-A-Sketch");

    // quit button
    gui_position += cv::Point(5, 25);
    if (cvui::button(_canvas, gui_position.x, gui_position.y, 50, 20, "Quit"))
       return false;

    // clear button
    gui_position += cv::Point(0, 25);
    if (cvui::button(_canvas, gui_position.x, gui_position.y, 50, 25, "Clear"))
       _reset = true;

    // colour display
    gui_position += cv::Point(65, -10);
    cv::putText(_canvas, "Colour:", gui_position, cv::FONT_HERSHEY_DUPLEX, 0.5, _colours[_colour_index], 1);

    gui_position += cv::Point(0, 20);
    cv::putText(_canvas, _colour_names[_colour_index], gui_position, cv::FONT_HERSHEY_DUPLEX, 0.5, _colours[_colour_index], 1);

    _control.set_data(DIGITAL, RED_LED, (_colour_index == 0 || _colour_index == 3 || _colour_index == 4) );
    _control.set_data(DIGITAL, GREEN_LED, (_colour_index == 1 || _colour_index == 3 || _colour_index == 4) );
    _control.set_data(DIGITAL, BLUE_LED, (_colour_index == 2 || _colour_index == 4 ) );

    ////////////////////
    // shake to reset
    _control.get_data(ANALOG, ACCELEROMETER_Y, _accel_y);
    _control.get_data(ANALOG, ACCELEROMETER_Z, _accel_z);

    if (!_shake_reset_triggered && (_accel_y - _previous_accel_y > ACCEL_SHAKE_THRESHOLD || _accel_z - _previous_accel_z > ACCEL_SHAKE_THRESHOLD))
       {
       _reset = true;
       _shake_reset_triggered = true;  
       }
    else if (_shake_reset_triggered) // wait for shake to end before allowing another shake to reset
       {
       if ((_accel_y - _previous_accel_y < ACCEL_SHAKE_THRESHOLD && _accel_z - _previous_accel_z < ACCEL_SHAKE_THRESHOLD))
         _shake_reset_triggered = false; 
       }
    else
       {
       _previous_accel_y = _accel_y;
       _previous_accel_z = _accel_z;
       }

    return true;
   }


bool CSketch::draw()
{
   if (_reset)
      {
      _canvas.setTo(cv::Scalar(0, 0, 0));
      _reset = false;
      }

   // drawing
   _canvas(_position_to_colour).setTo(_colours[_colour_index]);

   // create smooth lines
   if (_smoothed)
      {
      cv::line(_canvas, _previous_draw_position2, _draw_position, _colours[_colour_index], 2);
      _previous_draw_position2 = _draw_position;
      }
   else
      {
      _previous_draw_position2 = _draw_position;
      _smoothed = true;
      }

   cvui::update();

   cv::imshow(CANVAS_NAME, _canvas);

   return true;
   }

