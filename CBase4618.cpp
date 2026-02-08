#include "stdafx.h"
#include "CBase4618.h"
#include <conio.h>

#include "cvui.h"

#define CANVAS_NAME "Etch-A-Sketch"

CBase4618::CBase4618() { }
CBase4618::~CBase4618() { }

void CBase4618::run()
   {
   while (true)
      {
         if (cv::waitKey(1) == 'q' || !update() || !draw())
            {
            break;
            }
      }
   }