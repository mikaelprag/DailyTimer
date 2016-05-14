/*
DailyTimer.h Library
BulldogLowell@gmail.com
April, 2016
** Library provides tools to set daily timers for control of devices such as lamps, appliances, etc. Developed primarilary for houshold presence simulation.
** Allows for setting ON and OFF times, days of the week (i.e. weekends, weekdays, Fridays) and the use of random on/off times using this Constructor:
   DailyTimer myTimer( START_HOUR, START_MINUTE, END_HOUR, END_MINUTE, DAYS_OF_WEEK, RANDOM or FIXED)
   
** Timers may bridge midnight, simply enter times accordingly:
   DailyTimer myTimer( 18, 30,  1, 45, WEEKENDS, FIXED);  // starts at 6:30pm Saturdays and Sundays and ends at 1:45am the next day.
   
** Automatically sets correct timer state on powerup, using isActive() in loop(), see example below.
** Random start and/or end times using this member function:
   myTimer.setRandomOffset(30, RANDOM_START);  //  Can be RANDOM_START, RANDOM_END, or both (RANDOM)  default random offfset is 15min
   
** Random days of week using this member function:
   myTimer.setRandomDays(4); // will select four days of the week, randomly.  Useful to run this member function once a week, for example.
   
** Select custom days of the week using this member function:
   myTimer.setDaysActive(0b10101010);  // e.g.Sunday, Tuesday, Thursday and Saturday Note: Days in this order:  0bSMTWTFS0 <- LSB is zero 
   
** Set a timed event with just the start time as a trigger:
   myTimer.startTrigger();  // will return true when Start Time is passed if today is an active day. use it in loop() 
   
** you can return the active days using this getter:
   byte myByte = myTimer.getDays();  // returns a byte... Sunday is the MSB, Saturday is the LSB << 1
   
** Dynamically set your start or end time (i.e. using some type of celestial function or web call to determine Sunrise or Sunset times)
   myTimer.setStartTime(byte hour, byte minute);
   myTimer.setEndTime(byte hour, byte minute);
   
*/
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//Program Start


#include "DailyTimer.h"
#include <Time.h>

DailyTimer timer1( 14, 43, 14, 55, EVERY_DAY, RANDOM, customSeedGenerator);   // optional callback function for random number generation, see below example
DailyTimer timer2( 14, 38, 23, 59, SATURDAY, FIXED);                          // default is FIXED, this will randomize the start time only
DailyTimer timer3( 14, 39,  0,  1, WEEKENDS);                                 // creates with a fixed start time and end time
bool timer1_LastState = false;

void setup() 
{
  setTime(1461422261);                        // set your sync provider!!
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  //timer1.setDaysActive(WEEKENDS);           // Set the timer to be active on weekends
  //timer1.setDaysActive(B10101010);          // or define a custom day mask... SMTWTFS0
  //timer1.setRandomDays(4);                  // or four random days per week
  timer2.setRandomOffset(5, RANDOM);          // Change to random start time, +/- 5 minutes... max 59 mins
  Serial.println(timer1.getDays(), BIN);      // getDays() returns active days as a byte in the format above
  Serial.print(F("Today is "));
  Serial.println(dayStr(weekday()));
}

uint32_t lastUpdateTime = 0;

void loop() 
{
  
  bool timerState = timer1.isActive();
  if(timerState != timer1_LastState)
  {
    if(timerState)
    {
      digitalWrite(13, HIGH);
      Serial.println(F("Timer 1 is ON"));
    }
    else
    {
      digitalWrite(13, LOW);
      Serial.println(F("Timer 1 is OFF"));
    }
    timer1_LastState = timerState;
  }
  if(timer2.startTrigger()) 
  {
    Serial.println(F("Timer 2 FIRED!"));
  }
  
  if(timer3.startTrigger())
  {
    Serial.println(F("Timer 3 FIRED!"));
  }
  if(millis() - lastUpdateTime > 1000UL)
  {
    char timeBuffer[32] = "";
    sprintf(timeBuffer, "%02d/%02d/%d %02d:%02d:%02d" , month(), day(), year(), hour(), minute(), second());
    Serial.println(timeBuffer);
    lastUpdateTime += 1000UL;
  }
  Serial.print(F("Timer1 is: "));
  Serial.println(timerState? "Active" : "Inactive");
  delay(1000);
}

uint32_t customSeedGenerator()
{
  Serial.println(F("New Seed Created"));
  uint32_t seed = analogRead(A5);
  delay(100);
  seed << 8;
  return  seed |= analogRead(A5);
}
