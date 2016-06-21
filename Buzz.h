#include <stdio.h>
#ifndef __BUZZ_H__
#define __BUZZ_H__

#define DEVNAME "/dev/input/js0"

#define JS_EVENT_BUTTON         0x01    /* button pressed/released */
#define JS_EVENT_AXIS           0x02    /* joystick moved */
#define JS_EVENT_INIT           0x80    /* initial state of device */


struct Joystick {
	unsigned int 	Time;
	unsigned short 	Value;
	unsigned char 	Type;
	unsigned char 	Number;
};

struct BuzzController {
	unsigned int ControllerID;
	unsigned int ButtonID;
	unsigned int State;
	unsigned int Time;
};

extern int  OpenJoystick(char *JoystickDevice);
extern int  ReadJoystick(struct Joystick *JoystickEvent);
extern void CloseJoystick();
extern int  GetBuzzStatus(struct BuzzController *BuzzEvent);

#endif