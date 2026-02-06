#pragma once
#include "opencv2/core.hpp"
#include "Control.h"

class CBase4618
    {
    private:

      CControl _control;

      cv::Mat _canvas;

    public:

       CBase4618();

       ~CBase4618();

       void gpio();

       void update();

       void draw();

       void run();
    };

