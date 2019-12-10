#include <stdio.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <sys/stat.h>           // for the inode 
#include <stdlib.h> 

int main(int argc, char *argv[])
{
	FILE *fp;

	//char buff[50];
	int fd;		//file description 
	int wd;		//watch description 
	if(argc < 2)
	{
		perror("No directory to watch over");
		exit(1);
	}
	
	fp = fopen(argv[1], "r");
	//char c;
	//while((c = getc(fp)) != EOF)
	//	printf("%c", c);
	
	fd = inotify_init();	//create inotify for file descriptor
	if( fd == -1 )
	{
		perror("Failed to initialise, bye");
	}

	wd = inotify_add_watch(fd, argv[1], IN_MODIFY | IN_CREATE | IN_DELETE);
	if(wd < 0)
	{
		perror("Failed to initialise to watch file descriptor");
		exit(1);
	}
	fclose(fp);

	return 0;
}

