/*
 * Thanks scrary reasoner for the starting point 
 * https://scaryreasoner.wordpress.com/2008/02/22/programming-joysticks-with-linux/
 * 
 * Made few changes to make it work for the wireless Buzz controller
 * 
 * Compile using:
 * gcc -o Buzz main.c Buzz.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include "Buzz.h"
int main(int argc, char* argv[]){
	int fd, rc;
	int done = 0;

	FILE *fp= NULL;
	pid_t process_id = 0;
	pid_t sid = 0;
	// Create child process
	process_id = fork();
	// Indication of fork() failure
	if (process_id < 0){
		printf("fork failed!\n");
		// Return failure in exit status
		exit(1);
	}
	// PARENT PROCESS. Need to kill it.
	if (process_id > 0){
		printf("process_id of child process %d \n", process_id);
		// return success in exit status
		exit(0);
	}
	//unmask the file mode
	umask(0);
	//set new session
	sid = setsid();
	if(sid < 0){
	// Return failure
	exit(1);
	}
	// Change the current working directory to root.
	chdir("/home/raspberry/.log/");
	// Close stdin. stdout and stderr
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	// Open a log file in write mode.
	fp = fopen ("buzz.log", "w+");

	struct BuzzController Buzz;

	if (argc==0)
		fd = OpenJoystick(NULL);
	else 
		fd = OpenJoystick(argv[1]);
		
	if (fd < 0) {
		fprintf(fp, "Open failed.\n");
		fflush(fp);
		exit(1);
	}
	
	// Jump Initial Output
	int i=0;
	while (i<20){
		rc = GetBuzzStatus(&Buzz);
		usleep(1000);
		if (rc == 1) i=i+1;
	}

	while (1){
		rc = GetBuzzStatus(&Buzz);
		usleep(1000);
		if (rc == 1) {
			fprintf(fp, "Event: Time %u, ControllerID: %u, Button: %u, State: %u \n", Buzz.Time, Buzz.ControllerID, Buzz.ButtonID, Buzz.State);
			fflush(fp);

			if (Buzz.ControllerID==0){
				if ((Buzz.ButtonID==0)&&(Buzz.State==1)){
					system("RPIConnect -mp play_pause");
				}else if ((Buzz.ButtonID==1)&&(Buzz.State==1)){
					system("RPIConnect -mp next");
				}
				else if ((Buzz.ButtonID==2)&&(Buzz.State==1)){
					system("RPIConnect -mp prev");
				}
				else if ((Buzz.ButtonID==3)&&(Buzz.State==1)){
					system("RPIConnect -mp -");
				}
				else if ((Buzz.ButtonID==4)&&(Buzz.State==1)){
					system("RPIConnect -mp +");
				}
			}else if (Buzz.ControllerID==1){
				if ((Buzz.ButtonID==0)&&(Buzz.State==1)){
					system("RPIConnect -talk ip");
				}
				else if ((Buzz.ButtonID==1)&&(Buzz.State==1)){
					system("RPIConnect -talk time");
				}
			}

			// For later Speech recognition applications (requires pulseuaudio, bluez and bluetooth headset)
			// if ((Buzz.ButtonID==0)&&(Buzz.State==1)){
			// 	system("/usr/bin/paplay -r /home/raspberry/.tmp/test.wav &");
			// 	while (rc = GetBuzzStatus(&Buzz) == 0){
			// 		usleep(1000);
			// 	}
			// 	fprintf(fp, "Event: Time %u, ControllerID: %u, Button: %u, State: %u \n", Buzz.Time, Buzz.ControllerID, Buzz.ButtonID, Buzz.State);
			// 	fflush(fp);
			// 	system("killall paplay");
			// }else if ((Buzz.ButtonID==1)&&(Buzz.State==1)){
			// 	system("/usr/bin/paplay -p /home/raspberry/.tmp/test.wav");
			// }
		}
	}
	fclose(fp);
	return (0);
}