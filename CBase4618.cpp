#include "stdafx.h"
#include "CBase4618.h"
#include <conio.h>

#include "cvui.h"

CBase4618::CBase4618() { }

CBase4618::~CBase4618() { }

void CBase4618::run()
   {
      while (!(_kbhit() && (_getch() == 'q' || _getch() == 'Q') ))
         { 
         
            gpio();
            update();
            draw();
            
            if (!draw())
               break;
         }
   }