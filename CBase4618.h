#pragma once
#include "opencv2/core.hpp"
#include "Control.h"

class CBase4618
    {
    protected:
      CControl _control;
      cv::Mat _canvas;

    public:
       CBase4618();
       ~CBase4618();

       virtual void gpio() = 0;
       virtual void update() = 0;
       virtual bool draw() = 0;

       void run();
    };

