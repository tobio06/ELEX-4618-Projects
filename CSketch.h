#pragma 

#include "opencv2/core.hpp"
#include "CBase4618.h"

class CSketch : public CBase4618
    {
    private:

    public:

       CSketch();

       CSketch(cv::Size size, int comport);

       void CBase4618::gpio() override;
       void CBase4618::update() override;
       bool CBase4618::draw() override;

    };

