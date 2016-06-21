/*
 * Thanks scrary reasoner for the starting point 
 * https://scaryreasoner.wordpress.com/2008/02/22/programming-joysticks-with-linux/
 * 
 * Made few changes to make it work for the wireless Buzz controller
 * 
 * Compile using:
 * gcc -o Buzz main.c Buzz.c
*/
#include "Buzz.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int fd, rc;
	int done = 0;

	struct BuzzController Buzz;

	if (argc==0)
		fd = OpenJoystick(NULL);
	else 
		fd = OpenJoystick(argv[1]);
		
	if (fd < 0) {
		printf("open failed.\n");
		exit(1);
	}
	
	// Flush the initial output messages
	int i=0;
	while (i<20){
		rc = GetBuzzStatus(&Buzz);
		usleep(1000);
		if (rc == 1) i=i+1;
	}
	
	
	while (!done) {
		rc = GetBuzzStatus(&Buzz);
		usleep(1000);
		if (rc == 1) {
			printf("Event: Time %u, ControllerID: %u, Button: %u, State: %u \n", Buzz.Time, Buzz.ControllerID, Buzz.ButtonID, Buzz.State);
			
			// Here you put whatever script you want to run once a button is pushed

		}
	}
}
