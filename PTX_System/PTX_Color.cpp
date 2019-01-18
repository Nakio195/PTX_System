#include "PTX_Color.h"


PTX_Color::PTX_Color(unsigned char Hue)
{
    if(Hue > 0 && Hue < 180)
        MainHue = Hue;

    if((MainHue + 20) > 180)
        HighHue = (MainHue + 20) -180;
    else
        HighHue = MainHue + 20;

    if((MainHue - 20) < 0)
        LowHue = (MainHue - 20) +180;
    else
        LowHue = MainHue - 20;

    Polygons.resize(0);
    Objects.resize(0);
}

bool PTX_Color::FitHueInterval(int Hue)
{

    if(LowHue < HighHue)
    {
        if((Hue > LowHue && Hue < HighHue))
        {
            return true;
        }
    }

    else
    {
        //Negative RED
        if((Hue > LowHue || Hue < HighHue))
        {
            return true;
        }
    }

    return false;
}
