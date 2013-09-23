#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#define hours hours24%12
extern int hours24;
extern int minutes;
extern int seconds;

#define a_hours a_hours24%12

extern int a_hours24;
extern int a_minutes;
extern int a_seconds;

#define hours_temp hours24_temp%12
extern int hours24_temp;
extern int minutes_temp;
extern int seconds_temp;

#define a_hours_temp a_hours24_temp%12

extern int a_hours24_temp;
extern int a_minutes_temp;
extern int a_seconds_temp;

//inactivity timer
extern int inacTimer;

//ring alarms
extern int ringAlarms;

//is sound playing currently
extern int soundPlaying;

//flags for current mode

//displayMode = 0 for display time
//displayMode = 1 for set time

//timeMode = 0 for analog
//timeMode = 1 for digital

//setMode = 0 for time
//setMode = 1 for alarm

extern int displayMode;
extern int timeMode;
extern int setMode;

extern int ringAlarms;





#endif