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


void CSketch::update()
   {
    _reset = _control.get_button(BUTTON1);

    _joystick_position = cv::Point(JOYSTICK_X_SCALER*gpio(ANALOG, JOYSTICK_X),
                                   JOYSTICK_Y_SCALER*gpio(ANALOG, JOYSTICK_Y));

    // keep the joystick position within the bounds of the canvas
    if (_joystick_position.x < 0)
    {
        _joystick_position.x = 0;
    }
    else if (_joystick_position.x > _canvas.cols - 5)
    {
        _joystick_position.x = _canvas.cols - 5;
    }
    else if (_joystick_position.y < 0)
    {
        _joystick_position.y = 0;
    }
    else if (_joystick_position.y > _canvas.rows - 5)
    {
        _joystick_position.y = _canvas.rows - 5;
    }

    // translate the joystick position to an area to be coloured
	_position_to_colour = cv::Rect(_joystick_position, cv::Size(5, 5));

	// change colour if button 2 is pressed
	if (_control.get_button(BUTTON2))
    {
        _colour_index = (_colour_index + 1) % AMOUNT_OF_COLOURS;
    }
   }


bool CSketch::draw()
   {
    if (_reset)
    {
        _canvas.setTo(cv::Scalar(0, 0, 0));
    }

    // window
    cv::Point gui_position(10,10);
    cvui::window(_canvas, gui_position.x, gui_position.y, 150, 100, "Etch-A-Sketch");

    // quit button
    gui_position += cv::Point(5, 25);
    if (cvui::button(_canvas, gui_position.x, gui_position.y, 50, 20, "Quit"))
        {
        return false;
        }

    // clear button
    gui_position += cv::Point(0, 25);
    if (cvui::button(_canvas, gui_position.x, gui_position.y, 50, 25, "Clear"))
        {
        _canvas.setTo(cv::Scalar(0, 0, 0));
        }

    // colour display
	gui_position += cv::Point(60, -5);
	cv::putText(_canvas, "Colour: ", gui_position, cv::FONT_HERSHEY_PLAIN, 1.0, cv::Scalar(255, 255, 255), 1);

	_canvas(_position_to_colour).setTo(_colours[_colour_index]);

    cv::imshow(CANVAS_NAME, _canvas);

    return true;
   }


