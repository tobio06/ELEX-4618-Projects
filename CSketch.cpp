#include "stdafx.h"
#include "CSketch.h"

#include "cvui.h"

#define CANVAS_NAME "Etch-A-Sketch"

CSketch::CSketch() { };

CSketch::CSketch(cv::Size canvas_size, int comport)
   {
      _control.init_com(comport);

      cvui::init(CANVAS_NAME);

      _canvas = cv::Mat(canvas_size, CV_8UC3);
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

   }


bool CSketch::draw()
   {

   _canvas.setTo(cv::Scalar(255, 255, 255));

   //if (cvui::button(_canvas, 80, 80, "Clear"))
   //   {
   //   _canvas.setTo(cv::Scalar(255, 255, 255));
   //   }

   if (cvui::button(_canvas, 10, 10, "Quit"))
      {
      return false;
      }

   cv::imshow(CANVAS_NAME, _canvas);

   return true;
   }


