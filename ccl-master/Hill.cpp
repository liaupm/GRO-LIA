
#include "Hill.h"

float hill(float Ka, float L, float n)
{
    return 1/(pow((Ka/L),n)+1);
}

bool conc_pct_step(float max_conc, float pct, float current_conc)
{
    if(pct > 1.0 || pct < 0 || max_conc <= 0 || current_conc < 0)
    {
        return false;
    }
    else if(current_conc >= max_conc*pct)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool time_pct_step(float tmax, float pct, float current_time)
{
    if(pct > 1.0 || pct < 0 || tmax < 0 || current_time < 0)
    {
        return false;
    }
    else if(current_time >= tmax*pct)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool step(float current, float threshold)
{
    //current > threshold ? true : false ;

    if(current > threshold)
    {
        return true;
    }
    else
    {
        return false;
    }
}
