//Joystick (INCOMPLETE IMPLIMENTATION)
//Author: Carter Sharer 
//Date: 10/18/16

#include "Joystick.h"
#include "mbed.h"

//Member function definitions including constructor
Joystick::Joystick(PinName pinA, PinName pinB) : horiz(pinA), vert(pinB) {
    //Wait 1 second for voltage to settle
    wait(1); 
    
    //(1)Set Raw Center Values, this is where the joystick sits naturaly 
    //set raw horizontal center to current value (average with 5 readings)
    //set raw vertial center to current value (average with 5 readings) 
    raw_hc = (horiz.read() + horiz.read() + horiz.read() + horiz.read() + horiz.read()) / 5.0;
    raw_vc = (vert.read() + vert.read() + vert.read() + vert.read() + vert.read()) / 5.0; 
    
    //(2)Initalize the Rax Max to some value less then the real max value.  
    //We dont know what the max value will be until we read it, thats ok. 
    //But we can assume it will be greater then 0.8 (center + 0.3) so lets 
    //set it to that for now.  We will update this later if we see a larger value
    //Now do the same for the the Raw Min
    float delta = 0.3;
    rawMinH = raw_hc - delta;
    rawMaxH = raw_hc + delta;
    rawMinV = raw_hc - delta;
    rawMaxV = raw_hc + delta;
}

//Returns the scaled vertial value of joystick
float Joystick::horizontal(void) {
    //(3)Get average val (5 samples)
    float avg = (horiz.read() + horiz.read() + horiz.read() + horiz.read() + horiz.read()) / 5.0;
    
    //(4)Watch for Max and Min Values, if we see a new max/min update Raw Max/Min
    if (avg > rawMaxH) {
        rawMaxH = avg;    
    }
    if (avg < rawMinH) {
        rawMinH = avg;    
    }
    
    //(5)Here we will calculate the total range (Travel) of the joystick 
    //using the rawMax/rawMin values we have seen thus far
    //Since the joystick is not symetrical we have to calculate two ranges
    //Calculate the range from [center, max] and [center, min]
    float range_pos = rawMaxH - raw_hc;
    float range_neg = raw_hc - rawMinH;
    
    //(6)Here we will calculate how much our current reading is in one 
    //of the ranges, this will give us a percentage of our _max value we 
    //set in setScale. Then we can apply the scale by multiplying it by our 
    //scale (_max/_min). 
    float val;
    if(avg >= raw_hc) //Positive Range
        val = (avg - raw_hc) / (range_pos);
    else  //Negative Range
        val = (avg - raw_hc) / (range_neg);
         
    //(7)Here we will apply a dead zone.  If the |value| is <= our deadzone then 
    //set it to 0.  Otherwise we need to shift the value closer to 0 by dead zone
    
    if (val < 0) {
        if (-val < 0.1) {
            val = 0;
        }
    }
    if (val > 0) {
        if (val < 0.1) {
            val = 0;
        }
    }
    return val * 100;
}

//(8) Impliment vertial the same as you did for horizontal
//Returns the scaled horizontal value of joystick
float Joystick::vertical(void) {
    //Get average value (5 samples)
    float avg = (vert.read() + vert.read() + vert.read() + vert.read() + vert.read()) / 5.0;
    
    //(4)Watch for Max and Min Values, if we see a new max/min update Raw Max/Min
    if (avg > rawMaxV) {
        rawMaxV= avg;    
    }
    if (avg < rawMinV) {
        rawMinV= avg;    
    } 
    //(5)Here we will calculate the total range (Travel) of the joystick 
    //using the rawMax/rawMin values we have seen thus far
    //Since the joystick is not symetrical we have to calculate two ranges
    //Calculate the range from [center, max] and [center, min]
    float range_pos = rawMaxV - raw_vc;
    float range_neg = raw_vc - rawMinV;
    
    //(6)Here we will calculate how much our current reading is in one 
    //of the ranges, this will give us a percentage of our _max value we 
    //set in setScale. Then we can apply the scale by multiplying it by our 
    //scale (_max/_min).
    float val;
    if(avg >= raw_hc) //Positive Range
        val = (avg - raw_vc) / (range_pos);
    else  //Negative Range
        val = (avg - raw_vc) / (range_neg);
    
    //(7)Here we will apply a dead zone.  If the |value| is <= our deadzone then 
    //set it to 0.  Otherwise we need to shift the value closer to 0 by dead zone    
    
    if (val < 0) {
        if (-val < 0.1) {
            val = 0;
        }
    }
    if (val > 0) {
        if (val < 0.1) {
            val = 0;
        }
    }
    return val * 100;
}

//Set the Min and Max Values of joystick ex: -100, +100
void Joystick::setScale(float min, float max) {
       _min = min-1; //Add 1 for round off error
       _max = max+1; //Add 1 for round off error
}