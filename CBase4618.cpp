#include "stdafx.h"
#include "CBase4618.h"
#include <conio.h>

#include "cvui.h"

#define CANVAS_NAME "Etch-A-Sketch"

CBase4618::CBase4618() { }

CBase4618::~CBase4618() { }

void CBase4618::run()
   {
   cvui::init(CANVAS_NAME);

   _canvas = cv::Mat(500, 500, CV_8UC3);

   while (true)
      {
         update();
         draw();

         if (cv::waitKey(1) == 'q' || !draw())
            {
            break;
            }
      }

   }