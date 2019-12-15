#include <stdio.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <sys/stat.h>           // for the inode 
#include <stdlib.h> 
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 

#define IE_SIZE (sizeof(struct inotify_event))
#define BUFF (1024 * (IE_SIZE))

struct watch{

	int fd; //file descriptor
	int wd; //watch description
};

typedef enum {false, true} bool;

static struct watch *root;
static bool verbose = true;
void *watcher(struct watch o_w);

int main(int argc, char *argv[])
{
	struct watch o_w;
	FILE *fp;
	
	//root = (struct watch *) malloc(sizeof(struct watch));

	if(argc < 2)
	{
		perror("No directory to watch over");
		exit(1);
	}
	
	fp = fopen(argv[1], "r");
	
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

void *watch(struct watch o_w)
{
	char buffer[BUFF];
	int size;
	int i = 0;

	size = read(o_w.fd, buffer, BUFF);
	
	while(i < size)
	{
		struct inotify_event *p = (struct inotify_event*)&buffer[i];
		
		if(p -> len)
		{
			int masking = (p -> mask&IN_CREATE || p-> mask&IN_MODIFY || p-> mask&IN_DELETE || p-> mask&IN_DELETE_SELF);
	
		if(verbose)
		{
			if(p->mask&IN_MOVED_FROM || masking)
			{
				time_t t = time(NULL);
				struct tm *p = localtime(&t);

				char c[25];
				strftime(c, sizeof(c), "%c", p);	//%c date and time representation
				printf("{ %s }", c);

			}
			
			if(p -> mask&IN_CREATE)
			{
				printf("Created %s", p->name);

			}else if(p -> mask&IN_MODIFY)
			{
				printf("Modified");
			}else if(p -> mask&IN_MOVED_FROM)
			{
				printf("Renamed");
				
			}else if(p -> mask&IN_MOVED_TO)
			{
				printf("%s", p->name);
			}
			}else if(p -> mask&IN_DELETE)
			{
				printf("Deleted %s", p->name);
			}else if(p -> mask&IN_DELETE_SELF)
			{
				printf("The directory \"%s\" was deleted. \n ", p->name);
			}

			
			if(masking)
			{
				printf("{");
			}

			if(masking)
			{
				printf("}");
			}

			if(p-> mask&IN_MOVED_FROM)
			{
				printf("{");
			}

			if(p-> mask&IN_MOVED_FROM)
			{
				printf("}");
			}
			
			if(masking)
			{
				printf("%s", p-> name);
			}	

			if(p-> mask&IN_MOVED_FROM)
			{
				printf("%s", p->name);
			}
			
			if(masking)
			{
				printf("{");
				if(p-> mask&IN_ISDIR)
				{
					printf("Directory");
				}
				else
				{
					printf("File");
				}
				printf("}");
			}

			if(p-> mask&IN_MOVED_FROM)
			{
				printf("{");
				if (p-> mask&IN_ISDIR)
				{
					printf("Directory");
				}
				else
				{
					printf("File");
				}
				printf("}");
			}

		}

	}

	
}
