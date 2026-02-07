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


void CSketch::gpio()
   {
   int x_val = 0;
   int y_val = 0;
   double x_percent = _control.get_analog(ANALOG, JOYSTICK_X, x_val);
   double y_percent = _control.get_analog(ANALOG, JOYSTICK_X, y_val);
   }


void CSketch::update()
   {
    _reset = _control.get_button(BUTTON1);
    if (_reset)
       {
          _canvas.setTo(cv::Scalar(0, 0, 0));
	}
   }


bool CSketch::draw()
   {
   cv::Point gui_position(10,10);
   cvui::window(_canvas, gui_position.x, gui_position.y, 300, 200, "Etch-A-Sketch");

   gui_position += cv::Point(5, 25);
   if (cvui::button(_canvas, gui_position.x, gui_position.y, 100, 50, "Quit"))
      {
      return false;
      }

   cv::imshow(CANVAS_NAME, _canvas);

   return true;
   }


