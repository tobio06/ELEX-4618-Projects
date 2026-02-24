#include "stdafx.h"
#include "CAsteroidGame.h"

CAsteroidGame::CAsteroidGame(cv::Size size, int comport)
   {

   }

CAsteroidGame::~CAsteroidGame()
   {

   }

double CAsteroidGame::gpio(int type, int channel)
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