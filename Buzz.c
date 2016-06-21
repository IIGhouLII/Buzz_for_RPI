#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "Buzz.h"

static int joystick_fd = -1;

int OpenJoystick(char *JoystickDevice)
{
	if (JoystickDevice == NULL)
		JoystickDevice = DEVNAME;
	joystick_fd = open(JoystickDevice, O_RDONLY | O_NONBLOCK); /* read write for force feedback? */
	if (joystick_fd < 0)
		return joystick_fd;

	/* maybe ioctls to interrogate features here? */

	return joystick_fd;
}

int ReadJoystick(struct Joystick *JoystickEvent)
{
	int bytes;

	bytes = read(joystick_fd, JoystickEvent, sizeof(*JoystickEvent)); 

	if (bytes == -1)
		return 0;

	if (bytes == sizeof(*JoystickEvent))
		return 1;

	printf("Unexpected bytes from controller: %d\n", bytes);

	return -1;
}

void CloseJoystick()
{
	close(joystick_fd);
}

int GetBuzzStatus(struct BuzzController *BuzzEvent)
{
	int rc;
	struct Joystick JoystickEvent;
	if (joystick_fd < 0)
		return -1;

	/* memset(wjse, 0, sizeof(*wjse)); */
	if ((rc = ReadJoystick(&JoystickEvent) == 1)) {
		//JoystickEvent.Type &= ~JS_EVENT_INIT; /* ignore synthetic events */
		
		BuzzEvent->ButtonID     =JoystickEvent.Number % 5;
		BuzzEvent->ControllerID =JoystickEvent.Number / 5;
		BuzzEvent->State         =JoystickEvent.Value;
		BuzzEvent->Time          =JoystickEvent.Time;
	}
	return rc;
}
