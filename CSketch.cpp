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

    _joystick_x_percent = gpio(ANALOG, JOYSTICK_X);
    _joystick_y_percent = gpio(ANALOG, JOYSTICK_Y);

    if (_joystick_x_percent > JOYSTICK_X_CENTER + JOYSTICK_DEADZONE)
       if (_joystick_x_previous_percent > _joystick_x_percent)
          _joystick_position = cv::Point(JOYSTICK_X_SCALER * _joystick_x_previous_percent - 75,
                                         JOYSTICK_Y_SCALER * (100 - _joystick_y_previous_percent) - 180);
       else
          {
          _position_x_incrementer++;
          _joystick_position = cv::Point(JOYSTICK_X_SCALER * (_joystick_x_previous_percent + _position_x_incrementer) - 75,
                                         JOYSTICK_Y_SCALER * (100 - (_joystick_y_previous_percent + _position_y_incrementer)) - 180);
          }
    else if (_joystick_x_percent < JOYSTICK_X_CENTER - JOYSTICK_DEADZONE)
       if (_joystick_x_previous_percent < _joystick_x_percent)
          _joystick_position = cv::Point(JOYSTICK_X_SCALER * _joystick_x_previous_percent - 75,
                                         JOYSTICK_Y_SCALER * (100 - _joystick_y_previous_percent) - 180);
       else
          {
          _position_x_incrementer--;
          _joystick_position = cv::Point(JOYSTICK_X_SCALER * (_joystick_x_previous_percent + _position_x_incrementer) - 75,
                                          JOYSTICK_Y_SCALER * (100 - (_joystick_y_previous_percent + _position_y_incrementer)) - 180);
          }
   
       _position_x_incrementer = 0;
       _joystick_x_previous_percent = _joystick_x_percent;
    

    if (_joystick_y_percent > JOYSTICK_Y_CENTER + JOYSTICK_DEADZONE)
       if (_joystick_y_previous_percent > _joystick_y_percent)
          _joystick_position = cv::Point(JOYSTICK_X_SCALER * _joystick_x_previous_percent - 75,
                                         JOYSTICK_Y_SCALER * (100 - _joystick_y_previous_percent) - 180);
       else
          {
          _position_y_incrementer++;
          _joystick_position = cv::Point(JOYSTICK_X_SCALER * (_joystick_x_previous_percent + _position_x_incrementer) - 75,
                                         JOYSTICK_Y_SCALER * (100 - (_joystick_y_previous_percent + _position_y_incrementer)) - 180);
          }
    else if (_joystick_y_percent < JOYSTICK_Y_CENTER - JOYSTICK_DEADZONE)
       if (_joystick_y_previous_percent < _joystick_y_percent)
          _joystick_position = cv::Point(JOYSTICK_X_SCALER * _joystick_x_previous_percent - 75,
                                         JOYSTICK_Y_SCALER * (100 - _joystick_y_previous_percent) - 180);
      else
          {
          _position_y_incrementer--;
          _joystick_position = cv::Point(JOYSTICK_X_SCALER * (_joystick_x_previous_percent + _position_x_incrementer) - 75,
                                         JOYSTICK_Y_SCALER * (100 - (_joystick_y_previous_percent + _position_y_incrementer)) - 180);
          } 
    
       _position_y_incrementer = 0;
       _joystick_y_previous_percent = _joystick_y_percent;
    


    // keep the joystick position within the bounds of the canvas
    while (_joystick_position.x < 0 || _joystick_position.x > _canvas.cols - 2 ||
           _joystick_position.y < 0 || _joystick_position.y > _canvas.rows - 2)
    {
        if (_joystick_position.x < 0)
        {
            _joystick_position.x = 0;
        }
        else if (_joystick_position.x > _canvas.cols - 2)
        {
            _joystick_position.x = _canvas.cols - 2;
        }
        else if (_joystick_position.y < 0)
        {
            _joystick_position.y = 0;
        }
        else if (_joystick_position.y > _canvas.rows - 2)
        {
            _joystick_position.y = _canvas.rows - 2;
        }
    }

    // translate the joystick position to an area to be coloured
	 _position_to_colour = cv::Rect(_joystick_position, cv::Size(2, 2));

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

    _control.set_data(DIGITAL, RED_LED, _colour_index == 0);
    _control.set_data(DIGITAL, GREEN_LED, _colour_index == 1);
    _control.set_data(DIGITAL, BLUE_LED, _colour_index == 2);

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
      cv::line(_canvas, _previous_joystick_position, _joystick_position, _colours[_colour_index], 2);
      _previous_joystick_position = _joystick_position;
      }
   else
      {
      _previous_joystick_position = _joystick_position;
      _smoothed = true;
      }

   cv::imshow(CANVAS_NAME, _canvas);

   return true;
   }

