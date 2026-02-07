#pragma 

#include "opencv2/core.hpp"
#include "CBase4618.h"

#define RED cv::Scalar(0, 0, 255)
#define GREEN cv::Scalar(0, 255, 0)
#define BLUE cv::Scalar(255, 0, 0)
#define AMOUNT_OF_COLOURS 3

#define WINDOW_SIZE cv::Size(1000, 750)
#define JOYSTICK_X_SCALER 12.5
#define JOYSTICK_Y_SCALER 12.5

class CSketch : public CBase4618
    {
    private:
        int _reset = 0;
		cv::Point _joystick_position;
        cv::Scalar _colours[3] = { RED, GREEN, BLUE };
		int _colour_index = 0;
        cv::Rect _position_to_colour;
		cv::Point _previous_joystick_position;
        bool smoothed = false;

    public:
       CSketch(cv::Size size, int comport);

       double CBase4618::gpio(int type, int channel) override;
       void CBase4618::update() override;
       bool CBase4618::draw() override;

    };

