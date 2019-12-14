#include <stdio.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <sys/stat.h>           // for the inode 
#include <stdlib.h> 
#include <errno.h>

struct watch{

	int fd; //file descriptor
	int wd; //watch description
};

int main(int argc, char *argv[])
{
	FILE *fp;

	if(argc < 2)
	{
		perror("No directory to watch over");
		exit(1);
	}
	
	fp = fopen(argv[1], "r");

	struct watch o_w;
	o_w.fd = inotify_init();	//create inotify for file descriptor
	
	if( o_w.fd == -1 )
	{
		perror("Failed to initialise, bye");
	}

	o_w.wd = inotify_add_watch(o_w.fd, argv[1], IN_ALL_EVENTS);

	if(o_w.wd < 0)
	{
		perror("Failed to initialise to watch file descriptor");
		exit(1);
	}
	fclose(fp);

	return 0;
}


